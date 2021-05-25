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
#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

class Node;
class Edge;
class Road;
class Player;

struct DataManagerPrivate;
class DataManager : public QObject {
        Q_OBJECT
    public:
        static DataManager* instance();

        static QMap<QString, Node*> nodes();
        static QMap<QString, Edge*> edges();
        static QMap<QString, Road*> roads();
        static QList<Player*> players();

        static Edge* edgeForNodes(Node* from, Node* to);
        static QSet<QString> roadsConnectedToNode(Node* node);

    signals:
        void ready();
        void newNode();
        void newRoad();
        void newEdge();
        void removedNode();
        void removedRoad();
        void removedEdge();
        void updatedNode();
        void updatedRoad();
        void updatedEdge();
        void loadError();
        void playerUpdate();

    private:
        explicit DataManager(QObject* parent = nullptr);
        DataManagerPrivate* d;
};

#endif // DATAMANAGER_H
