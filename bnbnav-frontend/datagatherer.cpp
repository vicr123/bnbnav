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
#include "datagatherer.h"

#include <QMap>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

#include <QNetworkAccessManager>
#include <QNetworkReply>

struct DataGathererPrivate {
    static QMap<QString, QByteArray> cached;

    QString path;
};

QMap<QString, QByteArray> DataGathererPrivate::cached = QMap<QString, QByteArray>();

DataGatherer::DataGatherer(QString path, QObject* parent) : QObject(parent) {
    d = new DataGathererPrivate();
    d->path = path;

    if (DataGathererPrivate::cached.contains(path)) {
        QTimer::singleShot(0, [ = ] {
            sendData(DataGathererPrivate::cached.value(path));
        });
        return;
    }

    QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
#ifdef Q_OS_WASM
    QNetworkReply* reply = mgr->get(QNetworkRequest(QUrl(QStringLiteral("/api/%1").arg(path))));
#else
    QNetworkReply* reply = mgr->get(QNetworkRequest(QUrl(QStringLiteral("https://%1/api/%2").arg(BASE_URL, path))));
#endif
    connect(reply, &QNetworkReply::finished, [ = ] {
        if (reply->error() != QNetworkReply::NoError) {
            emit ready(QByteArray(), true);
            return;
        }

        sendData(reply->readAll());
    });
}

DataGatherer::~DataGatherer() {
    delete d;
}

void DataGatherer::submit(QString path, QJsonObject object, std::function<void (QByteArray, bool)> callback) {
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
#ifdef Q_OS_WASM
    req.setUrl(QUrl(QStringLiteral("/api/%1").arg(path)));
#else
    req.setUrl(QUrl(QStringLiteral("https://%1/api/%2").arg(BASE_URL, path)));
#endif
    QNetworkReply* reply = mgr->post(req, QJsonDocument(object).toJson());
    connect(reply, &QNetworkReply::finished, [ = ] {
        if (reply->error() != QNetworkReply::NoError) {
            callback(QByteArray(), true);
            return;
        }

        callback(reply->readAll(), false);
    });
}

void DataGatherer::del(QString path, std::function<void (bool)> callback) {
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    QNetworkRequest req;
#ifdef Q_OS_WASM
    req.setUrl(QUrl(QStringLiteral("/api/%1").arg(path)));
#else
    req.setUrl(QUrl(QStringLiteral("https://%1/api/%2").arg(BASE_URL, path)));
#endif
    QNetworkReply* reply = mgr->deleteResource(req);
    connect(reply, &QNetworkReply::finished, [ = ] {
        if (reply->error() != QNetworkReply::NoError) {
            callback(true);
            return;
        }

        callback(false);
    });
}

void DataGatherer::sendData(QByteArray data) {
    emit ready(data, false);
    DataGathererPrivate::cached.insert(d->path, data);
    this->deleteLater();
}
