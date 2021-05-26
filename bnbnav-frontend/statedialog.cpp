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
#include "player.h"
#include "statemanager.h"
#include "datamanager.h"
#include "instructionmodel.h"

StateDialog::StateDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::StateDialog) {
    ui->setupUi(this);

    ui->instructionList->setModel(new InstructionModel(this));
    ui->instructionList->setItemDelegate(new InstructionDelegate(this));

    connect(StateManager::instance(), &StateManager::routeChanged, this, [ = ] {
        ui->goModeButton->setEnabled(!StateManager::currentRoute().isEmpty());
    });
    connect(StateManager::instance(), &StateManager::stateChanged, this, [ = ] {
        if (StateManager::currentState() == StateManager::Go) {
            ui->stackedWidget->setCurrentWidget(ui->goModePage);
            this->setFixedSize(400, StateManager::currentInstructions().first().height());
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
            //Reroute!
            Player* player = DataManager::players().value(StateManager::login());

            QPoint start(player->x(), player->z());

            DataManager::resetTemporaries();
            StateManager::setCurrentRoute(DataManager::shortestPath(start, ui->endLocationBox->location()));
        }
    });

    QPalette pal = ui->currentInstructionWidget->palette();
    pal.setBrush(QPalette::Window, QColor(100, 100, 100));
    pal.setBrush(QPalette::WindowText, Qt::white);
    ui->currentInstructionWidget->setPalette(pal);

    ui->currentInstructionWidget->installEventFilter(this);
    ui->stackedWidget->setCurrentWidget(ui->normalModePage);
}

StateDialog::~StateDialog() {
    delete ui;
}

void StateDialog::on_getDirectionsButton_clicked() {
    QPoint start = ui->startLocationBox->location();
    QPoint end = ui->endLocationBox->location();
    if (start.isNull() || end.isNull()) {
        QMessageBox::warning(this, tr("Please enter a valid location"), tr("We can't find that location. Please enter a valid location."));
        return;
    }

    DataManager::resetTemporaries();
    StateManager::setCurrentRoute(DataManager::shortestPath(start, end));
}

void StateDialog::on_startLocationBox_textChanged(const QString& arg1) {
    StateManager::setCurrentRoute({});
}

void StateDialog::on_endLocationBox_textChanged(const QString& arg1) {
    StateManager::setCurrentRoute({});
}

void StateDialog::on_goModeButton_clicked() {
    StateManager::setCurrentState(StateManager::Go);
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
