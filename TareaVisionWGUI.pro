#-------------------------------------------------
#
# Project created by QtCreator 2017-03-30T00:24:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TareaVisionWGUI
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        filters.cpp

HEADERS  += mainwindow.h \
    filters.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
