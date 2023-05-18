# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = cardg12modbus
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = CARD_G12MODBUS_RESOURCE_LIBRARY

include (../sdkbuild.pri)

SOURCES += ./cardmanagerservice.cpp \
           ./dllmain.cpp \
           ./resource.cpp \
           ./axis.cpp \
            card.cpp \
            io.cpp \
            ioext.cpp \
    converymodbus.cpp
           
HEADERS +=./cardmanagerservice.h \
           ./resource.h \
           ./axis.h \
            card.h \
            io.h \
            ioext.h \
    converymodbus.h

# Load lib path and lib file
LIBS += -lsdkcore         \
        -llog             \
        -lpublicfunction  \
        -ljthread         \
        -ltinyxml         \
        -lg12common
