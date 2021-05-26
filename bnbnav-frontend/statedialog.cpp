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

#include <QMessageBox>
#include "statemanager.h"
#include "datamanager.h"

StateDialog::StateDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::StateDialog) {
    ui->setupUi(this);
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

