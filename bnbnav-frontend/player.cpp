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
#include "player.h"

#include <QPointF>
#include <QLineF>
#include <QPen>
#include <QJsonObject>
#include "edge.h"
#include "road.h"
#include "datamanager.h"

struct PlayerPrivate {
    QString name;
    double x, y, z;

    Edge* snappedEdge = nullptr;
    QList<QPair<qint64, QPointF>> posHistory;
};

Player::Player(QString name, QObject* parent) : QObject(parent) {
    d = new PlayerPrivate();
    d->name = name;
}

Player::~Player() {
    delete d;
}

void Player::update(QJsonObject object) {
    double newX = object.value("x").toDouble();
    double newY = object.value("y").toDouble();
    double newZ = object.value("z").toDouble();

    if (d->x == newX && d->y == newY && d->z == newZ) return;

    d->x = newX;
    d->y = newY;
    d->z = newZ;

    d->posHistory.append(QPair<qint64, QPointF>(QDateTime::currentMSecsSinceEpoch(), QPointF(d->x, d->z)));
    for (int i = 0; i < d->posHistory.count(); i++) {
        if (d->posHistory.count() <= 10) break;
        if (d->posHistory.at(i).first < QDateTime::currentMSecsSinceEpoch() - 100) {
            d->posHistory.removeAt(i);
            i--;
        }
    }

    QList<Edge*> candidateEdges;
    for (Edge* edge : DataManager::edges().values()) {
        QPolygonF hitbox = edge->hitbox(edge->road()->pen(edge).widthF() * 2);
        if (hitbox.containsPoint(QPointF(d->x, d->z), Qt::OddEvenFill)) {
            double angle = edge->line().angleTo(this->velocity());
            if (angle > 180) angle = -360 + angle;
            if (qAbs(angle) < 45) {
                //TODO: Prepend this edge if it's part of the current route
                candidateEdges.append(edge);
            }
        }
    }

    if (!candidateEdges.contains(d->snappedEdge)) {
        d->snappedEdge = candidateEdges.isEmpty() ? nullptr : candidateEdges.first();
    }
}

QString Player::name() {
    return d->name;
}

double Player::x() {
    return d->x;
}

double Player::y() {
    return d->y;
}

double Player::z() {
    return d->z;
}

Edge* Player::snappedEdge() {
    return d->snappedEdge;
}

QPointF Player::markerCoordinates() {
    if (d->snappedEdge) {
        //Find the intersection point
        QLineF playerLine(d->x, d->z, d->x + 1, d->z);
        playerLine.setAngle(d->snappedEdge->line().normalVector().angle());

        QPointF intersectionPoint;
        playerLine.intersects(d->snappedEdge->line(), &intersectionPoint);
        return intersectionPoint;
    } else {
        return QPointF(d->x, d->z);
    }
}

double Player::markerAngle() {
    if (d->snappedEdge) {
        return d->snappedEdge->line().angle();
    } else {
        return velocity().angle();
    }
}

QLineF Player::velocity() {
    return QLineF(d->posHistory.first().second, d->posHistory.last().second);
}

double Player::angle() {
    return velocity().angle();
}
