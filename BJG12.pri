RT_BASE_PATH=$$PWD
RT_INCLUDE_PATH=$$PWD/include
RT_COMMON_INCLUDE_PATH=$$PWD/include/common
RT_BIN_PATH=$$PWD/bin
RT_UI_PATH=$$PWD/ui
RT_SDK_PATH=$$PWD/sdk
RT_SDK_INCLUDE=$$RT_SDK_PATH/include
RT_SDK_INTERFACE=$$RT_SDK_PATH/interface
RT_UI_INCLUDE=$$RT_UI_PATH/include
RT_UI_INTERFACE=$$RT_UI_PATH/interface

OSBIT=64
OSPLATFORM = $$(PLATFORM)
!isEmpty(OSPLATFORM){
    OSBIT=64
}

CONFIG += debug
CONFIG(debug, debug|release) {
    BUILD_TYPE=debug
} else {
    BUILD_TYPE=release
}

win32:{
    BUILD_PATH = $$RT_BIN_PATH/win/$$BUILD_TYPE$$OSBIT
    DEFINES +=  _CRT_SECURE_NO_WARNINGS
    exists($${RT_INCLUDE_PATH}/version_info.rc) {
            RC_FILE += $${RT_INCLUDE_PATH}/version_info.rc
           }

    # lib文件从debug32中移出
    RT_LIB_PATH=$$PWD/libs/win/$$BUILD_TYPE$$OSBIT
    RT_PLUGIN_LIB_PATH=$$RT_LIB_PATH/pluginlib
    RT_THIRDPARTLIB_LIB_PATH=$$RT_LIB_PATH/thirdpartlib
    RT_VISIONLIB_LIB_PATH=$$RT_LIB_PATH/visionlib

    WIN_BUILD_PATH = $$replace(BUILD_PATH,/,\\)
    LIB_FILE_BUILD = $$WIN_BUILD_PATH\\pluginlib\\*.lib
    LIB_FILE_BUILD_NO_PLUGIN = $$WIN_BUILD_PATH\\*.lib
    LIB_STORE_PATH = $$replace(RT_PLUGIN_LIB_PATH,/,\\)
    MOVE_LIB = if exist $$LIB_FILE_BUILD move /y $$LIB_FILE_BUILD $$LIB_STORE_PATH
    MOVE_LIB2 = if exist $$LIB_FILE_BUILD_NO_PLUGIN move /y $$LIB_FILE_BUILD_NO_PLUGIN $$LIB_STORE_PATH
    QMAKE_POST_LINK += ($$MOVE_LIB) & ($$MOVE_LIB2) #拷贝多个文件,以&&连接
    #QMAKE_POST_LINK = $$MOVE_LIB2  #拷贝多个文件,以&&连接
    QMAKE_LIBS += -L$$RT_LIB_PATH/pluginlib
    QMAKE_LIBS += -L$$RT_LIB_PATH/thirdpartlib
    QMAKE_LIBS += -L$$RT_LIB_PATH/visionlib
    QMAKE_LIBS += -L$$BUILD_PATH
    DEFINES -= QT_NO_DYNAMIC_CAST
    QMAKE_CXXFLAGS -= -DQT_NO_DYNAMIC_CAST
}
unix:{
    OSBIT=64
    BUILD_PATH = $$RT_BIN_PATH/linux/$$BUILD_TYPE$$OSBIT
}
macx:{
    OSBIT=64
    BUILD_PATH = $$RT_BIN_PATH/mac/$$BUILD_TYPE$$OSBIT
}

DESTDIR = $$BUILD_PATH/pluginlib
NON_PLUGIN_LIB_PATH=$$BUILD_PATH
