QT       += gui
QT       += xml

QT       += core
TARGET = visionimageuitool
TEMPLATE = lib
CONFIG += plugin qt
CONFIG += uicommon


include(../uibuild.pri)
DESTDIR = $$NON_PLUGIN_LIB_PATH

win32:{
    # lib文件移到runtime4.0中
    WIN_BUILD_PATH = $$replace(BUILD_PATH,/,\\)
    LIB_FILE_BUILD = $$WIN_BUILD_PATH\\$${TARGET}.lib
    LIB_STORE_PATH = $$WIN_BUILD_PATH\\..\\..\\..\\libs\\win\\$$BUILD_TYPE$$OSBIT\\pluginlib
    MOVE_LIB = if exist $$LIB_FILE_BUILD move /y $$LIB_FILE_BUILD $$LIB_STORE_PATH
    QMAKE_POST_LINK = ($$MOVE_LIB) #拷贝多个文件,以&连接
}

HEADERS += \
    visionimagetool.h \
    ../../include/uitoolbox/visionimagetooldlg.h \
    graphwidget.h \
    imagewindow.h \
    graphscene.h \
    controlsdef.h \
    graphcontrols.h \
    shape/graphlineitem.h \
    shape/graphshapeitem.h \
    shape/graphpolyitem.h \
    shape/graphrectitem.h \
    shape/grapharcitem.h \
    shape/grapharccenteritem.h \
    shape/graphcircleitem.h \
    shape/graphcirclecenteritem.h \
    shape/grapheraseitem.h \
    shapemimedata.h \
    commands.h \
    pointalgorithm.h \
    shape/graphrectangle1item.h


SOURCES += \
    visionimagetool.cpp \
    visionimagetooldlg.cpp \
    graphwidget.cpp \
    imagewindow.cpp \
    graphscene.cpp \
    graphcontrols.cpp \
    shape/graphlineitem.cpp \
    shape/graphshapeitem.cpp \
    shape/graphpolyitem.cpp \
    shape/graphrectitem.cpp \
    shape/grapharcitem.cpp \
    shape/grapharccenteritem.cpp \
    shape/graphcircleitem.cpp \
    shape/graphcirclecenteritem.cpp \
    shape/grapheraseitem.cpp \
    shapemimedata.cpp \
    commands.cpp \
    shape/graphrectangle1item.cpp

FORMS += \
    imagewindow.ui 

TRANSLATIONS += en_us.ts \
               cn_zh.ts 

RESOURCES += \
    image.qrc


