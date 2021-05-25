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
#include "loginwidget.h"
#include "ui_loginwidget.h"

#include "statemanager.h"

LoginWidget::LoginWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget) {
    ui->setupUi(this);

    connect(StateManager::instance(), &StateManager::loginChanged, this, [ = ](QString login) {
        if (login.isEmpty()) {
            ui->stackedWidget->setCurrentWidget(ui->loginPage);
        } else {
            ui->stackedWidget->setCurrentWidget(ui->userPage);
        }
        ui->userPageTitle->setText(tr("Hello %1!").arg(login));
        this->setFixedSize(ui->stackedWidget->currentWidget()->sizeHint());
    });
    if (StateManager::login().isEmpty()) {
        ui->stackedWidget->setCurrentWidget(ui->loginPage);
    } else {
        ui->stackedWidget->setCurrentWidget(ui->userPage);
    }
    ui->userPageTitle->setText(tr("Hello %1!").arg(StateManager::login()));
}

LoginWidget::~LoginWidget() {
    delete ui;
}

void LoginWidget::on_stackedWidget_currentChanged(int arg1) {
    this->setFixedSize(ui->stackedWidget->currentWidget()->sizeHint());
}

void LoginWidget::on_loginButton_clicked() {
    StateManager::setLogin(ui->loginUsernameBox->text());
}

void LoginWidget::on_logOutButton_clicked() {
    StateManager::setLogin("");
}

