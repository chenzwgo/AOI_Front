# -------------------------------------------------
# Project created by QtCreator 2009-04-16T20:45:03
# -------------------------------------------------
QT -= gui core
TARGET = sdkcommon
TEMPLATE = lib
CONFIG += exportlib
DEFINES = SDKCOMMON_RESOURCE_LIBRARY
CONFIG   += dll
include (../sdkbuild.pri)

PLUGINPATH = $$PWD

HEADERS += \
            ./socket/udpsocket.h \
            ./socket/udpsocketmanager.h \
            ./socket/socketclient.h \
            ./socket/socketclientmanager.h \
            ./socket/socketservermanager.h \
            ./socket/socketserver.h \
            ./notificationcenter/notificationcenter.h \
            ./com/comcontrol.h \
            ./com/commanager.h \
            ./modbusport/modbusport.h \
            ./modbusport/modbusmanager.h    \
            ./errorcode/parseerrormessage.h \
			./reportcenter/reportcenter.h	\
            ./resource.h
SOURCES +=  \
 #           ./sdkcommon.cpp \
            ./socket/udpsocket.cpp \
            ./socket/udpsocketmanager.cpp \
            ./socket/socketclient.cpp \
            ./socket/socketclientmanager.cpp \
            ./socket/socketservermanager.cpp \
            ./socket/socketserver.cpp \
            ./notificationcenter/notificationcenter.cpp \
            ./com/commanager.cpp \
            ./modbusport/modbusport.cpp \
            ./modbusport/modbusmanager.cpp    \
            ./errorcode/parseerrormessage.cpp   \			
			./reportcenter/reportcenter.cpp	\
            ./resource.cpp


win32 :{
    SOURCES += ./com/comcontrol_win.cpp
    LIBS += -lws2_32
}


macx:{
    SOURCES += ./com/comcontrol_unix.cpp
}


LIBS += -lpublicfunction  \
        -ljthread   \
        -ltinyxml	\
		-lsdkcore



