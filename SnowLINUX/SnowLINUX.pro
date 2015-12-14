#-------------------------------------------------
#
# Project created by QtCreator 2015-12-11T21:43:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SnowLINUX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newdialog.cpp \
    missionbar.cpp \
    global.cpp \
    missioncheck.cpp \
    global_f.cpp

HEADERS  += mainwindow.h \
    newdialog.h \
    missionbar.h \
    global.h \
    global_t.h \
    global_f.h \
    missioncheck.h

FORMS    += mainwindow.ui \
    newDlg.ui \
    missionBar.ui
