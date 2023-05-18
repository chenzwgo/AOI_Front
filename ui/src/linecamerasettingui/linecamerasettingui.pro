QT       += gui
QT       += xml

TARGET = linecamerasettingui
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += LINE_CAMERA_SETTING_PLUGIN


include(../uibuild.pri)

DEFINES -= QT_NO_DYNAMIC_CAST

HEADERS += \
    camerasettingui.h \
    linecameramanagerui.h \
	customitemdelegate.h \ 
    clinecameralist.h


SOURCES += \
    camerasettingui.cpp \
    linecameramanagerui.cpp \
	customitemdelegate.cpp \ 
    clinecameralist.cpp

FORMS += \
    linecameramanagerui.ui \
    clinecameralist.ui

TRANSLATIONS += en_us.ts \
               cn_zh.ts 


LIBS += -L$${DESTDIR}   \
        -luicommon \
        -lcallsdk \
        -lsdkproxy \
        -ljthread \
        -lvisionimageuitool \
        -lg12common

