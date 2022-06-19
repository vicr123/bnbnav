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

#include "datamanager.h"
#include "edge.h"
#include "node.h"
#include "player.h"
#include "road.h"
#include <QElapsedTimer>
#include <QLineF>
#include <QPainter>
#include <QPalette>
#include <QSet>
#include <QSettings>
#include <QSvgRenderer>
#include <math.h>

struct StateManagerPrivate {
        StateManager::GlobalState state = StateManager::Browse;
        QString login;
        QList<Edge*> currentRoute;

        QList<StateManager::Instruction> instructions;
        int currentInstruction = -1;
        double blocksToNextInstruction = 0;
        bool isSnappedToRoute = false;
        QElapsedTimer lastSnappedToRoute;

        Landmark* selectedLandmark;

        StateManager::RouteOptions routeOptions = 0;
        StateManager::SpyMode spyMode = StateManager::SpyDisabled;

        bool followMe = false;
        bool nightMode = false;

        QString token;
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

    if (state != Edit) {
        setSpyMode(SpyDisabled);
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

    // Always add a departure instruction
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

        bool isMultiRoad = DataManager::edgesToNode(edge->from()).count() != DataManager::edgesFromNode(edge->from()).count();
        for (QString roadId : DataManager::roadsConnectedToNode(edge->from())) {
            if (DataManager::roads().value(roadId)->name() != edge->road()->name()) isMultiRoad = true;
        }
        bool isSameRoad = edge->road()->name() == previousEdge->road()->name();
        double turnAngle = 360 - edge->line().angleTo(previousEdge->line());
        inst.turnAngle = turnAngle;

        if (edge->road()->type() == "roundabout") {
            if (previousEdge->road()->type() == "roundabout") continue;

            for (int j = i + 1; j < currentRoute().length(); j++) {
                Edge* testEdge = currentRoute().at(j);
                if (testEdge->road()->type() != "roundabout") {
                    inst.roundaboutExit = j - i;
                    inst.roundaboutExitEdge = testEdge;

                    QLineF firstLine = previousEdge->line();
                    firstLine.setAngle(firstLine.angle() - 180);
                    inst.roundaboutExitAngle = firstLine.angleTo(testEdge->line());
                    break;
                }
            }

            inst.type = Instruction::EnterRoundabout;
            instructions.append(inst);
            currentLength = 0;
        } else if (previousEdge->road()->type() == "roundabout") {
            inst.type = Instruction::LeaveRoundabout;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 10 || turnAngle > 350) { // Straight!
            if (isSameRoad) continue;

            inst.type = Instruction::ContinueStraight;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 80) { // Bear Left!
            if (!isMultiRoad) continue;

            inst.type = edge->road()->type() == "motorway" ? Instruction::Merge : (previousEdge->road()->type() == "motorway" ? Instruction::ExitLeft : Instruction::BearLeft);
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 100) { // Turn Left!
            if (!isMultiRoad) continue;

            inst.type = Instruction::TurnLeft;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 160) { // Sharp Left!
            inst.type = Instruction::SharpLeft;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 200) { // U-turn!
            inst.type = Instruction::TurnAround;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 260) { // Sharp Right!
            inst.type = Instruction::SharpRight;
            instructions.append(inst);
            currentLength = 0;
        } else if (turnAngle < 280) { // Turn Right!
            if (!isMultiRoad) continue;

            inst.type = Instruction::TurnRight;
            instructions.append(inst);
            currentLength = 0;
        } else { // Bear Right!
            if (!isMultiRoad) continue;

            inst.type = edge->road()->type() == "motorway" ? Instruction::Merge : (previousEdge->road()->type() == "motorway" ? Instruction::ExitRight : Instruction::BearRight);
            instructions.append(inst);
            currentLength = 0;
        }
    }

    // Always add an arrive instruction
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

QList<StateManager::InstructionVoicePrompt> StateManager::voicePrompts() {
    QList<InstructionVoicePrompt> prompts;
    int totalBlocks = 0;
    int instIndex = StateManager::currentInstructions().length() - 1;

    for (auto inst = StateManager::currentInstructions().crbegin(); inst != StateManager::currentInstructions().crend(); inst++) {
        for (int targetDistance : QList<int>({10, 100, 500, static_cast<int>(inst->distance - 10)})) {
            if (targetDistance > inst->distance - 10 || targetDistance < 0) continue;
            int testBlocks = totalBlocks + targetDistance;

            InstructionVoicePrompt prompt;
            prompt.atBlocks = testBlocks;
            prompt.forInstruction = instIndex;
            prompts.append(prompt);
        }
        totalBlocks += inst->distance;
        instIndex--;
    }

    return prompts;
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

void StateManager::setSelectedLandmark(Landmark* landmark) {
    instance()->d->selectedLandmark = landmark;
    emit instance()->selectedLandmarkChanged();
}

Landmark* StateManager::selectedLandmark() {
    return instance()->d->selectedLandmark;
}

void StateManager::setNightMode(bool nightMode) {
    instance()->d->nightMode = nightMode;
    emit instance()->nightModeChanged();
}

bool StateManager::nightMode() {
    return instance()->d->nightMode;
}

StateManager::RouteOptions StateManager::routeOptions() {
    return instance()->d->routeOptions;
}

void StateManager::setRouteOption(RouteOption option, bool on) {
    if (on) {
        instance()->d->routeOptions |= option;
    } else {
        instance()->d->routeOptions &= ~option;
    }
    emit instance()->routeOptionsChanged(instance()->d->routeOptions);
}

QString StateManager::token() {
    return instance()->d->token;
}

void StateManager::setToken(QString token) {
    instance()->d->token = token;
}

StateManager::SpyMode StateManager::spyMode() {
    return instance()->d->spyMode;
}

void StateManager::setSpyMode(SpyMode spyMode) {
    instance()->d->spyMode = spyMode;
    emit instance()->spyModeChanged(spyMode);
}

StateManager::StateManager(QObject* parent) :
    QObject(parent) {
    d = new StateManagerPrivate();

    QSettings settings;
    d->login = settings.value("login/username").toString();

    connect(this, &StateManager::loginChanged, this, &StateManager::calculateCurrentInstruction);
    connect(this, &StateManager::routeChanged, this, &StateManager::calculateCurrentInstruction);
    connect(DataManager::instance(), &DataManager::playerUpdate, this, [=](QString player) {
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
            // We found it!
            d->currentInstruction = instructionIndex;
            instructionFound = true;

            QLineF distance(edge->to()->x(), edge->to()->z(), player->x(), player->z());
            d->blocksToNextInstruction += distance.length();
        }
    }
    // We should never get here?

    if (instructionFound) {
        emit currentInstructionChanged();
    }
}

QString StateManager::Instruction::humanReadableString(int distance) {
    return tr("In %n blocks, %1", nullptr, distance).arg(instructionString());
}

QString StateManager::Instruction::instructionString() const {
    switch (type) {
        case StateManager::Instruction::Departure:
            return tr("Depart");
        case StateManager::Instruction::Arrival:
            return tr("Arrive at the destination");
        case StateManager::Instruction::ContinueStraight:
            return tr("Continue onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::BearLeft:
            return tr("Bear left onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::TurnLeft:
            return tr("Turn left onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::SharpLeft:
            return tr("Sharp left onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::TurnAround:
            return tr("Perform a U-turn onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::SharpRight:
            return tr("Sharp right onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::TurnRight:
            return tr("Turn right onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::BearRight:
            return tr("Bear right onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::ExitLeft:
            return tr("Take the %1 exit on the left").arg(toEdge->road()->name());
        case StateManager::Instruction::ExitRight:
            return tr("Take the %1 exit on the right").arg(toEdge->road()->name());
        case StateManager::Instruction::Merge:
            return tr("Merge onto %1").arg(toEdge->road()->name());
        case StateManager::Instruction::EnterRoundabout:
            if (!roundaboutExitEdge) {
                return tr("Enter the roundabout");
            } else {
                return tr("At the roundabout, take exit %1 onto %2").arg(roundaboutExit).arg(roundaboutExitEdge->road()->name());
            }
        case StateManager::Instruction::LeaveRoundabout:
            return tr("Exit the roundabout onto %1").arg(toEdge->road()->name());
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

    painter->drawPixmap(imageRect, image(imageRect.size()));

    QRect roadText = rect;
    roadText.setLeft(distanceText.right() + 9);
    roadText.setRight(rect.right() - 9);

    QFont roadFont = font;
    roadFont.setPointSizeF(font.pointSizeF() * 1.5);
    painter->setFont(roadFont);

    QString text;
    if (roundaboutExitEdge) {
        text = roundaboutExitEdge->road()->name();
    } else if (toEdge) {
        text = toEdge->road()->name();
    } else {
        text = tr("Arrive at the destination");
    }

    painter->save();
    double width = QFontMetrics(roadFont).horizontalAdvance(text);
    if (width > roadText.width()) {
        // Scale the painter
        painter->scale(roadText.width() / width, 1);
        roadText.setWidth(width);
    }
    painter->drawText(roadText, Qt::AlignLeft | Qt::AlignVCenter, text);
    painter->restore();
}

QPixmap StateManager::Instruction::image(QSize size) {
    QPixmap px(size);
    px.fill(Qt::transparent);

    QPainter painter(&px);

    QString filename;
    switch (type) {
        case StateManager::Instruction::Departure:
            filename = "depart";
            break;
        case StateManager::Instruction::Arrival:
            filename = "arrive";
            break;
        case StateManager::Instruction::ContinueStraight:
            filename = "continue-straight";
            break;
        case StateManager::Instruction::BearLeft:
            filename = "bear-left";
            break;
        case StateManager::Instruction::TurnLeft:
            filename = "turn-left";
            break;
        case StateManager::Instruction::SharpLeft:
            filename = "sharp-left";
            break;
        case StateManager::Instruction::TurnAround:
            filename = "turn-around";
            break;
        case StateManager::Instruction::SharpRight:
            filename = "sharp-right";
            break;
        case StateManager::Instruction::TurnRight:
            filename = "turn-right";
            break;
        case StateManager::Instruction::BearRight:
            filename = "bear-right";
            break;
        case StateManager::Instruction::ExitLeft:
            filename = "exit-left";
            break;
        case StateManager::Instruction::ExitRight:
            filename = "exit-right";
            break;
        case StateManager::Instruction::Merge:
            filename = "merge";
            break;
        case StateManager::Instruction::EnterRoundabout:
            if (roundaboutExit == -1) {
                filename = "enter-roundabout";
            } else {
                QPen pen(QColor(255, 255, 255, 100), 0.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

                painter.setRenderHint(QPainter::Antialiasing);
                painter.setWindow(QRect(-1, -1, 2, 2));
                painter.setViewport(QRect(0, 0, size.width(), size.height()));
                painter.setBrush(Qt::transparent);

                QRectF circleRect(-0.2, -0.2, 0.4, 0.4);
                painter.setPen(pen);
                painter.drawEllipse(circleRect);

                pen.setColor(Qt::white);
                painter.setPen(pen);
                painter.drawArc(circleRect, -90 * 16, this->roundaboutExitAngle * 16);

                QLineF line(0, 0, 0, 1);
                painter.drawLine(line.pointAt(0.2), line.pointAt(0.7));
                line.setAngle(this->roundaboutExitAngle - 90);
                painter.drawLine(line.pointAt(0.2), line.pointAt(0.7));

                QPolygonF arrowhead;
                arrowhead.append(QPointF(-0.3, -0.4));
                arrowhead.append(QPointF(0, -0.7));
                arrowhead.append(QPointF(0.3, -0.4));
                painter.rotate(180 - this->roundaboutExitAngle);
                painter.drawPolyline(arrowhead);

                painter.end();
                return px;
            }
            break;
        case StateManager::Instruction::LeaveRoundabout:
            filename = "leave-roundabout";
            break;
    }
    QSvgRenderer renderer(QStringLiteral(":/directions/%1.svg").arg(filename));

    renderer.render(&painter);
    painter.end();

    return px;
}

//-600,320

QString StateManager::InstructionVoicePrompt::speech(InstructionVoicePrompt* thenInstruction) {
    Instruction inst = StateManager::currentInstructions().at(forInstruction);

    // Round the number
    int log = log10(StateManager::blocksToNextInstruction());
    int roundIncrements = pow(10, log) / 20;
    if (roundIncrements == 0) roundIncrements = 5;
    double blocks = StateManager::blocksToNextInstruction();
    blocks = qRound(blocks / roundIncrements) * roundIncrements;

    if (StateManager::blocksToNextInstruction() < 15) {
        QString instructionString = inst.instructionString();
        if (thenInstruction) instructionString += tr("Then, %1").arg(StateManager::currentInstructions().at(thenInstruction->forInstruction).instructionString());
        return instructionString;
    } else if (StateManager::blocksToNextInstruction() > 500) {
        return tr("Stay on %1 for %n blocks", nullptr, blocks).arg(inst.fromEdge->road()->name());
    } else {
        return inst.humanReadableString(static_cast<int>(blocks));
    }
}
