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
#include "landmark.h"

#include "datamanager.h"
#include <QJsonObject>

struct LandmarkPrivate {
    Node* node;
    QString name;
    QString type;
};

Landmark::Landmark(QJsonObject definition, QObject* parent) : QObject(parent) {
    d = new LandmarkPrivate();
    redefine(definition);
}

Landmark::~Landmark() {
    delete d;
}

void Landmark::redefine(QJsonObject definition) {
    d->node = DataManager::nodes().value(definition.value("node").toString());
    d->name = definition.value("name").toString();
    if (definition.contains("roadType")) {
        d->type = definition.value("roadType").toString();
    } else {
        d->type = definition.value("type").toString();
    }
}

Node* Landmark::node() {
    return d->node;
}

QString Landmark::name() {
    return d->name;
}

QString Landmark::type() {
    return d->type;
}

QList<QPair<QString, QString>> Landmark::landmarks() {
    return {
        {"aircs", tr("AirCS Station")},
        {"airport", tr("Airport")},
        {"hospital", tr("Hospital")},
        {"squid-transit", tr("Squid Transit Station")},
        {"parking", tr("Parking Lot")},
        {"walnut", tr("Walnut")},
        {"immigration-check", tr("Immigration Border")},
        {"tourist-attraction", tr("Tourist Attraction")},
        {"invisible", tr("Invisible")},
        {"generic-blue", tr("Generic Blue")},
        {"generic-green", tr("Generic Green")},
        {"generic-red", tr("Generic Red")},
        {"generic-gray", tr("Generic Gray")}
    };
}
