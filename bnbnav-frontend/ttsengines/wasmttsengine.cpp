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
#include "wasmttsengine.h"

#include <emscripten.h>
#include <QLocale>

struct WasmTtsEnginePrivate {

};

namespace WasmTtsEngine_EM {
    EM_JS(void, prepareVoices, (), {
        let voices = window.speechSynthesis.getVoices();
    });

    EM_JS(bool, say, (const char* text, const char* lang), {
        text = UTF8ToString(text);
        lang = UTF8ToString(lang);
        let utterance = new SpeechSynthesisUtterance(text);
        utterance.lang = lang;
        let selectedVoice = null;
        for (let voice of window.speechSynthesis.getVoices()) {
            if (voice.lang.startsWith(lang.toLowerCase())) selectedVoice = voice;
        }
        if (selectedVoice) {
            utterance.voice = selectedVoice;
            window.speechSynthesis.speak(utterance);
            return true;
        } else {
            console.log("Language " + lang + " not supported");
            return false;
        }
    });
}

WasmTtsEngine::WasmTtsEngine(QObject* parent) : TextToSpeechEngine(parent) {
    d = new WasmTtsEnginePrivate();
//    ::say("", qPrintable(QLocale().bcp47Name().replace("_", "-")));
    WasmTtsEngine_EM::prepareVoices();
}

WasmTtsEngine::~WasmTtsEngine() {
    delete d;
}

#include <QDebug>
void WasmTtsEngine::say(QString text) {
    bool ok = WasmTtsEngine_EM::say(qPrintable(text), qPrintable(QLocale().bcp47Name().replace("_", "-")));
    if (!ok) {
        //TODO: make a sound or something
    }
    qDebug() << "said " << text;
}
