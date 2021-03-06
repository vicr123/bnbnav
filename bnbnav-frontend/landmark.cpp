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
#include "landmark.h"

#include "datamanager.h"
#include "node.h"
#include <QJsonObject>
#include <QRectF>

struct LandmarkPrivate {
        Node* node;
        QString name;
        QString type;
};

Landmark::Landmark(QJsonObject definition, QObject* parent) :
    QObject(parent) {
    d = new LandmarkPrivate();
    redefine(definition);
}

Landmark::~Landmark() {
    delete d;
}

void Landmark::redefine(QJsonObject definition) {
    d->node = DataManager::nodes().value(definition.value("node").toString());
    d->name = definition.value("name").toString();
    if (definition.contains("landmarkType")) {
        d->type = definition.value("landmarkType").toString();
    } else {
        d->type = definition.value("type").toString();
    }
}

Node* Landmark::node() {
    return d->node;
}

QString Landmark::name() {
    return d->name;
}

QString Landmark::type() {
    return d->type;
}

QString Landmark::humanReadableType() {
    for (const QPair<QString, QString>& landmark : this->landmarks()) {
        if (landmark.first == d->type) return landmark.second;
    }
    for (const QPair<QString, QString>& landmark : this->labels()) {
        if (landmark.first == d->type) return landmark.second;
    }
    return tr("Unknown");
}

QRectF Landmark::hitbox() {
    QRectF landmarkPin;
    landmarkPin.setSize(QSizeF(8, 8));
    landmarkPin.moveCenter(QPointF(d->node->x(), d->node->z()));
    return landmarkPin;
}

QList<QPair<QString, QString>> Landmark::landmarks() {
    return {
        {"aircs",              tr("AirCS Station")         },
        {"airport",            tr("Airport")               },
        {"hospital",           tr("Hospital")              },
        {"squid-transit",      tr("Squid Transit Station") },
        {"parking",            tr("Parking Lot")           },
        {"walnut",             tr("Walnut")                },
        {"immigration-check",  tr("Immigration Checkpoint")},
        {"tourist-attraction", tr("Tourist Attraction")    },
        {"invisible",          tr("Invisible")             },
        {"generic-blue",       tr("Generic Blue")          },
        {"generic-green",      tr("Generic Green")         },
        {"generic-red",        tr("Generic Red")           },
        {"generic-gray",       tr("Generic Gray")          },
        {"city-hall",          tr("City Hall")             },
        {"cafe",               tr("Coffee Shop")           },
        {"shopping",           tr("Store")                 },
        {"restaurant",         tr("Restaurant")            },
        {"park",               tr("Park")                  },
        {"court",              tr("Courthouse")            },
        {"bank",               tr("Bank")                  },
        {"embassy",            tr("Embassy")               },
        {"postal-office",      tr("Post Office")           },
        {"hotel",              tr("Hotel")                 },
        {"frivoloco",          tr("FrivoloCo Chocolates")  },
        {"elc",                tr("ELC")                   },
        {"tesco",              tr("TESCO")                 },
    };
}

QList<QPair<QString, QString>> Landmark::labels() {
    return {
        {"label-city",    tr("City")   },
        {"label-country", tr("Country")}
    };
}
