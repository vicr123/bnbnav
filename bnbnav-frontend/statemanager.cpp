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
#include "statemanager.h"

#include <QSettings>
#include <QSet>
#include "datamanager.h"
#include "edge.h"
#include "road.h"
#include "node.h"
#include "player.h"
#include <QLineF>
#include <QElapsedTimer>
#include <QPainter>
#include <QPalette>
#include <QSvgRenderer>

struct StateManagerPrivate {
    StateManager::GlobalState state = StateManager::Browse;
    QString login;
    QList<Edge*> currentRoute;

    QList<StateManager::Instruction> instructions;
    int currentInstruction = -1;
    double blocksToNextInstruction = 0;
    bool isSnappedToRoute = false;
    QElapsedTimer lastSnappedToRoute;

    bool followMe;
};

StateManager* StateManager::instance() {
    static StateManager* instance = new StateManager();
    return instance;
}

StateManager::GlobalState StateManager::currentState() {
    return instance()->d->state;
}

void StateManager::setCurrentState(GlobalState state) {
    if (state == Go) {
        setFollowMe(true);
    }

    instance()->d->state = state;
    emit instance()->stateChanged(state);
}

void StateManager::setLogin(QString login) {
    QSettings settings;
    settings.setValue("login/username", login);
    instance()->d->login = login;
    emit instance()->loginChanged(login);

    setFollowMe(false);
}

QString StateManager::login() {
    return instance()->d->login;
}

Player* StateManager::loggedInPlayer() {
    return DataManager::players().value(login());
}

void StateManager::setCurrentRoute(QList<Edge*> edges) {
    instance()->d->currentRoute = edges;

    if (currentRoute().isEmpty()) {
        instance()->d->currentInstruction = -1;
        instance()->d->instructions.clear();
        emit instance()->routeChanged();
        return;
    }

    QList<Instruction> instructions;

    //Always add a departure instruction
    Instruction departure;
    departure.type = Instruction::Departure;
    departure.node = currentRoute().first()->from();
    departure.distance = 0;
    departure.toEdge = currentRoute().first();
    instructions.append(departure);

    double currentLength = 0;
    for (int i = 1; i < currentRoute().length(); i++) {
        Edge* previousEdge = currentRoute().at(i - 1);
        Edge* edge = currentRoute().at(i);
        currentLength += previousEdge->length();

        Instruction inst;
        inst.node = edge->from();
        inst.fromEdge = previousEdge;
        inst.toEdge = edge;
        inst.distance = currentLength;
        inst.turnAngle = 0;

        if (edge->isTemporary()) continue;

        bool isMultiRoad = false;
        for (QString roadId : DataManager::roadsConnectedToNode(edge->from())) {
            if (DataManager::roads().value(roadId)->name() != edge->road()->name()) isMultiRoad = true;
        }
        bool isSameRoad = edge->road()->name() == previousEdge->road()->name();
        double turnAngle = 360 - edge->line().angleTo(previousEdge->line());
        inst.turnAngle = turnAngle;

        if (turnAngle < 10 || turnAngle > 350) { //Straight!
            if (isSameRoad) continue;

            inst.type = Instruction::ContinueStraight;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 80) { //Bear Left!
            if (!isMultiRoad) continue;

            inst.type = edge->road()->type() == "motorway" ? Instruction::Merge : (previousEdge->road()->type() == "motorway" ? Instruction::ExitLeft : Instruction::BearLeft);
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 100) { //Turn Left!
            if (!isMultiRoad) continue;

            inst.type = Instruction::TurnLeft;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 160) { //Sharp Left!
            inst.type = Instruction::SharpLeft;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 200) { //U-turn!
            inst.type = Instruction::TurnAround;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 260) { //Sharp Right!
            inst.type = Instruction::SharpRight;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 280) { //Turn Right!
            if (!isMultiRoad) continue;

            inst.type = Instruction::TurnRight;
            instructions.append(inst);
            currentLength = 0;
        } else { //Bear Right!
            if (!isMultiRoad) continue;

            inst.type = edge->road()->type() == "motorway" ? Instruction::Merge : (previousEdge->road()->type() == "motorway" ? Instruction::ExitRight : Instruction::BearRight);
            instructions.append(inst);
            currentLength = 0;
        }
    }

    //Always add an arrive instruction
    Instruction arrival;
    arrival.type = Instruction::Arrival;
    arrival.node = currentRoute().last()->to();
    arrival.distance = currentLength;
    arrival.toEdge = nullptr;
    instructions.append(arrival);

    instance()->d->instructions = instructions;
    emit instance()->routeChanged();
}

QList<Edge*> StateManager::currentRoute() {
    return instance()->d->currentRoute;
}

QList<StateManager::Instruction> StateManager::currentInstructions() {
    return instance()->d->instructions;
}

int StateManager::currentInstruction() {
    return instance()->d->currentInstruction;
}

double StateManager::blocksToNextInstruction() {
    return instance()->d->blocksToNextInstruction;
}

void StateManager::setFollowMe(bool follow) {
    instance()->d->followMe = follow;
    emit instance()->followMeChanged(follow);
}

bool StateManager::followMe() {
    return instance()->d->followMe;
}

StateManager::StateManager(QObject* parent) : QObject(parent) {
    d = new StateManagerPrivate();

    QSettings settings;
    d->login = settings.value("login/username").toString();

    connect(this, &StateManager::loginChanged, this, &StateManager::calculateCurrentInstruction);
    connect(this, &StateManager::routeChanged, this, &StateManager::calculateCurrentInstruction);
    connect(DataManager::instance(), &DataManager::playerUpdate, this, [ = ](QString player) {
        if (player == d->login) calculateCurrentInstruction();
    });
}

void StateManager::calculateCurrentInstruction() {
    if (d->login.isEmpty()) {
        d->currentInstruction = -1;
        emit currentInstructionChanged();
        return;
    }

    Player* player = DataManager::players().value(d->login);
    if (!player) {
        d->currentInstruction = -1;
        emit currentInstructionChanged();
        return;
    }

    if (!d->currentRoute.contains(player->snappedEdge())) {
        if (d->isSnappedToRoute) {
            d->isSnappedToRoute = false;
            d->lastSnappedToRoute.start();
        } else if (d->lastSnappedToRoute.isValid() && d->lastSnappedToRoute.elapsed() > 3000) {
            d->currentInstruction = -1;
            d->lastSnappedToRoute.invalidate();
            emit currentInstructionChanged();
        }
        return;
    }
    d->lastSnappedToRoute.invalidate();
    d->isSnappedToRoute = true;

    d->blocksToNextInstruction = 0;
    bool instructionFound = false;
    int instructionIndex = 0;
    for (Edge* edge : d->currentRoute) {
        if (d->instructions.at(instructionIndex).toEdge == edge) {
            instructionIndex++;
            if (instructionFound) {
                emit currentInstructionChanged();
                return;
            }
        }
        if (instructionFound) {
            d->blocksToNextInstruction += edge->length();
        }
        if (player->snappedEdge() == edge) {
            //We found it!
            d->currentInstruction = instructionIndex;
            instructionFound = true;

            QLineF distance(edge->to()->x(), edge->to()->z(), player->x(), player->z());
            d->blocksToNextInstruction += distance.length();
        }
    }
    //We should never get here?

    if (instructionFound) {
        emit currentInstructionChanged();
    }
}

QString StateManager::Instruction::humanReadableString(int distance) {
    switch (type) {
        case StateManager::Instruction::Departure:
            return tr("Depart");
        case StateManager::Instruction::Arrival:
            return tr("Arrive in %n blocks", nullptr, distance);
        case StateManager::Instruction::ContinueStraight:
            return tr("In %n blocks, continue onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::BearLeft:
            return tr("In %n blocks, bear left onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::TurnLeft:
            return tr("In %n blocks, turn left onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::SharpLeft:
            return tr("In %n blocks, sharp left onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::TurnAround:
            return tr("In %n blocks, perform a U-turn onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::SharpRight:
            return tr("In %n blocks, sharp right onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::TurnRight:
            return tr("In %n blocks, turn right onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::BearRight:
            return tr("In %n blocks, bear right onto %1", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::ExitLeft:
            return tr("In %n blocks, take the %1 exit on the left", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::ExitRight:
            return tr("In %n blocks, take the %1 exit on the right", nullptr, distance).arg(toEdge->road()->name());
        case StateManager::Instruction::Merge:
            return tr("In %n blocks, merge onto %1", nullptr, distance).arg(toEdge->road()->name());
    }
    return tr("Get onto %1").arg(toEdge->road()->name());
}

int StateManager::Instruction::height() {
    return 100;
}

void StateManager::Instruction::render(QPainter* painter, QRect rect, QFont font, QPalette pal, int distance) {
    QFontMetrics metrics(font);

    painter->setFont(font);
    painter->setPen(pal.color(QPalette::WindowText));

    QRect distanceText;
    distanceText.setHeight(metrics.height());
    distanceText.setWidth(metrics.horizontalAdvance(tr("%n blk", nullptr, 9999)));
    distanceText.moveBottom(rect.bottom() - 9);
    distanceText.moveLeft(rect.left() + 9);
    painter->drawText(distanceText, Qt::AlignCenter, tr("%n blk", nullptr, distance));

    QRect imageRect;
    imageRect.setHeight(distanceText.top() - rect.top() - 18);
    imageRect.setWidth(imageRect.height());
    imageRect.moveCenter(distanceText.center());
    imageRect.moveBottom(distanceText.top() - 9);

    QSvgRenderer renderer(QStringLiteral(":/directions/%1.svg").arg(imageName()));
    renderer.render(painter, imageRect);

    QRect roadText = rect;
    roadText.setLeft(distanceText.right() + 9);
    roadText.setRight(rect.right() - 9);

    QFont roadFont = font;
    roadFont.setPointSizeF(font.pointSizeF() * 1.5);
    painter->setFont(roadFont);

    QString text;
    if (toEdge) {
        text = toEdge->road()->name();
    } else {
        text = tr("Arrive at the destination");
    }

    painter->save();
    double width = QFontMetrics(roadFont).horizontalAdvance(text);
    if (width > roadText.width()) {
        //Scale the painter
        painter->scale(roadText.width() / width, 1);
        roadText.setWidth(width);
    }
    painter->drawText(roadText, Qt::AlignLeft | Qt::AlignVCenter, text);
    painter->restore();
}

QString StateManager::Instruction::imageName() {
    switch (type) {
        case StateManager::Instruction::Departure:
            return "depart";
        case StateManager::Instruction::Arrival:
            return "arrive";
        case StateManager::Instruction::ContinueStraight:
            return "continue-straight";
        case StateManager::Instruction::BearLeft:
            return "bear-left";
        case StateManager::Instruction::TurnLeft:
            return "turn-left";
        case StateManager::Instruction::SharpLeft:
            return "sharp-left";
        case StateManager::Instruction::TurnAround:
            return "turn-around";
        case StateManager::Instruction::SharpRight:
            return "sharp-right";
        case StateManager::Instruction::TurnRight:
            return "turn-right";
        case StateManager::Instruction::BearRight:
            return "bear-right";
        case StateManager::Instruction::ExitLeft:
            return "turn-left";
        case StateManager::Instruction::ExitRight:
            return "exit-right";
        case StateManager::Instruction::Merge:
            return "merge";
    }
    return "";
}


//-600,320
