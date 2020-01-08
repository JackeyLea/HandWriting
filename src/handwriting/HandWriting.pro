#-------------------------------------------------
#
# Project created by QtCreator 2019-08-24T03:39:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HandWriting
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

win32:{
INCLUDEPATH += C:\Libraries\opencv\build\include

LIBS += -LC:\Libraries\opencv\build\x64\mingw\bin \
        -lopencv_core411 \
        -lopencv_dnn411 \
        -lopencv_highgui411 \
        -lopencv_imgcodecs411 \
        -lopencv_imgproc411 \
        -lopencv_ml411
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        base.hpp \
        mainwindow.h

FORMS += \
        mainwindow.ui


RESOURCES += \
    resources.qrc

ICON += \
    icon.rc

RC_FILE += \
    icon.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
