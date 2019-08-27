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

win32:{
INCLUDEPATH += $$PWD/OpenCV4.1.0/include

LIBS += -L$$PWD/OpenCV4.1.0/bin \
        -lopencv_core410 \
        -lopencv_dnn410 \
        -lopencv_highgui410 \
        -lopencv_imgcodecs410 \
        -lopencv_imgproc410 \
        -lopencv_ml410
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
        training.hpp

FORMS += \
        mainwindow.ui
