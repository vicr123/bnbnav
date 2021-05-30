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
#ifndef LOCALTTSENGINE_H
#define LOCALTTSENGINE_H

#include "texttospeechengine.h"

struct LocalTtsEnginePrivate;
class LocalTtsEngine : public TextToSpeechEngine {
        Q_OBJECT
    public:
        explicit LocalTtsEngine(QObject* parent = nullptr);
        ~LocalTtsEngine();

    signals:

    private:
        LocalTtsEnginePrivate* d;

        // TextToSpeechEngine interface
    public:
        void say(QString text);
};

#endif // LOCALTTSENGINE_H
