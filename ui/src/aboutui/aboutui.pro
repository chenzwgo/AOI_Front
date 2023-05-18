#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T00:20:58
#
#-------------------------------------------------

QT       += xml

TARGET = aboutui
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_ABOUTUI
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../uibuild.pri)

PLUGIN_PATH = $$PWD

HEADERS += \
    $$PLUGIN_PATH/aboutuiservice.h \
    $$PLUGIN_PATH/aboutuiplugin.h \
    $$PLUGIN_PATH/aboutdlg.h


SOURCES += \
    $$PLUGIN_PATH/aboutuiservice.cpp \
    $$PLUGIN_PATH/aboutuiplugin.cpp \
    $$PLUGIN_PATH/aboutdlg.cpp

FORMS += \
    $$PLUGIN_PATH/aboutdlg.ui

TRANSLATIONS += $$PLUGIN_PATH/en_us.ts \
               $$PLUGIN_PATH/cn_zh.ts


LIBS += -L$${DESTDIR}   \
        -luicommon
