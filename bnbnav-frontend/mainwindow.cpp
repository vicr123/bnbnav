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
#include "loginwidget.h"
#include "splashwidget.h"
#include "datamanager.h"
#include "statedialog.h"
#include "statemanager.h"
#include "landmark.h"

struct MainWindowPrivate {
    MapWidget* map;
    LoginWidget* login;
    SplashWidget* splash;
    StateDialog* stateDialog;
    int readyCount = 0;
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

    d->login = new LoginWidget(ui->centralwidget);
    d->login->move(0, 0);
    d->login->hide();
    connect(ui->topWidget, &TopWidget::showLoginWidget, this, [ = ] {
        if (d->login->isVisible()) {
            d->login->hide();
        } else {
            d->login->show();
        }
    });
    d->login->installEventFilter(this);

    d->stateDialog = new StateDialog(this);
    d->stateDialog->setWindowFlags(Qt::Tool);

    d->splash = new SplashWidget(this);
    d->splash->move(0, 0);
    d->splash->resize(this->width(), this->height());
    d->splash->raise();

    connect(DataManager::instance(), &DataManager::ready, this, [ = ] {
        d->splash->hide();
        d->stateDialog->show();
    });
    connect(DataManager::instance(), &DataManager::loadError, this, [ = ] {
        d->splash->show();
        d->stateDialog->hide();
    });

    connect(d->map, &MapWidget::routeFrom, d->stateDialog, &StateDialog::routeFrom);
    connect(d->map, &MapWidget::routeTo, d->stateDialog, &StateDialog::routeTo);
    connect(d->map, &MapWidget::landmarkClicked, d->stateDialog, &StateDialog::showLandmark);
    connect(d->stateDialog, &StateDialog::focusMap, d->map, &MapWidget::focusMap);

    connect(StateManager::instance(), &StateManager::stateChanged, this, [ = ] {
        if (StateManager::currentState() == StateManager::Go && d->login->isVisible()) d->login->setVisible(false);
    });
}

MainWindow::~MainWindow() {
    delete d;
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    d->map->resize(this->size());
    d->splash->resize(this->size());
    d->login->move(this->width() - d->login->width(), ui->topWidget->height());
}


bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (watched == d->login && event->type() == QEvent::Resize) {
        d->login->move(this->width() - d->login->width(), ui->topWidget->height());
    }
    return false;
}
