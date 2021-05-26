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
#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>

struct MapWidgetPrivate;
class MapWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MapWidget(QWidget* parent = nullptr);

        QPointF toMapCoordinates(QPointF widget);

        QTransform currentTransform();

    signals:
        void pan(int x, int y);
        void routeTo(QPoint location);
        void routeFrom(QPoint location);

    private:
        MapWidgetPrivate* d;

        void doClick();
        void followPlayer();

        // QWidget interface
    protected:
        void paintEvent(QPaintEvent* event);
        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void wheelEvent(QWheelEvent* event);
        void contextMenuEvent(QContextMenuEvent* event);
};

#endif // MAPWIDGET_H
