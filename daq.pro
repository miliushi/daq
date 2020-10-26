QT += core network
QT += core gui
CONFIG += c++11

TARGET = daq
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


INCLUDEPATH += ../lib/libssh-0.7.5/include/
INCLUDEPATH += /usr/include/libftdi1/
LIBS += ../lib/build-libssh-0.7.5-Desktop-Release/src/libssh.so
LIBS += -lz -lcrypto -lftdi1

SOURCES += main.cpp \
    mainthread.cpp \
    spindaq.cpp \
    remoteqbus.cpp \
    qsshsocket.cpp \
    le88.cpp \
    qbusline.cpp \
    qbusmodule.cpp \
    qbuscontroller.cpp \
    modulet162.cpp \
    extio.cpp \
    extiothread.cpp \
    dataclient.cpp \
    modulecounter8.cpp \
    modulebrrpk32.cpp \
    modulele35.cpp \
    modulekb003.cpp \
    moduletl2.cpp \
    modules335.cpp

HEADERS += \
    mainthread.h \
    spindaq.h \
    remoteqbus.h \
    qsshsocket.h \
    le88def.h \
    le88.h \
    camacdef.h \
    qbusline.h \
    qbusmodule.h \
    qbuscontroller.h \
    modulet162.h \
    extio.h \
    extiothread.h \
    dataclient.h \
    modulecounter8.h \
    qbus.h \
    modulebrrpk32.h \
    modulele35.h \
    modulekb003.h \
    moduletl2.h \
    modules335.h

FORMS +=
