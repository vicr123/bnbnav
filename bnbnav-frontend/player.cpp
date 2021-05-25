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
#include <QJsonObject>

struct PlayerPrivate {
    QString name;
    double x, y, z;

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
        if (d->posHistory.count() <= 10) break;;
        if (d->posHistory.at(i).first < QDateTime::currentMSecsSinceEpoch() - 100) {
            d->posHistory.removeAt(i);
            i--;
        }
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

double Player::angle() {
    QLineF line(d->posHistory.first().second, d->posHistory.last().second);
    return line.angle();
}
