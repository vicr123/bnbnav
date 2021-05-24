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
#include "statemanager.h"

struct StateManagerPrivate {
    StateManager::GlobalState state = StateManager::Browse;
};

StateManager* StateManager::instance() {
    static StateManager* instance = new StateManager();
    return instance;
}

StateManager::GlobalState StateManager::currentState() {
    return instance()->d->state;
}

void StateManager::setCurrentState(GlobalState state) {
    instance()->d->state = state;
    emit instance()->stateChanged(state);
}

StateManager::StateManager(QObject* parent) : QObject(parent) {
    d = new StateManagerPrivate();
}
