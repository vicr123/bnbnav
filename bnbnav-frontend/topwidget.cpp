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
#include "topwidget.h"
#include "ui_topwidget.h"

#include "statemanager.h"
#include <QInputDialog>
#include <QSettings>

TopWidget::TopWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TopWidget) {
    ui->setupUi(this);

    connect(StateManager::instance(), &StateManager::loginChanged, this, [=](QString login) {
        if (login.isEmpty()) {
            ui->loginButton->setText(tr("Log In"));
        } else {
            ui->loginButton->setText(login);
        }
    });
    if (StateManager::login().isEmpty()) {
        ui->loginButton->setText(tr("Log In"));
    } else {
        ui->loginButton->setText(StateManager::login());
    }

    connect(StateManager::instance(), &StateManager::stateChanged, this, [=] {
        ui->editModeButton->setVisible(StateManager::currentState() != StateManager::Go);
        ui->loginButton->setEnabled(StateManager::currentState() != StateManager::Go);
        ui->editModeButton->setChecked(StateManager::currentState() == StateManager::Edit);
    });
    connect(StateManager::instance(), &StateManager::nightModeChanged, this, [=] {
        ui->nightModeButton->setChecked(StateManager::nightMode());
    });
}

TopWidget::~TopWidget() {
    delete ui;
}

void TopWidget::setPan(int x, int y) {
    ui->panLabel->setText(QStringLiteral("x: %1 y: %2").arg(x).arg(y));
}

void TopWidget::on_editModeButton_toggled(bool checked) {
    if (checked && StateManager::token().isEmpty()) {
        bool ok;
        QString token = QInputDialog::getText(this->window(), tr("Token"), tr("Obtain a token with /editnav to edit"), QLineEdit::Normal, QString(), &ok);
        if (!ok) {
            ui->editModeButton->setChecked(false);
            return;
        }

        StateManager::setToken(token);
    }
    StateManager::setCurrentState(checked ? StateManager::Edit : StateManager::Browse);
}

void TopWidget::on_loginButton_clicked() {
    emit showLoginWidget();
}

void TopWidget::on_nightModeButton_toggled(bool checked) {
    StateManager::setNightMode(checked);
}
