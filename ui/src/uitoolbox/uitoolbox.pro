#-------------------------------------------------
#
# Project created by QtCreator 2010-11-16T22:28:31
#
#-------------------------------------------------

QT  += gui core
QT  += network

TARGET = uitoolbox
TEMPLATE = lib
QT       += xml
QT       += network
CONFIG += uicommon
CONFIG += dll
DEFINES += UI_UITOOLBOX

include(../uibuild.pri)
DESTDIR = $$BUILD_PATH

SOURCES +=   \
            $$PWD/qhttpclientimp.cpp \
            $$PWD/keyvaluebox.cpp \
            $$PWD/datatable.cpp \
            $$PWD/specbox.cpp \
            $$PWD/displaylogwidget.cpp   \
            $$PWD/qhardwareinfo.cpp \
            $$PWD/showpicturewidget.cpp \
            $$PWD/msgbox.cpp

HEADERS +=  \
            $$PWD/../../include/uitoolbox/qhttpclientimp.h \
            $$PWD/../../include/uitoolbox/keyvaluebox.h \
            $$PWD/../../include/uitoolbox/datatable.h \
            $$PWD/../../include/uitoolbox/specbox.h \
            $$PWD/../../include/uitoolbox/displaylogwidget.h \
            $$PWD/../../include/uitoolbox/qhardwareinfo.h \
            $$PWD/../../include/uitoolbox/showpicturewidget.h \
            $$PWD/../../include/uitoolbox/msgbox.h

win32 {
    HEADERS += $$PWD/../../include/uitoolbox/qexcel.h
    SOURCES += $$PWD/qexcel.cpp
    CONFIG += qaxcontainer
}

LIBS += -L$${DESTDIR}   \
        -luicommon

