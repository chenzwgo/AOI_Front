QT       += gui
QT       += xml
QT += network

TARGET = netdevui
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_NETDEVUI
DEFINES += Q_OS_WIN
DEFINES += MAINUI_SEND_PC


include(../uibuild.pri)
INCLUDEPATH +=  $$RT_INCLUDE_PATH/netdevui

HEADERS += \
    $$PWD/devclient/socketproxy.h \
    $$PWD/devclient/socketclient.h  \
    $$RT_INCLUDE_PATH/netdevui/socketcommon.h \
    $$PWD/devclient/socketlistcfg.h   \
    $$PWD/devclient/devclientui.h \
    $$PWD/devclient/socketlistui.h \
    $$PWD/devclient/clientinfodlg.h \
    $$PWD/devclient/communicationtestform.h


SOURCES += \
    $$PWD/devclient/socketproxy.cpp \
    $$PWD/devclient/socketclient.cpp  \
    $$PWD/devclient/socketcommon.cpp \
    $$PWD/devclient/socketlistcfg.cpp   \
    $$PWD/devclient/devclientui.cpp \
    $$PWD/devclient/socketlistui.cpp \
    $$PWD/devclient/clientinfodlg.cpp \
    $$PWD/devclient/communicationtestform.cpp

FORMS += \
    $$PWD/devclient/socketlistui.ui \
    $$PWD/devclient/clientinfodlg.ui \
    $$PWD/devclient/communicationtestform.ui


RESOURCES += \
    $$PWD/devclient/socketclientuiimage.qrc


LIBS +=   \
        -lcallsdk \
        -ljthread \
        -luitoolbox \
        -lsdkproxy \
        -llog  \
        -ltinyxml   \
        -luicommon
