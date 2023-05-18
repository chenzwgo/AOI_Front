# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = cardgg
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = CARD_GG_RESOURCE_LIBRARY

include (../sdkbuild.pri)

SOURCES += ./cardmanagerservice.cpp \
           ./dllmain.cpp \
           ./gts_common.cpp \
           ./resource.cpp \
           ./axis.cpp \
            card.cpp \
            io.cpp \
            ioext.cpp
           
HEADERS +=./cardmanagerservice.h \
           ./gts_common.h \
           ./resource.h \
           ./axis.h \
            card.h \
            io.h \
            ioext.h

# Load lib path and lib file
LIBS += -lsdkcore         \
        -lgts             \
        -llog             \
        -lpublicfunction  \
        -ljthread         \
        -ltinyxml

#CONFIG(release, debug|release)
#LIBS += -L../../bin/release \
#    -lTrace
#LIBS += -L../../bin/debug \
#    -lJThread
#CONFIG(release, debug|release)
#LIBS += -L../../bin/release \
#    -lJThread
