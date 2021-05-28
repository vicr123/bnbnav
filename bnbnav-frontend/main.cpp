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

#include <QApplication>
#include <QStyleFactory>
#include <QFontDatabase>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName("bnbnav");
    a.setOrganizationName("theSuite");
    a.setOrganizationDomain("vicr123.com");

    QFontDatabase::addApplicationFont(":/Overpass-Regular.ttf");
    a.setFont(QFont("Overpass", 15));

    a.setStyle(QStyleFactory::create("fusion"));

    QPalette pal = a.palette();
    pal.setColor(QPalette::Window, Qt::white);
    pal.setColor(QPalette::WindowText, Qt::black);
    a.setPalette(pal);

    MainWindow* w = new MainWindow();
    w->setFont(a.font());
    w->show();
    return a.exec();
}
