#-------------------------------------------------
#
# Project created by QtCreator 2010-11-29T00:20:58
#
#-------------------------------------------------

TARGET = defectclassificationuiplugin
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_HELPUIPLUGIN

include(../uibuild.pri)

PLUGIN_PATH = $$PWD

HEADERS += \
    $$PLUGIN_PATH/defectclassificationuiplugin.h \
    $$PLUGIN_PATH/defectclassificationui.h \
    defectexpression.h \
    keyfeatureswidget.h \
    scriptLexer.h  \
    computationalexpressions.h  \
    Features.h  \
    MathFunction.h  \
    csvparser.h

SOURCES += \
    $$PLUGIN_PATH/defectclassificationuiplugin.cpp \
    $$PLUGIN_PATH/defectclassificationui.cpp \
    defectexpression.cpp \
    keyfeatureswidget.cpp   \
    scriptLexer.cpp  \
    computationalexpressions.cpp    \
    Features.cpp    \
    MathFunction.cpp    \
    csvparser.cpp

FORMS += \ 
    defectexpression.ui \
    keyfeatureswidget.ui

TRANSLATIONS += $$PLUGIN_PATH/en_us.ts \
               $$PLUGIN_PATH/cn_zh.ts


LIBS += -L$${DESTDIR}   \
        -luicommon
