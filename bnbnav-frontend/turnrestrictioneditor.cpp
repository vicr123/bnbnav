/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2022 Victor Tran
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
#include "turnrestrictioneditor.h"
#include "ui_turnrestrictioneditor.h"

#include "datamanager.h"
#include "edge.h"
#include "node.h"
#include "road.h"
#include "statemanager.h"
#include <QCheckBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>
#include <QPicture>
#include <QPushButton>

struct TurnRestrictionEditorPrivate {
        Node* node;
        QPicture baseImage;

        QMap<QPushButton*, Edge*> stage1Buttons;
        QMap<QCheckBox*, Edge*> stage2Boxes;

        //        QList<QPair<QString, QString>> prohibitions;
        QMultiMap<QString, QString> restrictions;

        Edge* editingEdge;
};

TurnRestrictionEditor::TurnRestrictionEditor(Node* node, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::TurnRestrictionEditor) {
    ui->setupUi(this);
    d = new TurnRestrictionEditorPrivate();
    d->node = node;

    this->setFixedSize(QSize(300, 300));

    auto obj = d->node->annotation("turn-restrictions");
    if (obj.contains("restrictions")) {
        auto restrictions = obj.value("restrictions").toArray();
        for (auto restriction : restrictions) {
            auto restrictionObject = restriction.toObject();
            d->restrictions.insert(restrictionObject.value("from").toString(), restrictionObject.value("to").toString());
        }
    }

    QPalette pal;
    if (StateManager::nightMode()) {
        pal.setColor(QPalette::Window, QColor(0, 30, 50));
        pal.setColor(QPalette::WindowText, Qt::white);
    } else {
        pal.setColor(QPalette::Window, Qt::white);
        pal.setColor(QPalette::WindowText, Qt::black);
    }
    this->setPalette(pal);

    this->calculateBaseImage();
    on_editOtherEdgeButton_clicked();
}

TurnRestrictionEditor::~TurnRestrictionEditor() {
    delete ui;
    delete d;
}

void TurnRestrictionEditor::calculateBaseImage() {
    QPainter painter(&d->baseImage);

    for (auto* fromEdge : DataManager::edgesToNode(d->node)) {
        if (fromEdge->isTemporary()) continue;
        QLineF line(this->width() / 2, this->height() / 2, this->width() / 2 + 100, this->height() / 2);
        line.setAngle(fromEdge->line().angle() + 180);

        painter.setPen(fromEdge->road()->pen(fromEdge));
        painter.drawLine(line);

        auto* toButton = new QPushButton(this);
        toButton->setText(this->fontMetrics().elidedText(fromEdge->road()->name(), Qt::ElideMiddle, 100));
        toButton->setToolTip(fromEdge->road()->name());

        QRect geom;
        geom.setSize(toButton->sizeHint());
        geom.moveCenter(line.pointAt(0.9).toPoint());
        toButton->setGeometry(geom);
        toButton->setVisible(true);

        d->stage1Buttons.insert(toButton, fromEdge);
        connect(toButton, &QPushButton::clicked, this, [=] {
            d->editingEdge = fromEdge;
            for (auto* checkbox : d->stage2Boxes.keys()) {
                QSignalBlocker blocker(checkbox);

                auto toEdge = d->stage2Boxes.value(checkbox);
                checkbox->setChecked(!d->restrictions.contains(fromEdge->id(), toEdge->id()));
                checkbox->setVisible(true);
            }
            for (auto* button : d->stage1Buttons.keys()) {
                button->setVisible(false);
            }
            ui->editOtherEdgeButton->setVisible(true);
        });
    }

    for (auto* toEdge : DataManager::edgesFromNode(d->node)) {
        if (toEdge->isTemporary()) continue;
        QLineF line(this->width() / 2, this->height() / 2, this->width() / 2 + 100, this->height() / 2);
        line.setAngle(toEdge->line().angle());

        painter.setPen(toEdge->road()->pen(toEdge));
        painter.drawLine(line);

        auto* toButton = new QCheckBox(this);
        toButton->setToolTip(toEdge->road()->name());

        QRect geom;
        geom.setSize(toButton->sizeHint());
        geom.moveCenter(line.pointAt(0.9).toPoint());
        toButton->setGeometry(geom);
        d->stage2Boxes.insert(toButton, toEdge);

        connect(toButton, &QCheckBox::toggled, this, [=](bool checked) {
            if (checked) {
                d->restrictions.remove(d->editingEdge->id(), toEdge->id());
            } else {
                d->restrictions.insert(d->editingEdge->id(), toEdge->id());
            }
        });
    }
}

void TurnRestrictionEditor::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.fillRect(QRect(0, 0, this->width(), this->height()), this->palette().color(QPalette::Window));
    painter.drawPicture(QPoint(0, 0), d->baseImage);
}

void TurnRestrictionEditor::on_editOtherEdgeButton_clicked() {
    for (auto* checkbox : d->stage2Boxes.keys()) {
        checkbox->setVisible(false);
    }
    for (auto* button : d->stage1Buttons.keys()) {
        button->setVisible(true);
    }
    ui->editOtherEdgeButton->setVisible(false);
}

void TurnRestrictionEditor::on_applyButton_clicked() {
    QJsonArray restrictions;
    for (const auto& key : d->restrictions.keys()) {
        for (const auto& value : d->restrictions.values(key)) {
            restrictions.append(QJsonObject({
                {"from", key  },
                {"to",   value}
            }));
        }
    }

    QJsonObject annotation;
    annotation.insert("restrictions", restrictions);

    this->setEnabled(false);
    d->node->submitAnnotationUpdate("turn-restrictions", annotation, [=](bool error) {
        if (error) {
            this->setEnabled(true);
            QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
        } else {
            this->accept();
        }
    });
}

void TurnRestrictionEditor::on_clearTurnRestrictionsButton_clicked() {
    this->setEnabled(false);
    d->node->submitAnnotationDelete("turn-restrictions", [=](bool error) {
        if (error) {
            this->setEnabled(true);
            QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
        } else {
            this->accept();
        }
    });
}
