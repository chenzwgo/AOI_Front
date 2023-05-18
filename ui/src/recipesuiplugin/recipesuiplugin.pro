#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T00:20:58
#
#-------------------------------------------------
QT       += gui
QT       += xml
TARGET = recipesuiplugin
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_RECIPESUIPLUGIN
DEFINES += Q_OS_WIN
include(../uibuild.pri)

PLUGIN_PATH = $$PWD

HEADERS += \
    $$PLUGIN_PATH/recipesuiplugin.h \
    $$PLUGIN_PATH/recipesui.h \ 
    productparamui.h \
    recipesparamui.h    \
    sendmessage.h   \
    ./pages/abstractpagewidget.h    \
    ./pages/acquisitionpage.h   \
    ./pages/qualificationpage.h    \
    ./pages/staticshadingcorrectionpage.h    \
    ./pages/illuminationcontroldlg_new.h  \
    defectcustomdelegate.h  \
    ./pages/measuresizedlg.h   \
    ./pages/defectlevelpage/defectitemset.h \
    ./pages/defectlevelpage/defectlevelpage.h \
    ./pages/defectlevelpage/defectvalue.h \ 
    pages/netlightcommunicationdlg.h \
    pages/pccommunicationdlg.h \
	pages/otherpage.h \
    pages/areacampage.h \
    pages/robotsettingui.h


 
    
SOURCES += \
    $$PLUGIN_PATH/recipesuiplugin.cpp \
    $$PLUGIN_PATH/recipesui.cpp \ 
    productparamui.cpp \
    recipesparamui.cpp  \
    sendmessage.cpp \
    ./pages/abstractpagewidget.cpp    \
    ./pages/acquisitionpage.cpp   \
    ./pages/qualificationpage.cpp    \
    ./pages/staticshadingcorrectionpage.cpp    \
    ./pages/illuminationcontroldlg_new.cpp  \
    defectcustomdelegate.cpp    \
    ./pages/defectlevelpage/defectitemset.cpp \
    ./pages/defectlevelpage/defectlevelpage.cpp \
    ./pages/defectlevelpage/defectvalue.cpp  \
    ./pages/measuresizedlg.cpp \ 
    pages/netlightcommunicationdlg.cpp \
    pages/pccommunicationdlg.cpp \
    pages/otherpage.cpp \
    pages/areacampage.cpp \
    pages/robotsettingui.cpp

    
    
FORMS += \ 
    pages/staticcorrection.ui \
    pages/acquisitionui.ui \
    pages/generalform.ui \
    pages/illchannelform_new.ui \
    pages/inilight_new.ui   \
    pages/imageconfigform.ui    \
    ./pages/defectlevelpage/defectitemset.ui \
    ./pages/defectlevelpage/defectlevelpage.ui \
    ./pages/defectlevelpage/defectvalue.ui  \
    pages/measuresizedlg.ui \ 
    pages/netlightcommunicationdlg.ui \
    pages/pccommunicationdlg.ui	\
    pages/areacampage.ui    \
    pages/robotsettingui.ui



TRANSLATIONS += $$PLUGIN_PATH/en_us.ts \
               $$PLUGIN_PATH/cn_zh.ts


LIBS += -L$${DESTDIR}   \
        -luicommon	\
        -lg12common \
        -ljthread   \
        -llog	\
		-lcallsdk	\
        -lvisionimageuitool \
        -ltinyxml   \
        -luitoolbox

RESOURCES += \
    image.qrc
