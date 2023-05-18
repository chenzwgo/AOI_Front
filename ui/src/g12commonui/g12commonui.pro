#-------------------------------------------------
#
# Project created by QtCreator 2010-11-16T22:28:31
#
#-------------------------------------------------

QT  += gui core
TARGET = g12commonui
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_G12COMMON

include(../uibuild.pri)
DEFINES -= QT_NO_DYNAMIC_CAST

INCLUDEPATH += $$RT_UI_INCLUDE/g12commonui

SOURCES +=   \
            $$PWD/g12commonuiplugin.cpp \
            $$PWD/record/cdefectrecord.cpp 

HEADERS +=  \
            $$PWD/g12commonuiplugin.h \
            $$PWD/record/cdefectrecord.h 


FORMS += \
    $$PWD/record/cdefectrecord.ui


LIBS += -L$$DESTDIR  \
        -ljthread \
        -luicommon	\
        -lg12common  \
        -lvisionimageuitool 
