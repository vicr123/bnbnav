/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2021 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "statedialog.h"
#include "ui_statedialog.h"

#include <QPainter>
#include <QMessageBox>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>
#include "player.h"
#include "statemanager.h"
#include "datamanager.h"
#include "instructionmodel.h"
#include "texttospeechengine.h"
#include "landmark.h"
#include "node.h"

struct StateDialogPrivate {
    QTimer* recalculateTimer;

    int lastPrompt = -1;
};

StateDialog::StateDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::StateDialog) {
    ui->setupUi(this);

    d = new StateDialogPrivate();
    d->recalculateTimer = new QTimer();
    d->recalculateTimer->setInterval(3000);
    d->recalculateTimer->setSingleShot(true);
    connect(d->recalculateTimer, &QTimer::timeout, this, [ = ] {
        if (StateManager::currentInstruction() == -1 && StateManager::currentState() == StateManager::Go) recalculateRoute();
    });

    ui->instructionList->setModel(new InstructionModel(this));
    ui->instructionList->setItemDelegate(new InstructionDelegate(this));

    connect(StateManager::instance(), &StateManager::routeChanged, this, [ = ] {
        ui->goModeButton->setEnabled(!StateManager::currentRoute().isEmpty());
    });
    connect(StateManager::instance(), &StateManager::stateChanged, this, [ = ] {
        if (StateManager::currentState() == StateManager::Go) {
            ui->stackedWidget->setCurrentWidget(ui->goModePage);
            this->setFixedSize(400, StateManager::currentInstructions().first().height() + ui->currentRouteWidget->sizeHint().height());
            this->move(this->parentWidget()->geometry().topLeft() + QPoint(20, 20));
        } else {
            ui->stackedWidget->setCurrentWidget(ui->normalModePage);
            this->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        }
    });
    connect(StateManager::instance(), &StateManager::currentInstructionChanged, this, [ = ] {
        int instruction = StateManager::currentInstruction();

        QPalette pal = ui->currentInstructionWidget->palette();
        pal.setBrush(QPalette::Window, instruction == -1 ? QColor(100, 100, 100) : QColor(0, 150, 0));
        pal.setBrush(QPalette::WindowText, Qt::white);
        ui->currentInstructionWidget->setPalette(pal);

        ui->currentInstructionWidget->update();

        if (instruction == -1 && StateManager::currentState() == StateManager::Go) {
            ui->routeInformationLabel->setText(tr("Calculating..."));
            recalculateRoute();
        } else if (instruction != -1) {
            StateManager::Instruction inst = StateManager::currentInstructions().at(instruction);
            if (inst.type == StateManager::Instruction::Arrival && StateManager::blocksToNextInstruction() < 20) {
                //That's it!
                QTimer::singleShot(3000, [ = ] {
                    //Leave Go mode
                    StateManager::setCurrentState(StateManager::Browse);
                });
            }

            int blocksToDestination = StateManager::blocksToNextInstruction();
            for (int i = StateManager::currentInstruction() + 1; i < StateManager::currentInstructions().length(); i++) {
                blocksToDestination += StateManager::currentInstructions().at(i).distance;
            }

            QStringList routeInformation;
            routeInformation.append(tr("%n blocks", nullptr, blocksToDestination));
            routeInformation.append(tr("%n instructions", nullptr, StateManager::currentInstructions().count() - StateManager::currentInstruction()));
            ui->routeInformationLabel->setText(routeInformation.join(" · "));

            if (StateManager::currentState() == StateManager::Go) {
                //TTS
                for (int i = 0; i < StateManager::voicePrompts().length(); i++) {
                    StateManager::InstructionVoicePrompt prompt = StateManager::voicePrompts().at(i);
                    if (prompt.atBlocks > blocksToDestination) {
                        if (d->lastPrompt != prompt.atBlocks) {
                            TextToSpeechEngine::instance()->say(prompt.speech());
                            d->lastPrompt = prompt.atBlocks;
                        }
                        break;
                    }
                }
            }
        }

    });
    connect(StateManager::instance(), &StateManager::selectedLandmarkChanged, this, &StateDialog::updateLandmark);

    QPalette pal = ui->currentInstructionWidget->palette();
    pal.setBrush(QPalette::Window, QColor(100, 100, 100));
    pal.setBrush(QPalette::WindowText, Qt::white);
    ui->currentInstructionWidget->setPalette(pal);

    ui->currentInstructionWidget->installEventFilter(this);
    ui->stackedWidget->setCurrentWidget(ui->normalModePage);
}

StateDialog::~StateDialog() {
    delete d;
    delete ui;
}

void StateDialog::routeTo(QPoint location) {
    ui->tabWidget->setCurrentWidget(ui->directionsTab);
    ui->endLocationBox->setText(QStringLiteral("%1,%2").arg(location.x()).arg(location.y()));
    ui->getDirectionsButton->click();
    this->show();
}

void StateDialog::routeFrom(QPoint location) {
    ui->tabWidget->setCurrentWidget(ui->directionsTab);
    ui->startLocationBox->setText(QStringLiteral("%1,%2").arg(location.x()).arg(location.y()));
    ui->getDirectionsButton->click();
    this->show();
}

void StateDialog::showLandmark(Landmark* landmark) {
    if (StateManager::currentState() == StateManager::Go) return;

    ui->tabWidget->setCurrentWidget(ui->discoverTab);
    this->show();
}

void StateDialog::on_getDirectionsButton_clicked() {
    QPoint start = ui->startLocationBox->location();
    QPoint end = ui->endLocationBox->location();
    QObject* fromEntity = nullptr;
    QObject* toEntity = nullptr;

    if (start.isNull() && StateManager::loggedInPlayer()) {
        start = QPoint(StateManager::loggedInPlayer()->x(), StateManager::loggedInPlayer()->z());
        fromEntity = StateManager::loggedInPlayer();
    }

    if (start.isNull() || end.isNull()) {
        QMessageBox::warning(this, tr("Please enter a valid location"), tr("We can't find that location. Please enter a valid location."));
        return;
    }

    DataManager::resetTemporaries();
    StateManager::setCurrentRoute(DataManager::shortestPath(start, end, fromEntity, toEntity));
}

void StateDialog::on_startLocationBox_textChanged(const QString& arg1) {
    StateManager::setCurrentRoute({});
}

void StateDialog::on_endLocationBox_textChanged(const QString& arg1) {
    StateManager::setCurrentRoute({});
}

void StateDialog::on_goModeButton_clicked() {
    if (StateManager::login().isEmpty()) {
        QMessageBox::warning(this, tr("Please Log In"), tr("Log in to use Go mode."));
        return;
    }
    StateManager::setCurrentState(StateManager::Go);

    d->lastPrompt = -1;
}

void StateDialog::recalculateRoute() {
    //Reroute!
    Player* player = DataManager::players().value(StateManager::login());

    QPoint start(player->x(), player->z());

    DataManager::resetTemporaries();
    StateManager::setCurrentRoute(DataManager::shortestPath(start, ui->endLocationBox->location(), StateManager::loggedInPlayer()));

    d->lastPrompt = -1;

    //Recalculate every 3 seconds until on track
    d->recalculateTimer->start();
}

void StateDialog::updateLandmark() {
    Landmark* l = StateManager::selectedLandmark();
    if (l) {
        ui->landmarkName->setText(l->name());
        ui->landmarkCoordinates->setText(tr("%1 at x: %2  y: %3  z: %4").arg(l->humanReadableType()).arg(l->node()->x()).arg(l->node()->y()).arg(l->node()->z()));
        ui->discoverStack->setCurrentWidget(ui->landmarkPage);
    } else {
        ui->discoverStack->setCurrentWidget(ui->noLandmarkPage);
    }
}

bool StateDialog::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->currentInstructionWidget) {
        if (event->type() == QEvent::Paint) {
            QPainter painter(ui->currentInstructionWidget);
            QRect rect(0, 0, ui->currentInstructionWidget->width(), ui->currentInstructionWidget->height());
            painter.fillRect(rect, ui->currentInstructionWidget->palette().color(QPalette::Window));

            int instruction = StateManager::currentInstruction();
            if (instruction != -1) {
                StateManager::Instruction inst = StateManager::currentInstructions().at(instruction);
                inst.render(&painter, rect, ui->currentInstructionWidget->font(), ui->currentInstructionWidget->palette(), StateManager::blocksToNextInstruction());
            } else {
                painter.setPen(ui->currentInstructionWidget->palette().color(QPalette::WindowText));
                painter.drawText(rect, Qt::AlignCenter, tr("Calculating route..."));
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* e = static_cast<QMouseEvent*>(event);
            QMenu* menu = new QMenu(this);
            menu->addAction(tr("Exit Go Mode"), this, [ = ] {
                StateManager::setCurrentState(StateManager::Browse);
            });
            menu->popup(e->globalPos());
        }
    }
    return false;
}

void StateDialog::on_directionsToLandmarkButton_clicked() {
    routeTo(QPoint(StateManager::selectedLandmark()->node()->x(), StateManager::selectedLandmark()->node()->z()));
}


void StateDialog::on_searchButton_clicked() {
    if (ui->searchBox->landmark()) {
        StateManager::setSelectedLandmark(ui->searchBox->landmark());
        focusMap(ui->searchBox->location());
    }
}

