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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mapwidget.h"

struct MainWindowPrivate {
    MapWidget* map;
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    d = new MainWindowPrivate();

    d->map = new MapWidget(ui->centralwidget);
    d->map->move(0, 0);
    d->map->resize(this->size());
    d->map->show();
    d->map->lower();

    connect(d->map, &MapWidget::pan, ui->topWidget, &TopWidget::setPan);
}

MainWindow::~MainWindow() {
    delete d;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    d->map->resize(this->size());
}
