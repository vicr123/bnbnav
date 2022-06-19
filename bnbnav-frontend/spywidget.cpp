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
#include "spywidget.h"
#include "ui_spywidget.h"

#include "statemanager.h"

SpyWidget::SpyWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SpyWidget) {
    ui->setupUi(this);

    connect(StateManager::instance(), &StateManager::stateChanged, this, [=](StateManager::GlobalState state) {
        if (state != StateManager::Edit) this->hide();
    });
    connect(StateManager::instance(), &StateManager::spyModeChanged, this, &SpyWidget::updateSpyMode);
    updateSpyMode();
}

SpyWidget::~SpyWidget() {
    delete ui;
}

void SpyWidget::updateSpyMode() {
    switch (StateManager::spyMode()) {
        case StateManager::SpyDisabled:
            ui->disableSpyButton->setChecked(true);
            break;
        case StateManager::SpyTurnRestrictions:
            ui->spyOnTurnRestrictionButton->setChecked(true);
            break;
    }
}

void SpyWidget::on_disableSpyButton_toggled(bool checked) {
    if (checked) StateManager::setSpyMode(StateManager::SpyDisabled);
}

void SpyWidget::on_spyOnTurnRestrictionButton_toggled(bool checked) {
    if (checked) StateManager::setSpyMode(StateManager::SpyTurnRestrictions);
}
