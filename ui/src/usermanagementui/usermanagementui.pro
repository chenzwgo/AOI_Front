QT       += gui
QT       += xml

TARGET = usermanagementui
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_USERMANAGEMENTUI
include(../uibuild.pri)


HEADERS += \
    usermanagementui.h \
    usermanagementuidlg.h \
    usermanagercfg.h \
    competencesetdlg.h \
    usersetdlg.h


SOURCES += \
    usermanagementui.cpp \
    usermanagementuidlg.cpp \
    usermanagercfg.cpp \
    competencesetdlg.cpp \
    usersetdlg.cpp

FORMS += \
    usermanagementuidlg.ui \
    competencesetdlg.ui \
    usersetdlg.ui

TRANSLATIONS += en_us.ts \
               cn_zh.ts 


LIBS += -L$$DESTDIR  \
#        -luicommon \
        -luitoolbox \

RESOURCES += \
    image.qrc

