#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T00:20:58
#
#-------------------------------------------------

QT       += gui
TARGET = customsettingplugin
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += CUSTOM_SETTING_PLUGIN

include(../uibuild.pri)

PLUGIN_PATH = $$PWD

HEADERS += \
    $$PLUGIN_PATH/customsettingplugin.h \
    $$PLUGIN_PATH/customui.h \
    $$PLUGIN_PATH/configui/configview.h \
    $$PLUGIN_PATH/configui/usersetting.h \
    $$PLUGIN_PATH/configui/settingdefine.h  \ 
    $$PLUGIN_PATH/configui/configoper.h  \
    $$PLUGIN_PATH/debugui/debugview.h \
#    $$PLUGIN_PATH/modbuscom.h \
#    $$PLUGIN_PATH/illuminationcontroldlg.h \
#    imagecalibprocess.h \
    videoscope.h \
    imageviewer.h


SOURCES += \
    $$PLUGIN_PATH/customsettingplugin.cpp \
    $$PLUGIN_PATH/customui.cpp \
    $$PLUGIN_PATH/configui/configview.cpp \
    $$PLUGIN_PATH/configui/usersetting.cpp \
    $$PLUGIN_PATH/configui/configoper.cpp  \
    $$PLUGIN_PATH/debugui/debugview.cpp \
#    $$PLUGIN_PATH/modbuscom.CPP \
#    $$PLUGIN_PATH/illuminationcontroldlg.cpp \
#    imagecalibprocess.cpp \
    videoscope.cpp \
    imageviewer.cpp

FORMS += \
    $$PLUGIN_PATH/configui/usersetting.ui \
    $$PLUGIN_PATH/configui/configview.ui    \
    $$PLUGIN_PATH/debugui/debugview.ui \
#    illchannelform.ui \
    cameractrlform.ui \
#    inilight.ui \
#    imagecalibprocess.ui \
    videoscope.ui \
    imageviewer.ui

TRANSLATIONS += $$PLUGIN_PATH/en_us.ts \
               $$PLUGIN_PATH/cn_zh.ts


LIBS += -L$${DESTDIR}   \
        -luicommon      \
        -luicustomwidget \
        -lcustomwidgetui    \
        -lcallsdk   \
        -lg12common \
        -ljthread \
		-lvisionimageuitool

RESOURCES += \
    image.qrc
