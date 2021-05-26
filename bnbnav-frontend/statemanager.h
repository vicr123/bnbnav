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
struct StateManagerPrivate;
class StateManager : public QObject {
        Q_OBJECT
    public:
        enum GlobalState {
            Browse,
            Edit,
            Go
        };

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
                Merge
            };

            Node* node;
            Edge* fromEdge;
            Edge* toEdge;
            InstructionType type;
            double distance;
            double turnAngle;

            QString humanReadableString(int distance);

            int height();
            void render(QPainter* painter, QRect rect, QFont font, QPalette pal, int distance);
            QString imageName();
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
        static int currentInstruction();
        static double blocksToNextInstruction();

        static void setFollowMe(bool follow);
        static bool followMe();

    signals:
        void stateChanged(GlobalState state);
        void loginChanged(QString login);
        void followMeChanged(bool followMe);
        void routeChanged();
        void currentInstructionChanged();

    private:
        explicit StateManager(QObject* parent = nullptr);
        StateManagerPrivate* d;

        void calculateCurrentInstruction();
};

#endif // STATEMANAGER_H
