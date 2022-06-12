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
#include "newlabeldialog.h"
#include "ui_newlabeldialog.h"

#include "landmark.h"
#include "node.h"

#include "datagatherer.h"
#include "datamanager.h"
#include <QJsonObject>
#include <QMessageBox>

struct NewLabelDialogPrivate {
        Node* node;
};

NewLabelDialog::NewLabelDialog(Node* node, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NewLabelDialog) {
    ui->setupUi(this);
    d = new NewLabelDialogPrivate();
    d->node = node;

    QList<QPair<QString, QString>> landmarks = Landmark::labels();
    //    std::sort(landmarks.begin(), landmarks.end(), [ = ](const QPair<QString, QString>& first, const QPair<QString, QString>& second) {
    //        return first.second.localeAwareCompare(second.first) < 0;
    //    });

    for (QPair<QString, QString> landmarkType : qAsConst(landmarks)) {
        ui->typeBox->addItem(landmarkType.second, landmarkType.first);
    }
}

NewLabelDialog::~NewLabelDialog() {
    delete ui;
    delete d;
}

void NewLabelDialog::on_cancelButton_clicked() {
    this->reject();
}

void NewLabelDialog::on_okButton_clicked() {
    // Send data to the server
    this->setEnabled(false);
    DataGatherer::submit("/landmarks/add", {
                                               {"node", DataManager::nodes().key(d->node)    },
                                               {"name", ui->nameBox->text()                  },
                                               {"type", ui->typeBox->currentData().toString()}
    },
        [=](QByteArray data, bool error) {
        if (error) {
            this->setEnabled(true);
            QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
        } else {
            this->accept();
        }
    });
}
