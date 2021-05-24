QT       += core gui svg websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    datagatherer.cpp \
    datamanager.cpp \
    edge.cpp \
    main.cpp \
    mainwindow.cpp \
    mapwidget.cpp \
    newroaddialog.cpp \
    node.cpp \
    nodeconnectdialog.cpp \
    road.cpp \
    statemanager.cpp \
    topwidget.cpp

HEADERS += \
    datagatherer.h \
    datamanager.h \
    edge.h \
    mainwindow.h \
    mapwidget.h \
    newroaddialog.h \
    node.h \
    nodeconnectdialog.h \
    road.h \
    statemanager.h \
    topwidget.h

FORMS += \
    mainwindow.ui \
    newroaddialog.ui \
    nodeconnectdialog.ui \
    topwidget.ui

DEFINES += BASE_URL=\\\"localhost:4000\\\"
DEFINES += WS_URL=\\\"wss://bnbnav.aircs.racing/ws\\\"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
