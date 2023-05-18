
include(../../BJG12.pri)

CONFIG += debug warn_on

INCLUDEPATH += $${RT_SDK_INCLUDE}
INCLUDEPATH += $${RT_SDK_INTERFACE}
INCLUDEPATH += $${RT_INCLUDE_PATH}
INCLUDEPATH += $${RT_COMMON_INCLUDE_PATH}


CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -DRT_DBG_BUILD=1
    unix:QMAKE_LIBS += -Wl
    macx:CONFIG -= app_bundle
    TAEGET_SUFFIX=_d

} else {
    # pre define used -D
    QMAKE_CXXFLAGS += -DRT_REL_BUILD=1
    CONFIG += strip
}

# checks for memory leaks  
win32{
DEFINES += _WINDOWS
DEFINES += Q_OS_WIN
CONFIG (exportlib) {
    QMAKE_CXXFLAGS += -DRT_EXPORTS
    }
}


linux-g++{
DEFINES+=Q_OS_UNIX
DEFINES+=Q_OS_LINUX
DEFINES+=Q_WS_X11 
DEFINES+=_GLIBCXX_FULLY_DYNAMIC_STRING 

}
mac{
#    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
 #   CONFIG += x86 ppc
 #   QMAKE_CXXFLAGS += -mmacosx-version-min=10.4
#    QMAKE_LIBS += -mmacosx-version-min=10.4
#    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    DEFINES+=Q_OS_UNIX
    DEFINES+=Q_OS_DARWIN
 LIBS += -liconv
 LIBS += -lstdc++

}

win32: {
    MOC_DIR = $$RT_SDK_PATH/src/$$TARGET/moc
    OBJECTS_DIR = $$RT_SDK_PATH/src/$$TARGET/obj
}

unix: {
    MOC_DIR = $$RT_SDK_PATH/src/$$TARGET/moc
    OBJECTS_DIR = $$RT_SDK_PATH/src/$$TARGET/obj
}

macx: {
    MOC_DIR = $$RT_SDK_PATH/src/$$TARGET/moc
    OBJECTS_DIR = $$RT_SDK_PATH/src/$$TARGET/obj
}
