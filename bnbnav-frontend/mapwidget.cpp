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
#include "mapwidget.h"

#include <QPainter>
#include <QMap>
#include <QMenu>
#include <QMouseEvent>
#include <QPainterPath>
#include <datagatherer.h>
#include <QMessageBox>
#include "nodeconnectdialog.h"
#include "datamanager.h"
#include "edge.h"
#include "node.h"
#include "road.h"
#include "player.h"
#include "statemanager.h"

struct MapWidgetPrivate {
    QPointF origin;
    double scale = 10;

    bool dragging = false;
    QPoint dragStart;
    QPoint dragInitial;

    QList<QObject*> hoverTargets;
    Node* firstNode = nullptr;

    const QMap<QString, QColor> roadTypes = {
        {"local", QColor(Qt::black)},
        {"main", QColor(245, 179, 66)},
        {"highway", QColor(173, 25, 9)},
        {"motorway", QColor(105, 9, 173)}
    };
};

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
    d = new MapWidgetPrivate();
    this->setMouseTracking(true);

    connect(DataManager::instance(), &DataManager::ready, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::newRoad, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::newNode, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::newEdge, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::removedEdge, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::removedNode, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::removedRoad, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::playerUpdate, this, [ = ] {
        update();
    });
    connect(StateManager::instance(), &StateManager::stateChanged, this, [ = ] {
        update();
    });
}

QPointF MapWidget::toMapCoordinates(QPointF widget) {
    return currentTransform().inverted().map(widget);
}

QTransform MapWidget::currentTransform() {
    QTransform transform;
    transform.scale(d->scale, d->scale);
    transform.translate(d->origin.x() / d->scale, d->origin.y() / d->scale);
    return transform;
}

void MapWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setTransform(currentTransform());
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font = painter.font();
    font.setPixelSize(5);
    painter.setFont(font);

    for (Edge* edge : DataManager::edges().values()) {
        //Draw the edge
        Node* from = edge->from();
        Node* to = edge->to();
        painter.setPen(d->roadTypes.value(edge->road()->type()));

        if (StateManager::currentState() == StateManager::Edit && d->hoverTargets.contains(edge)) {
            painter.setPen(Qt::blue);
        }

        painter.drawLine(from->x(), from->z(), to->x(), to->z());
    }

    //Draw players
    for (Player* player : DataManager::players()) {
        painter.setPen(Qt::white);
        painter.setBrush(Qt::red);
        QRectF circle;
        circle.setSize(QSizeF(5, 5));
        circle.moveCenter(QPointF(player->x(), player->z()));
        painter.drawEllipse(circle);

        painter.setPen(Qt::black);

        QRectF text;
        text.setHeight(painter.fontMetrics().height());
        text.setWidth(painter.fontMetrics().horizontalAdvance(player->name()));
        text.moveCenter(circle.center());
        text.moveTop(circle.bottom() + 1);
        painter.drawText(text, player->name());
    }

    if (StateManager::currentState() == StateManager::Edit) {
        for (Node* node : DataManager::nodes().values()) {
            //Draw the node
            painter.setPen(QPen(Qt::black, 0.1));
            painter.setBrush(Qt::white);
            if (d->firstNode == node) painter.setPen(QPen(Qt::red, 0.1));
            if (d->hoverTargets.contains(node)) painter.setPen(QPen(Qt::blue, 0.1));
            painter.drawRect(node->nodeRect());
        }
    }
}

void MapWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        d->dragging = true;
        d->dragStart = event->globalPos();
        d->dragInitial = event->globalPos();
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (d->dragging) {
            d->dragging = false;
            if ((d->dragInitial - d->dragStart).manhattanLength() < 5) {
                //Treat this as a click!
                if (StateManager::currentState() == StateManager::Edit && !d->hoverTargets.isEmpty()) {
                    //TODO: more than one hover target
                    Node* hoverNode = qobject_cast<Node*>(d->hoverTargets.first());
                    Edge* hoverEdge = qobject_cast<Edge*>(d->hoverTargets.first());

                    if (hoverNode) {
                        if (d->firstNode) {
                            Node* first = d->firstNode;
                            Node* second = hoverNode;
                            d->firstNode = nullptr;

                            if (first == second) return;
                            if (DataManager::edgeForNodes(first, second)) return;

                            //Connect these nodes!
                            NodeConnectDialog dialog(first, second);
                            dialog.exec();
                        } else {
                            d->firstNode = hoverNode;
                        }
                    } else if (hoverEdge) {
                        //TODO: Edit edge
                    }
                }
            }
        }
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent* event) {
    if (d->dragging) {
        d->origin += event->globalPos() - d->dragStart;
        d->dragStart = event->globalPos();
    } else {
        //Figure out what we're on
        if (StateManager::currentState() == StateManager::Edit) {
            d->hoverTargets.clear();
            QPointF mapPos = toMapCoordinates(event->pos());
            for (Node* node : DataManager::nodes().values()) {
                if (node->nodeRect().contains(mapPos)) d->hoverTargets.append(node);
            }

            for (Edge* edge : DataManager::edges().values()) {
                QPainterPath path;
                path.moveTo(edge->from()->x(), edge->from()->y());
                path.lineTo(edge->to()->x(), edge->to()->y());
                path.lineTo(edge->to()->x(), edge->to()->y());

                QPainterPathStroker stroker(QPen(Qt::black, 1));
                if (stroker.createStroke(path).toFillPolygon().contains(mapPos)) d->hoverTargets.append(edge);
            }
        }
    }

    emit pan(-d->origin.x() / d->scale, -d->origin.y() / d->scale);
    this->update();
}

void MapWidget::wheelEvent(QWheelEvent* event) {
    double factor = 1.0 + 10 * (event->angleDelta().y() / 12000.0);
    d->scale *= factor;
    d->origin *= factor;
    this->update();
}

void MapWidget::contextMenuEvent(QContextMenuEvent* event) {
    QMenu* menu = new QMenu();

    if (StateManager::currentState() == StateManager::Edit && !d->hoverTargets.isEmpty()) {
        Node* hoverNode = qobject_cast<Node*>(d->hoverTargets.first());
        Edge* hoverEdge = qobject_cast<Edge*>(d->hoverTargets.first());

        if (hoverNode) {
            menu->addAction(tr("Delete Node"), [ = ] {
                DataGatherer::del(QStringLiteral("/nodes/%1").arg(DataManager::nodes().key(hoverNode)), [ = ](bool error) {
                    if (error) {
                        QMessageBox::warning(this, tr("Could not delete node"), tr("Could not delete the node."));
                    }
                });
            });
        }
    }

    menu->popup(event->globalPos());
    connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
}
