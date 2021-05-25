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
#include "locationentrybox.h"

#include "statemanager.h"
#include <QRegularExpression>
#include "player.h"
#include "datamanager.h"

LocationEntryBox::LocationEntryBox(QWidget* parent) : QLineEdit(parent) {

}

QPoint LocationEntryBox::location() {
    //Try searching by coordinate
    QRegularExpression exp(QRegularExpression::anchoredPattern("(-?\\d+),(-?\\d+)"));
    QRegularExpressionMatch match = exp.match(this->text());
    if (match.hasMatch()) {
        return QPoint(match.capturedTexts().at(1).toInt(), match.capturedTexts().at(2).toInt());
    }

    QString playerTest = this->text();
    if (this->text() == tr("My Location")) {
        QString playerTest = StateManager::login();
        if (playerTest.isEmpty()) return QPoint();
    }

    for (Player* player : DataManager::players()) {
        if (player->name() == playerTest) {
            return QPoint(player->x(), player->z());
        }
    }
    return QPoint();
}
