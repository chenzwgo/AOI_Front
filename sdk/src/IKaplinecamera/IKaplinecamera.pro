# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = IKaplinecamera
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = IKAP_LINE_RESOURCE_LIBRARY

IKAP_DEPEND_PATH = .


include (../sdkbuild.pri)

INCLUDEPATH += $$IKAP_DEPEND_PATH/include

SOURCES +=  resource.cpp \
            IKaplinecamera.cpp \
            IKapmanager.cpp     \
            SerialPortControl.cpp

HEADERS +=  resource.h \
            IKaplinecamera.h \
            IKapmanager.h   \
            SerialPortControl.h

LIBS += -L$$IKAP_DEPEND_PATH/Lib/Win$$OSBIT

# Load lib path and lib file
LIBS +=  \
        -lsdkcore   \
        -llog      \
        -ltinyxml   \
        -ljthread   \
        -lIKapBoard  

