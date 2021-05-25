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

#include <QPen>
#include <QJsonObject>
#include "edge.h"

struct RoadPrivate {
    QString name;
    QString type;

    const QMap<QString, QColor> roadTypes = {
        {"local", QColor(Qt::black)},
        {"main", QColor(245, 179, 66)},
        {"highway", QColor(173, 25, 9)},
        {"motorway", QColor(105, 9, 173)}
    };
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

QPen Road::pen(Edge* edge) {
    QBrush col = d->roadTypes.value(d->type);
    double thickness = 1;
    if (d->type == "motorway") {
        QLineF perpendicular = edge->line();
        perpendicular.setLength(0.5);
        perpendicular = perpendicular.normalVector();

        QLinearGradient grad(perpendicular.pointAt(-1), perpendicular.pointAt(1));
        grad.setColorAt(0, QColor(100, 0, 0));
        grad.setColorAt(0.2, QColor(100, 0, 0));
        grad.setColorAt(0.2001, QColor(200, 200, 0));
        grad.setColorAt(0.7999, QColor(200, 200, 0));
        grad.setColorAt(0.8, QColor(100, 0, 0));
        grad.setColorAt(1, QColor(100, 0, 0));
        col = grad;
        thickness = 2;
    }
    return QPen(col, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}
