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
#include "road.h"

#include "edge.h"
#include "statemanager.h"
#include <QJsonObject>
#include <QPen>

struct RoadPrivate {
        QString name;
        QString type;

        const QMap<QString, QColor> roadDayColours = {
            {"local",     QColor(Qt::black)  },
            {"main",      QColor(245,          179, 66)},
            {"highway",        QColor(173,                           25, 9)},
            {"motorway",QColor(105,9, 173)},
            {"footpath",           QColor(150,                     150, 150)},
            {"waterway",          QColor(0,                                         110, 150)},
            {"private",  QColor(100, 100, 100)},
            {"roundabout",       QColor(Qt::black)                         },
            {"duong-warp",QColor(Qt::transparent)},
        };
        const QMap<QString, QColor> roadNightColours = {
            {"local",     QColor(QColor(0,     80, 140))},
            {"main",      QColor(150,                         100, 50)},
            {"highway",QColor(115,15, 0)},
            {"motorway",           QColor(105,                      9, 173)},
            {"footpath",         QColor(150,                                       150, 150)},
            {"waterway",  QColor(0, 110, 150)},
            {"private",       QColor(100,                          100, 100)},
            {"roundabout",QColor(QColor(0,80, 140))},
            {"duong-warp",           QColor(Qt::transparent)                    },
        };

        const QMap<QString, double> roadPenalties = {
            {"local",      1  },
            {"main",       0.8},
            {"highway",    0.7},
            {"motorway",   0.6},
            {"footpath",   1.5},
            {"waterway",   1  },
            {"private",    2  },
            {"roundabout", 1  },
            {"duong-warp", 0  },
        };
};

Road::Road(QJsonObject definition, QObject* parent) :
    QObject(parent) {
    d = new RoadPrivate();
    redefine(definition);
}

Road::~Road() {
    delete d;
}

QList<QPair<QString, QString>> Road::roadTypes() {
    return {
        {"local",      tr("Local Road")      },
        {"main",       tr("Main Road")       },
        {"highway",    tr("Highway")         },
        {"motorway",   tr("Motorway")        },
        {"footpath",   tr("Footpath")        },
        {"waterway",   tr("Waterway")        },
        {"private",    tr("Private Use Road")},
        {"roundabout", tr("Roundabout")      },
        {"duong-warp", tr("Duong Warp")      },
    };
}

QString Road::name() {
    return d->name;
}

QString Road::type() {
    return d->type;
}

QString Road::humanReadableType() {
    for (QPair<QString, QString> type : roadTypes()) {
        if (type.first == d->type) return type.second;
    }
    return tr("Unknown");
}

QPen Road::pen(Edge* edge) {
    QBrush col = StateManager::nightMode() ? d->roadNightColours.value(d->type) : d->roadDayColours.value(d->type);
    double thickness = 5;
    if (d->type == "motorway") {
        QLineF perpendicular = edge->line();
        perpendicular.setLength(5);
        perpendicular = perpendicular.normalVector();

        QColor outer = StateManager::nightMode() ? QColor(50, 0, 0) : QColor(100, 0, 0);
        QColor inner = StateManager::nightMode() ? QColor(100, 100, 0) : QColor(200, 200, 0);

        QLinearGradient grad(perpendicular.pointAt(-1), perpendicular.pointAt(1));
        grad.setColorAt(0, outer);
        grad.setColorAt(0.3, outer);
        grad.setColorAt(0.3001, inner);
        grad.setColorAt(0.6999, inner);
        grad.setColorAt(0.7, outer);
        grad.setColorAt(1, outer);
        col = grad;
        thickness = 10;
    }
    return QPen(col, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}

double Road::penalty() {
    return d->roadPenalties.value(d->type, 1);
}

void Road::redefine(QJsonObject definition) {
    d->name = definition.value("name").toString();

    if (definition.contains("roadType")) {
        d->type = definition.value("roadType").toString();
    } else {
        d->type = definition.value("type").toString();
    }
}
