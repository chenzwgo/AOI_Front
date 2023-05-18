
#message($$PWD)
include(../../BJG12.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += $$RT_UI_INTERFACE \
            += $$RT_UI_INCLUDE  \
            += $$RT_SDK_INCLUDE   \
            += $$RT_SDK_INTERFACE \
            += $$RT_INCLUDE_PATH \
            += $$RT_COMMON_INCLUDE_PATH \
            += $$RT_UI_INCLUDE/customui \
            += $$RT_UI_INCLUDE/uitoolbox \
            += $$RT_UI_INCLUDE/uicommon

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -DRT_DBG_BUILD
    unix:QMAKE_LIBS += -Wl
    macx:CONFIG -= app_bundle
    TAEGET_SUFFIX=_d

} else {
    BUILD_TYPE=release
    # pre define used -D
    CONFIG += strip
}

#CONFIG += build_all warn_on



#QMAKE_CXXFLAGS += -fno-rtti -fno-exceptions -Wall

macx-g++ {
    message("got macx-g++")
    CONFIG (app) {     #message (Building app)
        QMAKE_LIBS += -L$$BUILD_PATH
    }
    CONFIG (plugin) {  #message (Building plugin $$TARGET)
        QMAKE_LIBS += -L$$BUILD_PATH -luicommon
    }
    CONFIG (uicommon) {     #message (Building core)
        QMAKE_LIBS += -L$$BUILD_PATH -luicontainer
    }
    CONFIG (uicontainer) {     #message (Building plugincontainer)
        QMAKE_LIBS += -L$$BUILD_PATH
    }
    CONFIG (sdk) {     #message (Building sdk)
        QMAKE_LIBS += -L$$BUILD_PATH
    }

    CONFIG -= app_bundle
 #   LIBS += -lstdc++
#    LIBS += -liconv
}

linux-g++{
    message("got linux-g++")
    DEFINES+=Q_OS_UNIX
    DEFINES+=Q_OS_LINUX
    DEFINES+=Q_WS_X11
    DEFINES+=_GLIBCXX_FULLY_DYNAMIC_STRING
    #QMAKE_CXXFLAGS += -Werror
    CONFIG (app) {     #message (Building app)
        QMAKE_LIBS += -Wl,-rpath,$$BUILD_PATH -L$$BUILD_PATH -luicommon
    }
    CONFIG (plugin) {  #message (Building plugin $$TARGET)
        QMAKE_LIBS += -Wl,-rpath,$$BUILD_PATH -L$$BUILD_PATH -luicommon
    }
    CONFIG (uicommon) {     #message (Building core)
        QMAKE_LIBS += -L$$BUILD_PATH -luicontainer
    }
    CONFIG (uicontainer) {     #message (Building plugincontainer)
        QMAKE_LIBS += -L$$BUILD_PATH
    }
    CONFIG (sdk) {     #message (Building sdk)
        QMAKE_LIBS += -L$$BUILD_PATH
    }
}

win32-g++ {
    message("got win32-g++")
    DEFINES += _WINDOWS

    CONFIG (app) {     #message (Building app)
        QMAKE_LIBS += -L$$BUILD_PATH
    }
    CONFIG (plugin) {  #message (Building plugin $$TARGET)
        QMAKE_LIBS += -L$$BUILD_PATH -luicommon
        CONFIG   += dll
        QMAKE_CXXFLAGS -= -DQT_NO_DYNAMIC_CAST
    }
    CONFIG (uicommon) {     #message (Building core)
        QMAKE_CXXFLAGS += -DRT_EXPORTS
    }
    CONFIG (uicontainer) {     #message (Building plugincontainer)
        QMAKE_CXXFLAGS += -DRT_EXPORTS
        QMAKE_LIBS += -L$$BUILD_PATH
        CONFIG   += dll
    }
    CONFIG (sdk) {     #message (Building sdk)
        QMAKE_CXXFLAGS += -DRT_EXPORTS
        QMAKE_LIBS += -L$$BUILD_PATH

    }
#    DEFINES += _WINDOWS
}

win32-msvc* {
    message("got win32-msvc")
    DEFINES += _WINDOWS
    CONFIG -= embed_manifest_dll
    CONFIG += embed_manifest_exe
    QMAKE_CXXFLAGS += /Wall #/WX


#    CONFIG(debug, debug|release) {
#        exists($$RT_BASE_PATH/include/vld.h) {
#            message( "enabling memory leak detection..." )
#            QMAKE_LIBS += -L$$BUILD_PATH -lvld
#            QMAKE_CXXFLAGS += -DRT_CHECK_MEM_LEAKS
#        }
#    }

    CONFIG (app) {     #message (Building app)
        QMAKE_LIBS += -L$$BUILD_PATH
    }
    CONFIG (plugin) {  #message (Building plugin $$TARGET)
        QMAKE_LIBS += -L$$BUILD_PATH -luicommon
        CONFIG   += dll
    }
    CONFIG (uicommon) {     #message (Building core)
        QMAKE_CXXFLAGS += -DRT_EXPORTS
        CONFIG   += dll
        DEFINES += RT_EXPORTS
    }
    CONFIG (uicontainer) {     #message (Building plugincontainer)
        QMAKE_CXXFLAGS += -DRT_EXPORTS
        QMAKE_LIBS += -L$$BUILD_PATH
        CONFIG   += dll
    }
    CONFIG (sdk) {     #message (Building sdk)
        QMAKE_CXXFLAGS += -DRT_EXPORTS
        QMAKE_LIBS += -L$$BUILD_PATH
        CONFIG   += dll
    }

}


win32: {
    MOC_DIR = $$RT_UI_PATH/src/$$TARGET/moc
    OBJECTS_DIR = $$RT_UI_PATH/src/$$TARGET/obj
}

unix: {
    MOC_DIR = $$RT_UI_PATH/src/$$TARGET/moc
    OBJECTS_DIR = $$RT_UI_PATH/src/$$TARGET/obj
}

macx: {
    MOC_DIR = $$RT_UI_PATH/src/$$TARGET/ui/moc
    OBJECTS_DIR = $$RT_UI_PATH/src/$$TARGET/ui/obj
}


