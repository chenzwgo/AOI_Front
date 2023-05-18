# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui
TARGET = hiksourcecamera
TEMPLATE = lib
DEFINES = HIKINGSOURCECAMERA_RESOURCE_LIBRARY

include (../sdkbuild.pri)
#DESTDIR = $$PLUGIN_LIB_PATH/$${TARGET}

win32:{
INCLUDEPATH += include
}
INCLUDEPATH += $${RT_INCLUDE_PATH}
INCLUDEPATH += $${SDK_INCLUDE_PATH}

SOURCES +=  ./hiksourcecamera.cpp \
            ./hiksourcecameramanager.cpp \
            ./mycamera.cpp\
            ./resource.cpp
           
HEADERS +=  ./hiksourcecamera.h \
            ./hiksourcecameramanager.h \
            ./mycamera.h\
            ./resource.h \
    include/CameraParams.h \
    include/MvCameraControl.h \
    include/MvErrorDefine.h

# UI needed    
#FORMS += ./

# UI needed  
#RESOURCES += ./    


# Load lib path and lib file
win32:{
    LIBS += -Llib
}

TTT = $$PWD/lib
LIBS += -L$$NON_PLUGIN_LIB_PATH   \
        -ljthread
LIBS += -l$$PWD/lib/win64/MvCameraControl

#CONFIG(release, debug|release)
#LIBS += -L../../bin/release \
#    -lTrace
#LIBS += -L../../bin/debug \
#    -lJThread
#CONFIG(release, debug|release)
#LIBS += -L../../bin/release \
#    -lJThread

