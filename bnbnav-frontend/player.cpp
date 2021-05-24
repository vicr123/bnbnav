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

#include <QJsonObject>

struct PlayerPrivate {
    QString name;
    double x, y, z;
};

Player::Player(QString name, QObject* parent) : QObject(parent) {
    d = new PlayerPrivate();
    d->name = name;
}

Player::~Player() {
    delete d;
}

void Player::update(QJsonObject object) {
    d->x = object.value("x").toDouble();
    d->y = object.value("y").toDouble();
    d->z = object.value("z").toDouble();
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
