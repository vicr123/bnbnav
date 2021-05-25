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

#include <QSettings>

struct StateManagerPrivate {
    StateManager::GlobalState state = StateManager::Browse;
    QString login;
    QList<Edge*> currentRoute;
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

void StateManager::setLogin(QString login) {
    QSettings settings;
    settings.setValue("login/username", login);
    instance()->d->login = login;
    emit instance()->loginChanged(login);
}

QString StateManager::login() {
    return instance()->d->login;
}

void StateManager::setCurrentRoute(QList<Edge*> edges) {
    instance()->d->currentRoute = edges;
}

QList<Edge*> StateManager::currentRoute() {
    return instance()->d->currentRoute;
}

StateManager::StateManager(QObject* parent) : QObject(parent) {
    d = new StateManagerPrivate();

    QSettings settings;
    d->login = settings.value("login/username").toString();
}
