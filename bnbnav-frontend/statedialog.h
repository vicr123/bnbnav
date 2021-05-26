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
#ifndef STATEDIALOG_H
#define STATEDIALOG_H

#include <QDialog>

namespace Ui {
    class StateDialog;
}

class StateDialog : public QDialog {
        Q_OBJECT

    public:
        explicit StateDialog(QWidget* parent = nullptr);
        ~StateDialog();

    private slots:
        void on_getDirectionsButton_clicked();

        void on_startLocationBox_textChanged(const QString& arg1);

        void on_endLocationBox_textChanged(const QString& arg1);

        void on_goModeButton_clicked();

    private:
        Ui::StateDialog* ui;

        // QObject interface
    public:
        bool eventFilter(QObject* watched, QEvent* event);
};

#endif // STATEDIALOG_H
