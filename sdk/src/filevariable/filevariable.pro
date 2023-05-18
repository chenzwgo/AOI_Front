# -------------------------------------------------
# Project created by QtCreator 2009-04-16T20:45:03
# -------------------------------------------------
QT -= gui core
TARGET = filevariable
TEMPLATE = lib
CONFIG += exportlib
DEFINES += FILEVARIABLE_RESOURCE_LIBRARY
CONFIG   += dll
include (../sdkbuild.pri)
PLUGINPATH = $$PWD

HEADERS += \
            ./filevariable/filevariable.h   \
#            ./filevariable/filepathvariable.h   \
            $${RT_SDK_INTERFACE}/filevariable/cvariantvalue.h   \
            ./filevariable/iniFile.h   \
            ./resource.h
SOURCES +=  \
            ./filevariable/filevariable.cpp   \
#            ./filevariable/filepathvariable.cpp   \
            ./filevariable/iniFile.cpp   \
            ./resource.cpp


LIBS += -L$${RT_PLUGIN_LIB_PATH}   \
        -lpublicfunction  \
        -ljthread


