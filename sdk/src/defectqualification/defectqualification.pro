# -------------------------------------------------
# Project created by QtCreator 2009-04-16T20:45:03
# -------------------------------------------------
QT -= gui core
TARGET = defectqualification
TEMPLATE = lib
CONFIG += exportlib
DEFINES += DEFECT_QUALIFICATION_RESOURCE_LIBRARY
CONFIG   += dll
include (../sdkbuild.pri)
PLUGINPATH = $$PWD

HEADERS += \
            ./resource.h \
    defectqualification.h \
    idefectlist.h \
    defectlist.h
SOURCES +=  \
            ./resource.cpp \
    defectqualification.cpp \
    defectlist.cpp


LIBS += -L$${RT_PLUGIN_LIB_PATH}   \
        -lpublicfunction   \
        -llog      \
        -ltinyxml   \
	-ljthread   \
		-lg12common


