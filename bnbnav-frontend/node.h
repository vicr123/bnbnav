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
#ifndef NODE_H
#define NODE_H

#include <QObject>

struct NodePrivate;
class Node : public QObject {
        Q_OBJECT
    public:
        Node(QJsonObject definition, QObject* parent = nullptr);
        ~Node();

        int x();
        int y();
        int z();
        QRectF nodeRect(double scale);

        void setX(int x);
        void setY(int y);
        void setZ(int z);

        void redefine(QJsonObject definition);
        void submitUpdate(int x, int y, int z, std::function<void(bool)> callback);

    signals:

    private:
        NodePrivate* d;
};

#endif // NODE_H
