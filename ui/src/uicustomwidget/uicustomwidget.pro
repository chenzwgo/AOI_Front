#-------------------------------------------------
#
# Project created by QtCreator 2010-11-16T22:28:31
#
#-------------------------------------------------

QT  += gui core

TARGET = uicustomwidget
TEMPLATE = lib
CONFIG += dll
DEFINES += UI_CUSTOMWIDGET

include(../uibuild.pri)
DESTDIR = $$BUILD_PATH
LIBS += -L$${DESTDIR}   \
        -lcallsdk  \
        -lcustomwidgetui

RT_UI_CUSTOM_INCLUDE=$$RT_UI_PATH/include/customui
HEADERS += \
    $$RT_UI_CUSTOM_INCLUDE/axisstatus.h \
    $$RT_UI_CUSTOM_INCLUDE/customwidget.h \
    $$RT_UI_CUSTOM_INCLUDE/qcylinder.h \
    $$RT_UI_CUSTOM_INCLUDE/motionctldlg.h \
    $$RT_UI_CUSTOM_INCLUDE/ioinput.h \
    $$RT_UI_CUSTOM_INCLUDE/iooutput.h \
    $$RT_UI_CUSTOM_INCLUDE/movingdlg.h \
    $$RT_UI_CUSTOM_INCLUDE/runflowdlg.h \
    iolabel.h \
    addpointdialog.h \
    TextPushButton.h

SOURCES += \
    customwidget.cpp \
    motionctldlg.cpp \
    movingdlg.cpp \
    runflowdlg.cpp \
    iolabel.cpp \
    addpointdialog.cpp
    
FORMS += \
    motionctldlg.ui \
    movingdlg.ui \
    runflowdlg.ui \
    addpointdialog.ui
    
RESOURCES += \
    image.qrc

