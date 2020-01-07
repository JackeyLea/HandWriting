#-------------------------------------------------
# Project created by QtCreator 2019-06-03T19:20:56
# Qt 5.14.0
# GCC 9.2.0
# OpenCV 4.1.2
#-------------------------------------------------

QT       += core gui
QT       += widgets

TEMPLATE = app
CONFIG += c++11
DESTDIR = bin
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:{
INCLUDEPATH += /usr/include/opencv4/
LIBS += \
        -lopencv_core -lopencv_highgui -lopencv_videoio \
        -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml
}

win32:{
INCLUDEPATH += C:\Libraries\opencv\build\include

LIBS += -LC:\Libraries\opencv\build\x64\mingw\bin \
        -lopencv_core412 \
        -lopencv_dnn412 \
        -lopencv_highgui412 \
        -lopencv_imgcodecs412 \
        -lopencv_imgproc412 \
        -lopencv_ml412
}

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
