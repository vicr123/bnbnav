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
#include <QPoint>
#include <QLineF>
#include <QPen>
#include "statemanager.h"
#include "node.h"
#include "road.h"
#include "edge.h"
#include "player.h"
#include "landmark.h"
#include "datagatherer.h"

struct DataManagerPrivate {
    QMap<QString, Node*> nodes;
    QMap<QString, Edge*> edges;
    QMap<QString, Road*> roads;
    QMap<QString, Landmark*> landmarks;
    QMap<QString, Player*> players;
    QMap<QString, Player*> allPlayers;

    QMap<QString, Node*> temporaryNodes;
    QMap<QString, Edge*> temporaryEdges;

    uint currentTemporaryEdge = 0;
    uint currentTemporaryNode = 0;
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

QMap<QString, Landmark*> DataManager::landmarks() {
    return instance()->d->landmarks;
}

QMap<QString, Player*> DataManager::players() {
    return instance()->d->players;
}

Edge* DataManager::edgeForNodes(Node* from, Node* to) {
    for (Edge* edge : instance()->d->edges.values()) {
        if (edge->from() == from && edge->to() == to) return edge;
    }
    return nullptr;
}

QList<Edge*> DataManager::edgesFromNode(Node* from) {
    QList<Edge*> edges;
    for (Edge* edge : instance()->d->edges.values()) {
        if (edge->from() == from) edges.append(edge);
    }
    return edges;
}

QList<Edge*> DataManager::edgesToNode(Node* to) {
    QList<Edge*> edges;
    for (Edge* edge : instance()->d->edges.values()) {
        if (edge->to() == to) edges.append(edge);
    }
    return edges;
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

QList<Edge*> DataManager::shortestPath(QPoint from, QPoint to, QObject* fromEntity, QObject* toEntity) {
    Player* fromPlayer = qobject_cast<Player*>(fromEntity);

    //Find the closest edge for each point
    QMultiMap<double, Edge*> point1Candidates, point2Candidates;
    for (Edge* edge : edges().values()) {
        QPolygonF hitbox = edge->hitbox(edge->road()->pen(edge).widthF());
        bool ok;
        double distance = edge->distanceTo(from, &ok);
        if (!ok) {
            if (hitbox.containsPoint(from, Qt::OddEvenFill)) {
                distance = 0;
                ok = true;
            }
        }
        if (ok) point1Candidates.insert(distance, edge);

        distance = edge->distanceTo(to, &ok);
        if (!ok) {
            if (hitbox.containsPoint(to, Qt::OddEvenFill)) {
                distance = 0;
                ok = true;
            }
        }
        if (ok) point2Candidates.insert(distance, edge);
    }

    if (point1Candidates.isEmpty() || point2Candidates.isEmpty()) return QList<Edge*>();

    Node* fromNode = nextTemporaryNode(from.x(), point1Candidates.first()->averageY(), from.y());
    Node* toNode = nextTemporaryNode(to.x(), point2Candidates.first()->averageY(), to.y());

    //Construct edges and nodes to each candiate road
    if (fromPlayer && fromPlayer->snappedEdge()) {
        //Only consider the road that the player is snapped on
        nextTemporaryEdge(fromNode, fromPlayer->snappedEdge()->to(), fromPlayer->snappedEdge()->road());
    } else {
        double shortest1 = point1Candidates.firstKey();
        for (Edge* edge : point1Candidates.values(shortest1)) {
            QPointF closest = edge->closestPointTo(from);
            if (closest.isNull()) {
                nextTemporaryEdge(fromNode, edge->to(), edge->road());
            } else {
                Node* node = nextTemporaryNode(closest.x(), edge->averageY(), closest.y());
                nextTemporaryEdge(fromNode, node, edge->road());
                nextTemporaryEdge(node, edge->to(), edge->road());
            }
        }
    }

    double shortest2 = point2Candidates.firstKey();
    for (Edge* edge : point2Candidates.values(shortest2)) {
        QPointF closest = edge->closestPointTo(to);
        if (closest.isNull()) {
            nextTemporaryEdge(edge->from(), toNode, edge->road());
        } else {
            Node* node = nextTemporaryNode(closest.x(), edge->averageY(), closest.y());
            nextTemporaryEdge(edge->from(), node, edge->road());
            nextTemporaryEdge(node, toNode, edge->road());
        }
    }

    struct SearchNode {
        Node* node;
        double distance = 0;
        bool visited = false;
        Node* via = nullptr;
    };

    QList<SearchNode*> nodes;
    QMap<Node*, SearchNode*> searchNodes;

    for (Node* node : DataManager::nodes().values()) {
        SearchNode* n = new SearchNode();
        n->node = node;
        if (node == fromNode) {
            n->distance = 0;
            n->visited = true;
        }
        nodes.append(n);
        searchNodes.insert(n->node, n);
    }

    while (!nodes.isEmpty()) {
        //Sort the nodes by distance
        std::sort(nodes.begin(), nodes.end(), [ = ](SearchNode * first, SearchNode * second) {
            if (first->visited && second->visited) {
                return first->distance < second->distance;
            } else if (first->visited) {
                return true;
            } else if (second->visited) {
                return false;
            } else {
                return false;
            }
        });

        //Inspect the stations!
        SearchNode* top = nodes.takeFirst();
        if (!top->visited) {
            //No route found; bail here.

            qDeleteAll(searchNodes.values());
            return QList<Edge*>();
        }

        if (top->node == toNode) {
            QList<Edge*> edges;

            //We found it!
            do {
                edges.prepend(edgeForNodes(top->via, top->node));
                top = searchNodes.value(top->via);
            } while (top->via != nullptr);


            qDeleteAll(searchNodes.values());
            return edges;
        }

        for (Edge* edge : edgesFromNode(top->node)) {
            SearchNode* otherNode = searchNodes.value(edge->to());
            double distance = edge->routePenalty();

            if (otherNode->distance > top->distance + distance || !otherNode->visited) {
                otherNode->distance = top->distance + distance;
                otherNode->via = top->node;
                otherNode->visited = true;
            }
        }
    }

    qDeleteAll(searchNodes.values());
    return QList<Edge*>();
}

Node* DataManager::nextTemporaryNode(int x, int y, int z) {
    QJsonObject def = {
        {"x", x},
        {"y", y},
        {"z", z}
    };
    QString nodeName = QStringLiteral("temp%1").arg(++instance()->d->currentTemporaryNode);
    if (instance()->d->temporaryNodes.contains(nodeName)) {
        Node* node = instance()->d->temporaryNodes.value(nodeName);
        node->redefine(def);
        instance()->d->nodes.insert(nodeName, node);
        return node;
    } else {
        Node* node = new Node(def);
        instance()->d->temporaryNodes.insert(nodeName, node);
        instance()->d->nodes.insert(nodeName, node);
        return node;
    }
}

Edge* DataManager::nextTemporaryEdge(Node* from, Node* to, Road* road) {
    QJsonObject def = {
        {"road", instance()->d->roads.key(road)},
        {"node1", instance()->d->nodes.key(from)},
        {"node2", instance()->d->nodes.key(to)}
    };
    QString edgeName = QStringLiteral("temp%1").arg(++instance()->d->currentTemporaryNode);
    if (instance()->d->temporaryEdges.contains(edgeName)) {
        Edge* edge = instance()->d->temporaryEdges.value(edgeName);
        edge->redefine(def);
        instance()->d->edges.insert(edgeName, edge);
        return edge;
    } else {
        Edge* edge = new Edge(def);
        instance()->d->temporaryEdges.insert(edgeName, edge);
        instance()->d->edges.insert(edgeName, edge);
        return edge;
    }
}

void DataManager::resetTemporaries() {
    for (QString nodeId : instance()->d->temporaryNodes.keys()) {
        instance()->d->nodes.remove(nodeId);
    }
    for (QString edgeId : instance()->d->temporaryEdges.keys()) {
        instance()->d->edges.remove(edgeId);
    }
    instance()->d->currentTemporaryEdge = 0;
    instance()->d->currentTemporaryNode = 0;
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

        QJsonObject landmarks = root.value("landmarks").toObject();
        for (QString landmarkId : landmarks.keys()) {
            d->landmarks.insert(landmarkId, new Landmark(landmarks.value(landmarkId).toObject()));
        }

        emit ready();
    });


    QWebSocket* ws = new QWebSocket("localhost:4000");
    ws->open(QUrl(WS_URL));
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
        } else if (type == "newLandmark") {
            d->landmarks.insert(id, new Landmark(object));
            emit newLandmark();
        } else if (type == "edgeRemoved") {
            d->edges.remove(id);
            emit removedEdge();
        } else if (type == "roadRemoved") {
            d->roads.remove(id);
            emit removedRoad();
        } else if (type == "nodeRemoved") {
            d->nodes.remove(id);
            emit removedNode();
        } else if (type == "landmarkRemoved") {
            d->landmarks.remove(id);
            emit removedLandmark();
        } else if (type == "nodeUpdated") {
            d->nodes.value(id)->redefine(object);
            emit updatedNode();
        } else if (type == "roadUpdated") {
            d->roads.value(id)->redefine(object);
            emit updatedRoad();
        } else if (type == "playerMove") {
            if (!d->players.contains(id)) {
                if (d->allPlayers.contains(id)) {
                    d->players.insert(id, d->allPlayers.value(id));
                } else {
                    Player* p = new Player(id);
                    d->allPlayers.insert(id, p);
                    d->players.insert(id, p);
                }
            }
            d->players.value(id)->update(object);
            emit playerUpdate(id);
        } else if (type == "playerGone") {
            d->players.remove(id);
            emit removedPlayer();
        }
    });
}
