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
#include "edge.h"

#include "road.h"
#include "node.h"
#include "datamanager.h"
#include <QJsonObject>
#include <QLineF>
#include <QPolygonF>

struct EdgePrivate {
    Road* road;
    Node* from;
    Node* to;
};

Edge::Edge(QJsonObject definition, QObject* parent) : QObject(parent) {
    d = new EdgePrivate();
    redefine(definition);
}

Edge::~Edge() {
    delete d;
}

QString Edge::id() {
    return DataManager::edges().key(this);
}

bool Edge::isTemporary() {
    return this->id().startsWith("temp");
}

Node* Edge::from() {
    return d->from;
}

Node* Edge::to() {
    return d->to;
}

Road* Edge::road() {
    return d->road;
}

QLineF Edge::line() {
    return QLineF(d->from->x(), d->from->z(), d->to->x(), d->to->z());
}

QPolygonF Edge::hitbox(double width) {
    QLineF perpendicular = this->line();
    perpendicular.setLength(width);
    perpendicular = perpendicular.normalVector();

    QPolygonF poly;
    poly.append(perpendicular.pointAt(-1));
    poly.append(perpendicular.pointAt(1));

    perpendicular = this->line();
    perpendicular.setPoints(perpendicular.p2(), perpendicular.p1());
    perpendicular.setLength(width);
    perpendicular = perpendicular.normalVector();
    poly.append(perpendicular.pointAt(1));
    poly.append(perpendicular.pointAt(-1));

    return poly;
}

double Edge::length() {
    return this->line().length();
}

QPointF Edge::closestPointTo(QPointF point) {
    QLineF intersectionLine(point, point + QPointF(5, 0));
    intersectionLine.setAngle(line().normalVector().angle());

    QPointF intersectionPoint;
    intersectionLine.intersects(line(), &intersectionPoint);

    QLineF testLine(point, intersectionPoint);
    testLine.setLength(testLine.length() + 20);
    if (testLine.intersects(line(), nullptr) == QLineF::BoundedIntersection) {
        return intersectionPoint;
    } else {
        return QPointF();
    }
}

double Edge::distanceTo(QPointF point, bool* ok) {
    QPointF closestPoint = closestPointTo(point);
    if (closestPoint.isNull()) {
        if (ok) *ok = false;
        return 0;
    }
    *ok = true;
    return QLineF(closestPoint, point).length();
}

double Edge::averageY() {
    return (d->from->y() + d->to->y()) / 2.0;
}

void Edge::redefine(QJsonObject definition) {
    d->road = DataManager::roads().value(definition.value("road").toString());
    d->from = DataManager::nodes().value(definition.value("node1").toString());
    d->to = DataManager::nodes().value(definition.value("node2").toString());
}
