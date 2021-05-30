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
#include "locationentrycompleter.h"

#include "datamanager.h"
#include "landmark.h"

LocationEntryCompleter::LocationEntryCompleter(QObject* parent)
    : QAbstractListModel(parent) {
    connect(DataManager::instance(), &DataManager::newLandmark, this, [ = ] {
        emit dataChanged(index(0), index(rowCount()));
    });
    connect(DataManager::instance(), &DataManager::removedLandmark, this, [ = ] {
        emit dataChanged(index(0), index(rowCount()));
    });
}

int LocationEntryCompleter::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return DataManager::landmarks().count();
}

QVariant LocationEntryCompleter::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();

    Landmark* l = DataManager::landmarks().values().at(index.row());
    switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return l->name();
        case Qt::UserRole:
            return QVariant::fromValue(l);
    }

    return QVariant();
}
