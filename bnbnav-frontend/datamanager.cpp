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
#include "datamanager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QWebSocket>
#include "node.h"
#include "road.h"
#include "edge.h"
#include "datagatherer.h"

struct DataManagerPrivate {
    QMap<QString, Node*> nodes;
    QMap<QString, Edge*> edges;
    QMap<QString, Road*> roads;
};

DataManager* DataManager::instance() {
    static DataManager* instance = new DataManager();
    return instance;
}

QMap<QString, Node*> DataManager::nodes() {
    return instance()->d->nodes;
}

QMap<QString, Edge*> DataManager::edges() {
    return instance()->d->edges;
}

QMap<QString, Road*> DataManager::roads() {
    return instance()->d->roads;
}

Edge* DataManager::edgeForNodes(Node* from, Node* to) {
    for (Edge* edge : instance()->d->edges.values()) {
        if (edge->from() == from && edge->to() == to) return edge;
    }
    return nullptr;
}

QSet<QString> DataManager::roadsConnectedToNode(Node* node) {
    QSet<QString> roads;
    for (Edge* edge : instance()->d->edges.values()) {
        if (edge->from() == node || edge->to() == node) {
            roads.insert(instance()->d->roads.key(edge->road()));
        }
    }
    return roads;
}

DataManager::DataManager(QObject* parent) : QObject(parent) {
    d = new DataManagerPrivate();

    connect(new DataGatherer("/data"), &DataGatherer::ready, this, [ = ](QByteArray data, bool error) {
        if (error) {
            emit loadError();
            return;
        }

        QJsonObject root = QJsonDocument::fromJson(data).object();

        QJsonObject nodes = root.value("nodes").toObject();
        for (QString nodeId : nodes.keys()) {
            d->nodes.insert(nodeId, new Node(nodes.value(nodeId).toObject()));
        }

        QJsonObject roads = root.value("roads").toObject();
        for (QString roadId : roads.keys()) {
            d->roads.insert(roadId, new Road(roads.value(roadId).toObject()));
        }

        QJsonObject edges = root.value("edges").toObject();
        for (QString edgeId : edges.keys()) {
            d->edges.insert(edgeId, new Edge(edges.value(edgeId).toObject()));
        }

        emit ready();
    });


    QWebSocket* ws = new QWebSocket("localhost:4000");
    ws->open(QUrl("ws://localhost:4000/ws"));
    connect(ws, &QWebSocket::aboutToClose, this, [ = ] {
        emit loadError();
    });
    connect(ws, &QWebSocket::textMessageReceived, this, [ = ](QString message) {
        QJsonObject object = QJsonDocument::fromJson(message.toUtf8()).object();
        QString type = object.value("type").toString();
        QString id = object.value("id").toString();

        if (type == "newNode") {
            d->nodes.insert(id, new Node(object));
            emit newNode();
        } else if (type == "newRoad") {
            d->roads.insert(id, new Road(object));
            emit newRoad();
        } else if (type == "newEdge") {
            d->edges.insert(id, new Edge(object));
            emit newEdge();
        }
    });
}
