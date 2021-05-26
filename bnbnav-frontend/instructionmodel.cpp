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
#include "instructionmodel.h"

#include "statemanager.h"

InstructionModel::InstructionModel(QObject* parent)
    : QAbstractListModel(parent) {
    connect(StateManager::instance(), &StateManager::routeChanged, this, [ = ] {
        emit dataChanged(index(0), index(rowCount()));
    });
}

int InstructionModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid()) return 0;

    return StateManager::currentInstructions().length();
}

QVariant InstructionModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    StateManager::Instruction instruction = StateManager::currentInstructions().value(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return instruction.humanReadableString(instruction.distance);
    }

    return QVariant();
}

InstructionDelegate::InstructionDelegate(QObject* parent) : QStyledItemDelegate(parent) {

}

void InstructionDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    StateManager::Instruction instruction = StateManager::currentInstructions().value(index.row());
    int distance = instruction.distance;
    int currentInstruction = StateManager::currentInstruction();
    if (currentInstruction != -1) {
        StateManager::Instruction ci = StateManager::currentInstructions().value(currentInstruction);
        if (ci.toEdge == instruction.toEdge) distance = StateManager::blocksToNextInstruction();
    }
    instruction.render(painter, option.rect, option.font, option.palette, distance);
}

QSize InstructionDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    StateManager::Instruction instruction = StateManager::currentInstructions().value(index.row());
    return QSize(300, instruction.height());
}
