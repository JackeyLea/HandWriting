#-------------------------------------------------
#
# Project created by QtCreator 2019-06-03T19:20:56
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = OpenCV-MNIST
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
DESTDIR = bin
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj

unix:{
INCLUDEPATH += /usr/include/opencv4/
LIBS += \
        -lopencv_core -lopencv_highgui -lopencv_videoio \
        -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        training.cpp

HEADERS += \
        mainwindow.h \
        training.h

FORMS += \
        mainwindow.ui
