#-------------------------------------------------
#
# Project created by QtCreator 2010-11-16T22:28:31
#
#-------------------------------------------------

QT  += gui core
QT += printsupport
TARGET = uicommon
TEMPLATE = lib
CONFIG += uicommon
CONFIG   += dll
DEFINES += UI_UICOMMON

include(../uibuild.pri)
DESTDIR = $$BUILD_PATH

SOURCES +=   \
            $$PWD/commonutils.cpp \
            $$PWD/ExtendDialog/ExtendDialog.cpp \
            $$PWD/titlebar/titlebar.cpp \
            $$PWD/change/change.cpp    \       	
            $$PWD/ExtendStackWidget/extendstackwidget.cpp	\
            $$PWD/ExtendStackWidget/tool_button.cpp	\
            $$PWD/curvegraph/qcustomplot.cpp	\
            $$PWD/curvegraph/curvegraph.cpp \
            $$PWD/image/cimageprocess.cpp

HEADERS +=  \
            $$PWD/../../include/uicommon/commonutils.h \
            $$PWD/../../include/uicommon/ExtendDialog.h \
            $$PWD/../../include/uicommon/titlebar.h  \
            $$PWD/../../include/uicommon/change.h  \       	
            $$PWD/../../include/uicommon/extendstackwidget.h	\
            $$PWD/../../include/uicommon/tool_button.h	\
            $$PWD/../../include/uicommon/curvegraphwidget.h	\
            $$PWD/curvegraph/qcustomplot.h \
            $$PWD/../../include/uicommon/cimageprocess.h


FORMS += \
    $$PWD/curvegraph/curvegraph.ui


