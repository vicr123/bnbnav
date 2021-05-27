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
#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
    class LoginWidget;
}

class LoginWidget : public QWidget {
        Q_OBJECT

    public:
        explicit LoginWidget(QWidget* parent = nullptr);
        ~LoginWidget();

    private slots:
        void on_stackedWidget_currentChanged(int arg1);

        void on_loginButton_clicked();

        void on_logOutButton_clicked();

        void on_followMeButton_toggled(bool checked);

        void on_loginUsernameBox_returnPressed();

    private:
        Ui::LoginWidget* ui;
};

#endif // LOGINWIDGET_H
