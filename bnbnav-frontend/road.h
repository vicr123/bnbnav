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
#ifndef ROAD_H
#define ROAD_H

#include <QObject>

class Edge;
struct RoadPrivate;
class Road : public QObject {
        Q_OBJECT
    public:
        explicit Road(QJsonObject definition, QObject* parent = nullptr);
        ~Road();

        static QList<QPair<QString, QString>> roadTypes();

        QString name();
        QString type();
        QPen pen(Edge* edge);

        void redefine(QJsonObject definition);

    signals:

    private:
        RoadPrivate* d;
};

#endif // ROAD_H
