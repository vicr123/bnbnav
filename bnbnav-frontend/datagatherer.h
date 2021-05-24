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
#ifndef DATAGATHERER_H
#define DATAGATHERER_H

#include <QObject>
#include <functional>

struct DataGathererPrivate;
class DataGatherer : public QObject {
        Q_OBJECT
    public:
        explicit DataGatherer(QString path, QObject* parent = nullptr);
        ~DataGatherer();

        static void submit(QString path, QJsonObject object, std::function<void(QByteArray, bool)> callback);

    signals:
        void ready(QByteArray data, bool error);

    private:
        DataGathererPrivate* d;

        void sendData(QByteArray data);
};

#endif // DATAGATHERER_H
