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
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>

struct StateManagerPrivate;
class StateManager : public QObject {
        Q_OBJECT
    public:
        enum GlobalState {
            Browse,
            Edit,
            Route
        };

        static StateManager* instance();

        static GlobalState currentState();
        static void setCurrentState(GlobalState state);

        static void setLogin(QString login);
        static QString login();

    signals:
        void stateChanged(GlobalState state);
        void loginChanged(QString login);

    private:
        explicit StateManager(QObject* parent = nullptr);
        StateManagerPrivate* d;
};

#endif // STATEMANAGER_H
