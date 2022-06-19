/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2022 Victor Tran
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
#ifndef SPYWIDGET_H
#define SPYWIDGET_H

#include <QWidget>

namespace Ui {
    class SpyWidget;
}

class SpyWidget : public QWidget {
        Q_OBJECT

    public:
        explicit SpyWidget(QWidget* parent = nullptr);
        ~SpyWidget();

    private slots:
        void on_disableSpyButton_toggled(bool checked);

        void on_spyOnTurnRestrictionButton_toggled(bool checked);

    private:
        Ui::SpyWidget* ui;

        void updateSpyMode();
};

#endif // SPYWIDGET_H
