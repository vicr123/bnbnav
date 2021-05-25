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
#include "road.h"

#include <QJsonObject>

struct RoadPrivate {
    QString name;
    QString type;
};

Road::Road(QJsonObject definition, QObject* parent) : QObject(parent) {
    d = new RoadPrivate();
    d->name = definition.value("name").toString();

    if (definition.contains("roadType")) {
        d->type = definition.value("roadType").toString();
    } else {
        d->type = definition.value("type").toString();
    }
}

Road::~Road() {
    delete d;
}

QString Road::name() {
    return d->name;
}

QString Road::type() {
    return d->type;
}
