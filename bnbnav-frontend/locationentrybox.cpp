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
#include "locationentrybox.h"

#include "statemanager.h"
#include <QRegularExpression>
#include "player.h"
#include "datamanager.h"
#include <QCompleter>
#include "locationentrycompleter.h"
#include "landmark.h"
#include "node.h"
#include <QDebug>
#include <QAbstractItemView>

struct LocationEntryBoxPrivate {
    Landmark* selectedLandmark = nullptr;
};

LocationEntryBox::LocationEntryBox(QWidget* parent) : QLineEdit(parent) {
    d = new LocationEntryBoxPrivate;

    QCompleter* completer = new QCompleter(new LocationEntryCompleter(this), this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    connect(completer, QOverload<const QModelIndex&>::of(&QCompleter::highlighted), this, [ = ](QModelIndex index) {
        d->selectedLandmark = index.data(Qt::UserRole).value<Landmark*>();
    });
    connect(completer, QOverload<const QModelIndex&>::of(&QCompleter::activated), this, [ = ](QModelIndex index) {
        d->selectedLandmark = index.data(Qt::UserRole).value<Landmark*>();
    });
    completer->popup()->setItemDelegate(new LocationEntryCompleterDelegate(this));
    this->setCompleter(completer);

    connect(this, &QLineEdit::textEdited, this, [ = ](QString text) {
        if (!d->selectedLandmark || d->selectedLandmark->name() != text) {
            d->selectedLandmark = nullptr;
        }
    });

    connect(this, &QLineEdit::returnPressed, this, &LocationEntryBox::go);
}

LocationEntryBox::~LocationEntryBox() {
    delete d;
}

QPoint LocationEntryBox::location() {
    if (d->selectedLandmark) {
        return QPoint(d->selectedLandmark->node()->x(), d->selectedLandmark->node()->z());
    } else {
        //Try searching by coordinate
        QRegularExpression exp(QRegularExpression::anchoredPattern("(-?\\d+),(-?\\d+)"));
        QRegularExpressionMatch match = exp.match(this->text());
        if (match.hasMatch()) {
            return QPoint(match.capturedTexts().at(1).toInt(), match.capturedTexts().at(2).toInt());
        }

        QString playerTest = this->text();
        if (this->text() == tr("My Location")) {
            QString playerTest = StateManager::login();
            if (playerTest.isEmpty()) return QPoint();
        }

        for (Player* player : DataManager::players()) {
            if (player->name() == playerTest) {
                return QPoint(player->x(), player->z());
            }
        }

        Landmark* l = landmark();
        if (l) return QPoint(l->node()->x(), l->node()->z());

        return QPoint();
    }
}

Landmark* LocationEntryBox::landmark() {
    if (d->selectedLandmark) return d->selectedLandmark;

    for (Landmark* landmark : DataManager::landmarks()) {
        if (landmark->name() == this->text()) return landmark;
    }

    return nullptr;
}
