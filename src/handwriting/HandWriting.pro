#-------------------------------------------------
# Project created by QtCreator 2019-08-24T03:39:53
#-------------------------------------------------

QT       += core gui
QT       += widgets

TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR = tmp/ui
DESTDIR = bin

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

unix:{
INCLUDEPATH += /usr/include/opencv4

LIBS += -L/usr/lib \
        -lopencv_core \
        -lopencv_dnn \
        -lopencv_highgui \
        -lopencv_imgcodecs \
        -lopencv_imgproc \
        -lopencv_ml
}

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
