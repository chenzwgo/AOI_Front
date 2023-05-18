# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = cameramanager
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = CAMERAMANAGERPLUGIN_RESOURCE_LIBRARY

include (../sdkbuild.pri)

SOURCES +=   \
            linecammanagerplugin.cpp \
            resource.cpp \
            buffermanager.cpp   \
            imageProcess.cpp

           
HEADERS +=  \
            cameramanagererror.h \
			linecammanagerplugin.h \
            resource.h \
            buffermanager.h \
            imageProcess.h

# UI needed    
#FORMS += ./

# UI needed  
#RESOURCES += ./    


# Load lib path and lib file
LIBS += -L$$NON_PLUGIN_LIB_PATH  \
        -lsdkcore   \
        -llog      \
        -ltinyxml   \
        -lg12common   \
        -ljthread

