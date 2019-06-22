QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notilearn
TEMPLATE = app
CONFIG += c++14

SOURCES += main.cpp\
        mainwindow.cpp \
    addnewdialog.cpp

HEADERS  += mainwindow.h \
    addnewdialog.h

FORMS    += mainwindow.ui \
    addnewdialog.ui
