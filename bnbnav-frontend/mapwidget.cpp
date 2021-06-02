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
#include <QSvgRenderer>
#include <QPicture>
#include <QBuffer>
#include <QVariantAnimation>
#include "nodeconnectdialog.h"
#include "newlandmarkdialog.h"
#include "datamanager.h"
#include "edge.h"
#include "node.h"
#include "road.h"
#include "landmark.h"
#include "player.h"
#include "statemanager.h"
#include "newroaddialog.h"

struct MapWidgetPrivate {
    QPointF origin;
    double scale = 10;

    bool dragging = false;
    QPoint dragStart;
    QPoint dragInitial;

    QList<QObject*> hoverTargets;
    Node* firstNode = nullptr;

    Node* dragNode = nullptr;
    QPoint initialNodeCoordinates;
    QPoint dragNodeCoordinates;

    QPicture baseMap;
};

MapWidget::MapWidget(QWidget* parent) : QWidget(parent) {
    d = new MapWidgetPrivate();
    this->setMouseTracking(true);

    connect(DataManager::instance(), &DataManager::ready, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::newRoad, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::newNode, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::newEdge, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::newLandmark, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::removedEdge, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::removedNode, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::removedRoad, this, [ = ] {
        update();
    });
    connect(DataManager::instance(), &DataManager::removedLandmark, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::updatedEdge, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::updatedNode, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::updatedRoad, this, [ = ] {
        updateBaseMap();
    });
    connect(DataManager::instance(), &DataManager::playerUpdate, this, [ = ](QString player) {
        update();

        if (player == StateManager::login()) followPlayer();
    });
    connect(StateManager::instance(), &StateManager::stateChanged, this, [ = ] {
        update();
    });
    connect(StateManager::instance(), &StateManager::followMeChanged, this, [ = ] {
        followPlayer();
    });


    connect(StateManager::instance(), &StateManager::nightModeChanged, this, [ = ] {
        QPalette pal;
        if (StateManager::nightMode()) {
            pal.setColor(QPalette::Window, QColor(0, 30, 50));
            pal.setColor(QPalette::WindowText, Qt::white);
        } else {
            pal.setColor(QPalette::Window, Qt::white);
            pal.setColor(QPalette::WindowText, Qt::black);
        }
        this->setPalette(pal);
        updateBaseMap();
    });

    updateBaseMap();
}

void MapWidget::focusMap(QPoint point) {
    if (StateManager::followMe()) StateManager::setFollowMe(false);

    QVariantAnimation* anim = new QVariantAnimation();
    anim->setStartValue(d->origin);
    anim->setEndValue(QPointF(-point * d->scale + QPoint(this->width() / 2, this->height() / 2)));
    connect(anim, &QVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->origin = value.toPointF();
        this->update();
    });
    connect(anim, &QVariantAnimation::finished, anim, &QVariantAnimation::deleteLater);
    anim->setEasingCurve(QEasingCurve::InOutCubic);
    anim->setDuration(500);
    anim->start();
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

void MapWidget::doClick() {
    if (d->hoverTargets.isEmpty()) {
        StateManager::setSelectedLandmark(nullptr);
    } else {
        //TODO: more than one hover target
        Node* hoverNode = qobject_cast<Node*>(d->hoverTargets.first());
        Edge* hoverEdge = qobject_cast<Edge*>(d->hoverTargets.first());
        Landmark* hoverLandmark = qobject_cast<Landmark*>(d->hoverTargets.first());

        if (StateManager::currentState() == StateManager::Edit) {
            if (hoverNode) {
                if (d->firstNode) {
                    Node* first = d->firstNode;
                    Node* second = hoverNode;
                    d->firstNode = nullptr;

                    if (first == second) return;
                    if (DataManager::edgeForNodes(first, second)) return;

                    //Connect these nodes!
                    NodeConnectDialog* dialog = new NodeConnectDialog(first, second);
                    dialog->setWindowModality(Qt::ApplicationModal);
                    connect(dialog, &NodeConnectDialog::finished, dialog, &NodeConnectDialog::deleteLater);
                    dialog->open();
                } else {
                    d->firstNode = hoverNode;
                }
            } else if (hoverEdge) {
                //TODO: Edit edge
            }
        }

        if (hoverLandmark) {
            StateManager::setSelectedLandmark(hoverLandmark);
            emit landmarkClicked(hoverLandmark);
        } else {
            StateManager::setSelectedLandmark(nullptr);
        }
    }
}

void MapWidget::followPlayer() {
    if (!StateManager::followMe()) return;

    Player* player = StateManager::loggedInPlayer();
    QPointF playerCoords(player->x(), player->z());

    d->origin = -playerCoords * d->scale + QPointF(this->width() / 2, this->height() / 2);
}

void MapWidget::updateBaseMap() {
    QPainter painter(&d->baseMap);

    QSet<Edge*> drawnEdges;
    QList<Edge*> edges = DataManager::edges().values();
    //Sort edges by average Y height
    std::stable_sort(edges.begin(), edges.end(), [ = ](Edge * first, Edge * second) {
        if (StateManager::currentRoute().contains(first) == StateManager::currentRoute().contains(second)) {
            return first->averageY() < second->averageY();
        } else if (StateManager::currentRoute().contains(first)) {
            return false;
        } else {
            return true;
        }
    });
    for (Edge* edge : qAsConst(edges)) {
        //Draw the edge
        if (drawnEdges.contains(edge)) continue;
        if (edge->isTemporary()) continue;

//        QPolygonF poly;
//        poly.append(QPointF(edge->from()->x(), edge->from()->z()));
//        poly.append(QPointF(edge->to()->x(), edge->to()->z()));

//        Edge* tracking = edge;
//        QList<Edge*> trackingEdges;

//        do {
//            trackingEdges = DataManager::edgesToNode(tracking->from());
//            for (Edge* edge : trackingEdges) {
//                if (edge->road() == tracking->road() && !drawnEdges.contains(edge)) {
//                    poly.prepend(QPointF(edge->from()->x(), edge->from()->z()));
//                    tracking = edge;
//                    drawnEdges.insert(edge);
//                    break;
//                }
//            }
//        } while (trackingEdges.contains(tracking));

//        tracking = edge;

//        do {
//            trackingEdges = DataManager::edgesFromNode(tracking->to());
//            for (Edge* edge : trackingEdges) {
//                if (edge->road() == tracking->road() && !drawnEdges.contains(edge)) {
//                    poly.append(QPointF(edge->to()->x(), edge->to()->z()));
//                    tracking = edge;
//                    drawnEdges.insert(edge);
//                    break;
//                }
//            }
//        } while (trackingEdges.contains(tracking));

        painter.setPen(edge->road()->pen(edge));
        painter.drawLine(edge->line());
//        painter.drawPolyline(poly);

        drawnEdges.insert(edge);
    }

    for (Landmark* landmark : DataManager::landmarks().values()) {
        QSvgRenderer renderer(QStringLiteral(":/landmarks/%1.svg").arg(landmark->type()));
        renderer.render(&painter, landmark->hitbox());
    }

    painter.end();

    this->update();
}

void MapWidget::paintEvent(QPaintEvent* event) {
    QPicture pic;

    QFont font = this->font();
    font.setPointSizeF(20 / d->scale);

    {
        QPainter painter;
        painter.begin(&pic);

        //Draw the base map
        d->baseMap.play(&painter);

        painter.setFont(font);

        for (Edge* edge : StateManager::currentRoute()) {
            QPen pen = edge->road()->pen(edge);
            pen.setColor(QColor(0, 150, 255));
            painter.setPen(pen);
            painter.drawLine(edge->line());
        }

        for (QObject* target : d->hoverTargets) {
            Edge* edge = qobject_cast<Edge*>(target);
            if (edge) {
                QPen pen = edge->road()->pen(edge);
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                painter.drawLine(edge->line());
            }
        }

        if (StateManager::currentInstruction() != -1) {
            StateManager::Instruction inst = StateManager::currentInstructions().at(StateManager::currentInstruction());
            if (inst.fromEdge && inst.toEdge) {
                QPolygonF arrow;
                QLineF fromLine(inst.fromEdge->line().p2(), inst.fromEdge->line().p2() + QPointF(100, 0) / d->scale);
                fromLine.setAngle(inst.fromEdge->line().angle());

                arrow.append(fromLine.pointAt(-1));
                arrow.append(fromLine.pointAt(0));
                fromLine.setAngle(inst.toEdge->line().angle());
                arrow.append(fromLine.pointAt(1));

                painter.setPen(QPen(QColor(100, 50, 150), inst.fromEdge->road()->pen(inst.fromEdge).widthF(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.setBrush(QColor(100, 50, 150));

                QPolygonF arrowHead;
                arrowHead.append(fromLine.pointAt(1));

                QLineF arrowHeadBase(fromLine.pointAt(1), fromLine.pointAt(1) + QPointF(50, 0) / d->scale);
                arrowHeadBase.setAngle(fromLine.normalVector().angle());

                fromLine.setLength(fromLine.length() + 50 / d->scale);

                arrowHead.append(arrowHeadBase.pointAt(1));
                arrowHead.append(fromLine.pointAt(1));
                arrowHead.append(arrowHeadBase.pointAt(-1));

                painter.drawPolyline(arrow);
                painter.drawPolygon(arrowHead);
            }
        }

        painter.end();
    }

    bool goMode = StateManager::currentState() == StateManager::Go;
    QPointF goModeOrigin(this->width() / 2, this->height() * 0.8);
    {
        QPainter painter(this);

        //Draw the background
        painter.fillRect(0, 0, this->width(), this->height(), this->palette().color(QPalette::Window));

        painter.setWorldTransform(currentTransform());
        painter.setRenderHint(QPainter::Antialiasing);

        Player* player = StateManager::loggedInPlayer();
        if (goMode) {
            QTransform playerTransform;
            playerTransform.rotate(player->markerAngle() - 90);
            playerTransform.scale(d->scale, d->scale);

            QRect viewport(goModeOrigin.toPoint(), QSize(this->width(), this->height()));
            painter.setViewport(viewport);

            painter.setTransform(playerTransform);
            painter.drawPicture(-player->markerCoordinates(), pic);
        } else {
            painter.drawPicture(0, 0, pic);
        }

        painter.setFont(font);

        //Draw players
        painter.save();
        QSvgRenderer playerMarkRenderer(QStringLiteral(":/playermark.svg"));
        for (Player* player : DataManager::players()) {
            if (goMode && player != StateManager::loggedInPlayer()) continue;

            QRectF circle;
            circle.setSize(QSizeF(15, 15) * d->scale);
            circle.moveCenter(QPointF(0, 0));

            QPointF markerCoordinates = player->markerCoordinates();

            painter.save();
            painter.resetTransform();
            if (goMode) {
                painter.translate(goModeOrigin);
                painter.rotate(-90);
            } else {
                painter.translate(currentTransform().map(markerCoordinates));
                painter.rotate(-player->markerAngle());
            }
            playerMarkRenderer.render(&painter, circle);
            painter.restore();

            painter.setPen(StateManager::nightMode() ? Qt::white : Qt::black);

            circle.setSize(QSizeF(15, 15));
            if (goMode) {
                painter.resetTransform();
//                painter.translate(goModeOrigin);
//                painter.scale(d->scale, d->scale);
                circle.moveCenter(goModeOrigin);
                painter.setFont(this->font());
            } else {
                circle.moveCenter(markerCoordinates);
            }

            double scale = goMode ? 1 : d->scale;

            QRectF text;
            text.setHeight(painter.fontMetrics().height());
            text.setWidth(painter.fontMetrics().horizontalAdvance(player->name()));
            text.moveCenter(circle.center());
            text.moveTop(circle.bottom() + 10 / scale);
            if (!goMode) painter.drawText(text, player->name());

            if (player->snappedEdge()) {
                QRectF roadText;
                roadText.setHeight(painter.fontMetrics().height());
                roadText.setWidth(painter.fontMetrics().horizontalAdvance(player->snappedEdge()->road()->name()) + 20 / scale);
                roadText.moveCenter(text.center());

                QRectF roadBacking = roadText.adjusted(-20 / scale, -5 / scale, 20 / scale, 5 / scale);
                if (goMode) {
                    roadBacking.moveTop(text.bottom() + 10 * d->scale);
                } else {
                    roadBacking.moveTop(text.bottom() + 10 / d->scale);
                }

                painter.setBrush(QColor(0, 130, 120));
                painter.setPen(Qt::transparent);
                painter.drawRoundedRect(roadBacking, roadBacking.height() / 2, roadBacking.height() / 2, Qt::AbsoluteSize);

                roadText.moveCenter(roadBacking.center());
                painter.setPen(Qt::white);
                painter.drawText(roadText, Qt::AlignCenter, player->snappedEdge()->road()->name());
            }
        }
        painter.restore();

        //Draw pin
        if (!goMode && StateManager::selectedLandmark()) {
            QSvgRenderer pinRenderer(QStringLiteral(":/pin.svg"));
            QRectF rect(QPointF(0, 0), pinRenderer.defaultSize());
            rect.moveCenter(StateManager::selectedLandmark()->hitbox().center());
            rect.moveBottom(StateManager::selectedLandmark()->hitbox().center().y());
            pinRenderer.render(&painter, rect);
        }

        if (StateManager::currentState() == StateManager::Edit) {
            for (Node* node : DataManager::nodes().values()) {
                //Draw the node
                //Don't draw temporary nodes
                if (node->isTemporary()) continue;

                painter.setPen(QPen(Qt::black, 2 / d->scale));
                painter.setBrush(Qt::white);
                if (d->firstNode == node) painter.setPen(QPen(Qt::red, 0.1));
                if (d->hoverTargets.contains(node)) painter.setPen(QPen(Qt::blue, 0.1));

                QRectF nodeRect = node->nodeRect(d->scale);
                if (d->dragNode == node) nodeRect.moveCenter(d->dragNodeCoordinates);
                painter.drawRect(nodeRect);
            }
        }
    }

}

void MapWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (!d->firstNode && !d->hoverTargets.isEmpty() && qobject_cast<Node*>(d->hoverTargets.first())) {
            d->dragNode = qobject_cast<Node*>(d->hoverTargets.first());
            d->initialNodeCoordinates = QPoint(d->dragNode->x(), d->dragNode->z());
            d->dragNodeCoordinates = d->initialNodeCoordinates;
        } else {
            d->dragging = true;
            d->dragStart = event->globalPos();
            d->dragInitial = event->globalPos();
        }
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (d->dragNode) {
            if (d->dragNodeCoordinates == d->initialNodeCoordinates) {
                doClick();
            } else {
                d->dragNode->submitUpdate(d->dragNodeCoordinates.x(), d->dragNode->y(), d->dragNodeCoordinates.y(), [ = ](bool error) {
                    if (error) {
                        QMessageBox::warning(this, tr("Could not update node"), tr("Could not update the node."));
                    }
                });
            }
            d->dragNode = nullptr;
        } else if (d->dragging) {
            d->dragging = false;
            if ((d->dragInitial - d->dragStart).manhattanLength() < 5) {
                //Treat this as a click!
                doClick();
            }
        }
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent* event) {
    if (d->dragNode) {
        d->dragNodeCoordinates = toMapCoordinates(event->pos()).toPoint();
    } else if (d->dragging) {
        if (StateManager::followMe()) StateManager::setFollowMe(false);
        d->origin += event->globalPos() - d->dragStart;
        d->dragStart = event->globalPos();
    } else {
        //Figure out what we're on
        d->hoverTargets.clear();
        QPointF mapPos = toMapCoordinates(event->pos());

        if (StateManager::currentState() == StateManager::Edit) {
            for (Node* node : DataManager::nodes().values()) {
                if (node->isTemporary()) continue;
                if (node->nodeRect(d->scale).contains(mapPos)) d->hoverTargets.append(node);
            }

            for (Edge* edge : DataManager::edges().values()) {
                if (edge->isTemporary()) continue;
                double width = edge->road()->pen(edge).widthF();
                if (edge->hitbox(width).containsPoint(mapPos, Qt::OddEvenFill)) d->hoverTargets.append(edge);
            }
        }

        for (Landmark* landmark : DataManager::landmarks().values()) {
            if (landmark->hitbox().contains(mapPos)) d->hoverTargets.append(landmark);
        }
    }

    emit pan(-d->origin.x() / d->scale, -d->origin.y() / d->scale);
    this->update();
}

void MapWidget::wheelEvent(QWheelEvent* event) {
    double factor = 1.0 + 10 * (event->angleDelta().y() / 12000.0);
    d->scale *= factor;
    d->origin = (d->origin - event->position()) * factor + event->position();
    this->update();
}

void MapWidget::contextMenuEvent(QContextMenuEvent* event) {
    QMenu* menu = new QMenu();

    QPoint mapPos = toMapCoordinates(event->pos()).toPoint();
    menu->addAction(tr("Route from here"), [ = ] {
        emit routeFrom(mapPos);
    });
    menu->addAction(tr("Route to here"), [ = ] {
        emit routeTo(mapPos);
    });

    if (StateManager::currentState() == StateManager::Edit) {
        for (QObject* target : d->hoverTargets) {
            Node* hoverNode = qobject_cast<Node*>(target);
            Edge* hoverEdge = qobject_cast<Edge*>(target);

            if (hoverNode) {
                menu->addSection(tr("Node"));
                menu->addAction(tr("Node ID: %1").arg(DataManager::nodes().key(hoverNode)))->setEnabled(false);

                Landmark* attached = nullptr;
                for (Landmark* landmark : DataManager::landmarks().values()) {
                    if (landmark->node() == hoverNode) attached = landmark;
                }

                if (attached) {
                    menu->addAction(tr("Detach Landmark"), this, [ = ] {
                        //Detach this landmark!
                        DataGatherer::del(QStringLiteral("/landmarks/%1").arg(DataManager::landmarks().key(attached)), [ = ](bool error) {
                            if (error) {
                                QMessageBox::warning(this, tr("Could not detach landmark"), tr("Could not detach the landmark."));
                            }
                        });
                    });
                } else {
                    menu->addAction(tr("Attach Landmark"), this, [ = ] {
                        //Connect these nodes!
                        NewLandmarkDialog* dialog = new NewLandmarkDialog(hoverNode);
                        dialog->setWindowModality(Qt::ApplicationModal);
                        connect(dialog, &NewLandmarkDialog::finished, dialog, &NewLandmarkDialog::deleteLater);
                        dialog->open();
                    });
                }
                menu->addAction(tr("Delete Node"), this, [ = ] {
                    DataGatherer::del(QStringLiteral("/nodes/%1").arg(DataManager::nodes().key(hoverNode)), [ = ](bool error) {
                        if (error) {
                            QMessageBox::warning(this, tr("Could not delete node"), tr("Could not delete the node."));
                        }
                    });
                });
            } else if (hoverEdge) {
                menu->addSection(tr("Edge"));
                menu->addAction(hoverEdge->road()->name())->setEnabled(false);
                menu->addAction(tr("Edit Road"), this, [ = ] {
                    NewRoadDialog* d = new NewRoadDialog(DataManager::roads().key(hoverEdge->road()), this);
                    d->setWindowModality(Qt::ApplicationModal);
                    connect(d, &NewRoadDialog::finished, d, &QDialog::deleteLater);
                    d->open();
                });
                menu->addAction(tr("Delete Edge"), this, [ = ] {
                    DataGatherer::del(QStringLiteral("/edges/%1").arg(DataManager::edges().key(hoverEdge)), [ = ](bool error) {
                        if (error) {
                            QMessageBox::warning(this, tr("Could not delete edge"), tr("Could not delete the edge."));
                        }
                    });
                });
            }
        }
    }

    menu->popup(event->globalPos());
    connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
}
