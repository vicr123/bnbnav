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
#include "splashwidget.h"
#include "ui_splashwidget.h"

#include <QPainter>
#include <QSvgRenderer>

SplashWidget::SplashWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SplashWidget) {
    ui->setupUi(this);

    this->setAutoFillBackground(true);

    QPixmap px(QSize(256, 256));
    px.fill(this->palette().color(QPalette::Window));

    QPainter painter(&px);
    QSvgRenderer renderer(QStringLiteral(":/logo.svg"));
    renderer.render(&painter);

    ui->logoLabel->setPixmap(px);
}

SplashWidget::~SplashWidget() {
    delete ui;
}
