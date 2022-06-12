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
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>

class QPainter;
class Edge;
class Node;
class Player;
class Landmark;
struct StateManagerPrivate;
class StateManager : public QObject {
        Q_OBJECT
    public:
        enum GlobalState {
            Browse,
            Edit,
            Go
        };

        enum RouteOption {
            AvoidDuongWarp = 0x1
        };
        typedef QFlags<RouteOption> RouteOptions;

        struct Instruction {
                enum InstructionType {
                    Departure,
                    Arrival,
                    ContinueStraight,
                    BearLeft,
                    TurnLeft,
                    SharpLeft,
                    TurnAround,
                    SharpRight,
                    TurnRight,
                    BearRight,
                    ExitLeft,
                    ExitRight,
                    Merge,
                    EnterRoundabout,
                    LeaveRoundabout
                };

                Node* node;
                Edge* fromEdge;
                Edge* toEdge;
                InstructionType type;
                double distance;
                double turnAngle;

                int roundaboutExit = -1;
                Edge* roundaboutExitEdge = nullptr;
                double roundaboutExitAngle;

                QString humanReadableString(int distance);
                QString instructionString() const;

                int height();
                void render(QPainter* painter, QRect rect, QFont font, QPalette pal, int distance);
                //            QString imageName();
                QPixmap image(QSize size);
        };

        struct InstructionVoicePrompt {
                int forInstruction;
                int atBlocks;

                QString speech(InstructionVoicePrompt* thenInstruction);
        };

        static StateManager* instance();

        static GlobalState currentState();
        static void setCurrentState(GlobalState state);

        static void setLogin(QString login);
        static QString login();
        static Player* loggedInPlayer();

        static void setCurrentRoute(QList<Edge*> edges);
        static QList<Edge*> currentRoute();
        static QList<Instruction> currentInstructions();
        static QList<InstructionVoicePrompt> voicePrompts();
        static int currentInstruction();
        static double blocksToNextInstruction();

        static void setFollowMe(bool follow);
        static bool followMe();

        static void setSelectedLandmark(Landmark* landmark);
        static Landmark* selectedLandmark();

        static void setNightMode(bool nightMode);
        static bool nightMode();

        static RouteOptions routeOptions();
        static void setRouteOption(RouteOption option, bool on = true);

    signals:
        void stateChanged(GlobalState state);
        void loginChanged(QString login);
        void followMeChanged(bool followMe);
        void routeChanged();
        void currentInstructionChanged();
        void selectedLandmarkChanged();
        void nightModeChanged();
        void routeOptionsChanged(RouteOptions options);

    private:
        explicit StateManager(QObject* parent = nullptr);
        StateManagerPrivate* d;

        void calculateCurrentInstruction();
};

#endif // STATEMANAGER_H
