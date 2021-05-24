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
#include "newroaddialog.h"
#include "ui_newroaddialog.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "datagatherer.h"

struct NewRoadDialogPrivate {
    QString roadId;
};

NewRoadDialog::NewRoadDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::NewRoadDialog) {
    ui->setupUi(this);

    d = new NewRoadDialogPrivate();

    ui->typeBox->addItem(tr("Local Road"), "local");
    ui->typeBox->addItem(tr("Main Road"), "main");
    ui->typeBox->addItem(tr("Highway"), "highway");
    ui->typeBox->addItem(tr("Motorway"), "motorway");
}

NewRoadDialog::~NewRoadDialog() {
    delete ui;
}

QString NewRoadDialog::addedRoadId() {
    return d->roadId;
}

QString NewRoadDialog::addedRoadName() {
    return ui->nameBox->text();
}

void NewRoadDialog::on_cancelButton_clicked() {
    this->reject();
}

void NewRoadDialog::on_okButton_clicked() {
    //Send data to the server
    this->setEnabled(false);
    DataGatherer::submit("/roads/add", {
        {"name", ui->nameBox->text()},
        {"type", ui->typeBox->currentData().toString()}
    }, [ = ](QByteArray data, bool error) {
        if (error) {
            this->setEnabled(true);
            QMessageBox::warning(this, tr("Could not submit data"), tr("Could not submit data to the server"));
        } else {
            d->roadId = QJsonDocument::fromJson(data).object().value("id").toString();
            this->accept();
        }
    });
}

