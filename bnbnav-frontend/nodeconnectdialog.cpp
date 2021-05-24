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

#include <QSet>
#include <QJsonObject>
#include <QMessageBox>
#include "datagatherer.h"
#include "road.h"
#include "datamanager.h"
#include "newroaddialog.h"

struct NodeConnectDialogPrivate {
    Node* first;
    Node* second;

    QString firstKey, secondKey;
};

NodeConnectDialog::NodeConnectDialog(Node* first, Node* second, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NodeConnectDialog) {
    ui->setupUi(this);

    d = new NodeConnectDialogPrivate();
    d->first = first;
    d->second = second;

    QSet<QString> roads;
    roads.unite(DataManager::roadsConnectedToNode(first));
    roads.unite(DataManager::roadsConnectedToNode(second));

    for (QString road : roads) {
        ui->roadSelectionBox->addItem(DataManager::roads().value(road)->name(), road);
    }

    d->firstKey = DataManager::nodes().key(d->first);
    d->secondKey = DataManager::nodes().key(d->second);
}

NodeConnectDialog::~NodeConnectDialog() {
    delete d;
    delete ui;
}

void NodeConnectDialog::on_cancelButton_clicked() {
    this->reject();
}

void NodeConnectDialog::on_okButton_clicked() {
    //Submit changes to the server
    this->setEnabled(false);
    DataGatherer::submit("/edges/add", {
        {"road", ui->roadSelectionBox->currentData().toString()},
        {"node1", d->firstKey},
        {"node2", d->secondKey}
    }, [ = ](QByteArray data, bool error) {
        if (error) {
            this->setEnabled(true);
            QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
            return;
        }

        if (ui->twoWayBox->isChecked()) {
            //Add in the opposite direction
            DataGatherer::submit("/edges/add", {
                {"road", ui->roadSelectionBox->currentData().toString()},
                {"node1", d->secondKey},
                {"node2", d->firstKey}
            }, [ = ](QByteArray data, bool error) {
                if (error) {
                    this->setEnabled(true);
                    QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server. A one way road was created!"));
                    return;
                }

                this->accept();
            });
        }

        this->accept();
    });
}

void NodeConnectDialog::on_newRoadButton_clicked() {
    NewRoadDialog* d = new NewRoadDialog(this);
    d->setWindowModality(Qt::ApplicationModal);
    connect(d, &NewRoadDialog::accepted, this, [ = ] {
        ui->roadSelectionBox->addItem(d->addedRoadName(), d->addedRoadId());
        ui->roadSelectionBox->setCurrentIndex(ui->roadSelectionBox->count() - 1);
    });
    connect(d, &NewRoadDialog::finished, d, &QDialog::deleteLater);
    d->open();
}

