QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = aquaBackend
CONFIG += console
CONFIG -= app_bundle

unix {
# So wiringPi include files can be found during compile
INCLUDEPATH += /usr/local/include

# To link the wiringPi library when making the executable
LIBS += -L/usr/local/lib -lwiringPi
}

TEMPLATE = app

SOURCES += main.cpp \
    BackendServer.cpp \
    RelayControl.cpp \
    TemperatureSensor.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    BackendServer.h \
    RelayControl.h \
    TemperatureSensor.h
