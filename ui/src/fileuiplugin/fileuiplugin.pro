#-------------------------------------------------
#
# Project created by QtCreator 2017-01-29T00:20:58
#
#-------------------------------------------------
QT       += gui
TARGET = fileuiplugin
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_FILEUIPLUGIN


include(../uibuild.pri)

PLUGIN_PATH = $$PWD

HEADERS += \
    $$PLUGIN_PATH/fileuiplugin.h    \
    programdialog/cprogramdialog.h  \
    fileuimanager.h \
    lostfileshowdlg.h \
    openfiledlg.h \
    fileuicommon.h


SOURCES += \
    $$PLUGIN_PATH/fileuiplugin.cpp  \
    programdialog/cprogramdialog.cpp    \
    $$PLUGIN_PATH/fileuimanager.cpp \
    lostfileshowdlg.cpp \
    openfiledlg.cpp \
    fileuicommon.cpp


LIBS += -L$${DESTDIR}   \
        -luicommon      \
        -ljthread \
        -lg12common

FORMS += \
    newfiledlg.ui \
    programdialog/cprogramdialog.ui \
    lostfileshowdlg.ui \
    openfiledlg.ui

RESOURCES += \
    image.qrc
