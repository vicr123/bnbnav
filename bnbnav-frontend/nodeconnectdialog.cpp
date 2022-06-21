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
#include "nodeconnectdialog.h"
#include "ui_nodeconnectdialog.h"

#include "datagatherer.h"
#include "datamanager.h"
#include "newroaddialog.h"
#include "node.h"
#include "road.h"
#include "statemanager.h"
#include <QJsonObject>
#include <QMessageBox>
#include <QSet>

struct NodeConnectDialogPrivate {
        Node* first;
        Node* second = nullptr;

        QString firstKey, secondKey;

        QObject* autobuildObject = nullptr;
};

NodeConnectDialog::NodeConnectDialog(Node* first, Node* second, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NodeConnectDialog) {
    ui->setupUi(this);

    this->setWindowTitle(tr("New Edge"));
    ui->label->setText(tr("New Edge"));
    ui->cancelButton->setText(tr("Cancel"));
    ui->okButton->setText(tr("OK"));

    ui->addDirective->setVisible(false);

    d = new NodeConnectDialogPrivate();
    d->first = first;
    d->second = second;

    QSet<QString> roads;
    roads.unite(DataManager::roadsConnectedToNode(first));
    roads.unite(DataManager::roadsConnectedToNode(second));

    for (QString r : roads) {
        Road* road = DataManager::roads().value(r);
        ui->roadSelectionBox->addItem(QStringLiteral("%1 (%2)").arg(road->name(), road->humanReadableType()), r);
    }

    d->firstKey = DataManager::nodes().key(d->first);
    d->secondKey = DataManager::nodes().key(d->second);
}

NodeConnectDialog::NodeConnectDialog(Node* autoFrom, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NodeConnectDialog) {
    ui->setupUi(this);

    this->setWindowTitle(tr("Autobuild"));
    ui->label->setText(tr("Autobuild"));
    ui->cancelButton->setText(tr("Close"));
    ui->okButton->setText(tr("Start"));

    ui->addDirective->setVisible(false);

    d = new NodeConnectDialogPrivate();
    d->first = autoFrom;

    QSet<QString> roads;
    roads.unite(DataManager::roadsConnectedToNode(autoFrom));

    for (QString r : roads) {
        Road* road = DataManager::roads().value(r);
        ui->roadSelectionBox->addItem(QStringLiteral("%1 (%2)").arg(road->name(), road->humanReadableType()), r);
    }

    d->firstKey = DataManager::nodes().key(d->first);
}

NodeConnectDialog::~NodeConnectDialog() {
    delete d;
    delete ui;
}

void NodeConnectDialog::on_cancelButton_clicked() {
    this->reject();
}

void NodeConnectDialog::on_okButton_clicked() {
    if (d->autobuildObject) {
        d->autobuildObject->deleteLater();
        d->autobuildObject = nullptr;

        ui->roadSelectionBox->setEnabled(true);
        ui->newRoadButton->setEnabled(true);
        ui->twoWayBox->setEnabled(true);
        ui->addDirective->setVisible(false);
        ui->okButton->setText(tr("Start"));
    } else if (d->second) {
        // Submit changes to the server
        this->setEnabled(false);
        this->doConnect(d->first, d->second, ui->roadSelectionBox->currentData().toString(), ui->twoWayBox->isChecked(), [=](bool error) {
            if (error) {
                this->setEnabled(true);
                QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
                return;
            }

            this->accept();
        });
    } else {
        ui->roadSelectionBox->setEnabled(false);
        ui->newRoadButton->setEnabled(false);
        ui->twoWayBox->setEnabled(false);
        ui->addDirective->setVisible(true);
        ui->okButton->setText(tr("Stop"));

        d->autobuildObject = new QObject(this);
        connect(DataManager::instance(), &DataManager::newNode, d->autobuildObject, [=](Node* node, QString player) {
            if (player != StateManager::login()) return;
            this->doConnect(d->first, node, ui->roadSelectionBox->currentData().toString(), ui->twoWayBox->isChecked(), [=](bool error) {

            });

            d->first = node;
        });
    }
}

void NodeConnectDialog::on_newRoadButton_clicked() {
    auto* d = new NewRoadDialog("", this);
    d->setWindowModality(Qt::ApplicationModal);
    connect(d, &NewRoadDialog::accepted, this, [=] {
        ui->roadSelectionBox->addItem(d->addedRoadName(), d->addedRoadId());
        ui->roadSelectionBox->setCurrentIndex(ui->roadSelectionBox->count() - 1);
    });
    connect(d, &NewRoadDialog::finished, d, &QDialog::deleteLater);
    d->open();
}

void NodeConnectDialog::doConnect(Node* first, Node* second, QString road, bool twoWay, std::function<void(bool)> callback) {
    // Submit changes to the server
    DataGatherer::submit("/edges/add", {
                                           {"road",  road                            },
                                           {"node1", DataManager::nodes().key(first) },
                                           {"node2", DataManager::nodes().key(second)}
    },
        [=](QByteArray, bool error) {
        if (error) {
            callback(true);
            this->setEnabled(true);
            QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
            return;
        }

        if (twoWay) {
            // Add in the opposite direction
            DataGatherer::submit("/edges/add", {
                                                   {"road",  road                            },
                                                   {"node1", DataManager::nodes().key(second)},
                                                   {"node2", DataManager::nodes().key(first) }
            },
                [=](QByteArray, bool error) {
                callback(error);
            });
            return;
        }

        callback(false);
    });
}
