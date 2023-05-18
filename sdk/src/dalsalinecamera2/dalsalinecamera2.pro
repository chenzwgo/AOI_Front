# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = dalsalinecamera2
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = DALSA_LINE_RESOURCE_LIBRARY

#DALSA_DEPEND_PATH = $(SAPERADIR)
DALSA_DEPEND_PATH = .

DALSA_LINE_BASIC_INCLUDE = $$DALSA_DEPEND_PATH/Classes/Basic

include (../sdkbuild.pri)

INCLUDEPATH += ./inc
INCLUDEPATH += $$DALSA_DEPEND_PATH/Classes/Basic
INCLUDEPATH += $$DALSA_DEPEND_PATH/include

SOURCES +=  resource.cpp \
            dalsamanager.cpp \
            dalsalinecamera.cpp \
            device/dalsalinecameradevice.cpp    \
            dalsalineGigEcamera.cpp \
            device/dalsalineGigEcameradevice.cpp

HEADERS +=  resource.h \
            dalsamanager.h \
            dalsalinecamera.h \
            device/dalsalinecameradevice.h    \
            dalsalineGigEcamera.h \
            device/dalsalineGigEcameradevice.h


LIBS += -L$$DALSA_DEPEND_PATH/Lib/Win$$OSBIT

# Load lib path and lib file
LIBS += -L$$NON_PLUGIN_LIB_PATH  \
        -lsdkcore   \
        -llog      \
        -ltinyxml   \
        -ljthread   \
        -lSapClassBasic  

