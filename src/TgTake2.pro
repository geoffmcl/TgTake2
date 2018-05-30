#-------------------------------------------------
#
# Project created by QtCreator 2011-08-28T12:26:27
#
#-------------------------------------------------

QT       += core gui

TARGET = TgTake2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    utilities/utilities.cpp \
    tabs/tabsetup.cpp \
    tabs/tabelevations.cpp \
    tabs/tablanduse.cpp \
    tabs/tabairports.cpp \
    tabs/tabconstruction.cpp \
    tabs/tabadvanced.cpp \
    utilities/srtm_utils.cpp \
    utilities/srtm_data.cpp \
    utilities/newbucket.cpp \
    utilities/testExe.cpp \
    utilities/workThread.cpp \
    utilities/tg_utils.cpp

HEADERS  += mainwindow.h \
    app_config.h \
    utilities/utilities.h \
    tabs/tabsetup.h \
    tabs/tabelevations.h \
    tabs/tablanduse.h \
    tabs/tabairports.h \
    tabs/tabconstruction.h \
    tabs/tabadvanced.h \
    settings.h \
    utilities/srtm_data.h \
    utilities/srtm_utils.h \
    utilities/newbucket.h \
    utilities/testExe.h \
    utilities/workThread.h \
    utilities/tg_utils.h \
    utilities/app_macros.h
