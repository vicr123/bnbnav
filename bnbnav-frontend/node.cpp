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
#include "node.h"

#include "datagatherer.h"
#include "datamanager.h"
#include <QJsonObject>
#include <QRect>

struct NodePrivate {
        int x, y, z;
        QMap<QString, QJsonObject> annotations;
};

Node::Node(QJsonObject definition, QObject* parent) :
    QObject(parent) {
    d = new NodePrivate();
    redefine(definition);
}

Node::~Node() {
    delete d;
}

QString Node::id() {
    return DataManager::nodes().key(this);
}

bool Node::isTemporary() {
    return this->id().startsWith("temp");
}

int Node::x() {
    return d->x;
}

int Node::y() {
    return d->y;
}

int Node::z() {
    return d->z;
}

QRectF Node::nodeRect(double scale) {
    QRectF rect;
    rect.setSize(QSizeF(15, 15) / scale);
    rect.moveCenter(QPointF(d->x, d->z));
    return rect;
}

QJsonObject Node::annotation(QString name) {
    return d->annotations.value(name);
}

void Node::removeAnnotation(QString name) {
    d->annotations.remove(name);
}

void Node::updateAnnotation(QString name, QJsonObject annotation) {
    d->annotations.insert(name, annotation);
}

void Node::setX(int x) {
    d->x = x;
}

void Node::setZ(int z) {
    d->z = z;
}

void Node::redefine(QJsonObject definition) {
    d->x = definition.value("x").toInt();
    d->y = definition.value("y").toInt();
    d->z = definition.value("z").toInt();
}

void Node::defineAnnotations(QJsonObject annotations) {
    d->annotations.clear();
    for (auto key : annotations.keys()) {
        d->annotations.insert(key, annotations.value(key).toObject());
    }
}

void Node::submitUpdate(int x, int y, int z, std::function<void(bool)> callback) {
    NodePrivate* oldPrivate = d;
    d = new NodePrivate();
    d->x = x;
    d->y = y;
    d->z = z;
    d->annotations = oldPrivate->annotations;

    DataGatherer::submit(QStringLiteral("/nodes/%1").arg(DataManager::nodes().key(this)), {
                                                                                              {"x", x},
                                                                                              {"y", y},
                                                                                              {"z", z}
    },
        [=](QByteArray data, bool error) {
        if (error) {
            delete d;
            d = oldPrivate;
            callback(true);
            return;
        }

        delete oldPrivate;
        callback(false);
    });
}

void Node::submitAnnotationUpdate(QString name, QJsonObject annotation, std::function<void(bool)> callback) {
    DataGatherer::submit(QStringLiteral("/nodes/%1/annotations/%2").arg(DataManager::nodes().key(this)).arg(name), annotation,
        [=](QByteArray data, bool error) {
        if (error) {
            callback(true);
            return;
        }

        callback(false);
    });
}

void Node::submitAnnotationDelete(QString name, std::function<void(bool)> callback) {
    DataGatherer::del(QStringLiteral("/nodes/%1/annotations/%2").arg(DataManager::nodes().key(this)).arg(name), callback);
}
