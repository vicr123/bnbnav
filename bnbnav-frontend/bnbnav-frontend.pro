QT       += core gui svg websockets

!wasm {
    QT += texttospeech
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datagatherer.cpp \
    datamanager.cpp \
    edge.cpp \
    instructionmodel.cpp \
    landmark.cpp \
    locationentrybox.cpp \
    loginwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    mapwidget.cpp \
    newlandmarkdialog.cpp \
    newroaddialog.cpp \
    node.cpp \
    nodeconnectdialog.cpp \
    player.cpp \
    road.cpp \
    splashwidget.cpp \
    statedialog.cpp \
    statemanager.cpp \
    texttospeechengine.cpp \
    topwidget.cpp \
    ttsengines/dummyttsengine.cpp

HEADERS += \
    datagatherer.h \
    datamanager.h \
    edge.h \
    instructionmodel.h \
    landmark.h \
    locationentrybox.h \
    loginwidget.h \
    mainwindow.h \
    mapwidget.h \
    newlandmarkdialog.h \
    newroaddialog.h \
    node.h \
    nodeconnectdialog.h \
    player.h \
    road.h \
    splashwidget.h \
    statedialog.h \
    statemanager.h \
    texttospeechengine.h \
    topwidget.h \
    ttsengines/dummyttsengine.h

FORMS += \
    loginwidget.ui \
    mainwindow.ui \
    newlandmarkdialog.ui \
    newroaddialog.ui \
    nodeconnectdialog.ui \
    splashwidget.ui \
    statedialog.ui \
    topwidget.ui

wasm {
    HEADERS += ttsengines/wasmttsengine.h
    SOURCES += ttsengines/wasmttsengine.cpp
} else {
    HEADERS += ttsengines/localttsengine.h
    SOURCES += ttsengines/localttsengine.cpp
}

DEFINES += BASE_URL=\\\"bnbnav.aircs.racing\\\"
DEFINES += WS_URL=\\\"wss://bnbnav.aircs.racing/ws\\\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
