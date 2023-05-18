QT       += gui
QT       += xml
QT       += sql

TARGET = mainui
TEMPLATE = lib
CONFIG += plugin qt
DEFINES += UI_MAINUI
DEFINES += Q_OS_WIN
DEFINES += MAINUI_SEND_PC
QT += network
QT += charts
QT += core gui
QT += printsupport

include(../uibuild.pri)
#include(../../../dlapi/dlapi.pri)

INCLUDEPATH +=$$PWD/bjai_v2.051/bjai/include
QMAKE_LIBS += -L$$PWD/bjai_v2.051/bjai/lib/release

HEADERS += \
    mainui.h \
    workflow/workflow.h \
    workflow/workflowfun.h \
    signalcheck.h \
    sendmessage.h \
    workflow/utils.h    \
#    homeui/mainuidlg.h \
    homeui/mainwidget.h \
    homeui/homeview.h  \
    toolbox/clock.h \
    showdefect/diagramscene.h\
    showdefect/diagramtextitem.h\
    showdefect/graphwidget.h\
    defecttableview.h	\
    $$RT_UI_INCLUDE/ProductUnion.h \
    $$RT_UI_INCLUDE/ProductOutUnion.h \
	$$RT_UI_INCLUDE/ProductMark.h \
    workflow/cimagelogic.h	\
    workflow/cimagelogicmanager.h	\
    workflow/chkimagelogic.h	\
	workflow/InvokeArithmetic.h \
    commonapi.h \
    toolbox/defectimage.h \
#    defectcfg/defectcfg.h \
 #   filter/filter.h \
 #   filter/filterset.h \
#    filter/typeitem.h \
    showTest/formshow.h \
    workflow/defectlevelthreads.h \
    config/dvconfig.h \
    config/dvfilterconfig.h   \
    homeui/defectlevelpiechart.h \
    homeui/uphbarchart.h    \
    homeui/defectchart.h    \
    homeui/defectheatmap.h  \
    homeui/dvdefecttypebarchart.h \
    mainframe/mainframe.h \
    toolbarui/toolbarbarui.h \
    dataprocess/cdataprocess.h \
    dataprocess/cdatabase.h \
     showTest/formlinechart.h \
    showTest/uphstatistics.h \
    homeui/heatmapper/heatmapper.h \
    homeui/heatmapper/gradientpalette.h \
    homeui/heatmapper/defectheatmapper.h    \
    dlArithmetic/dlArithmetic.h \
    dlArithmetic/dlArithmetic0.h \
    homeui/thresholdui.h \
    glasssizeshow.h \
    homeui/flickerlabel.h	\
	homeui/regioncntbarchart.h\
	homeui/heatmapper/defectheatmapclassify.h \
    workflow/imagestation.h \
    outputDataSave.h \
    workflow/frontpc.h \
    workflow/backpc.h  
#    workflow/createdump.h

    
SOURCES += \
    mainui.cpp \
    workflow/workflow.cpp \
    workflow/workflowfun.cpp \
    workflow/utils.cpp \
    signalcheck.cpp \
    sendmessage.cpp \
#    homeui/mainuidlg.cpp \
    homeui/homeview.cpp \
    toolbox/clock.cpp \
    showdefect/diagramscene.cpp\
    showdefect/diagramtextitem.cpp \
    showdefect/graphwidget.cpp\
    defecttableview.cpp \
	ProductUnion.cpp \
	ProductOutUnion.cpp \
	ProductMark.cpp \
    workflow/cimagelogic.cpp	\
    workflow/cimagelogicmanager.cpp	\
    workflow/chkimagelogic.cpp	\
	workflow/InvokeArithmetic.cpp \
    commomapi.cpp \
    toolbox/defectimage.cpp \
    ../../../sdk/include/iniFile.cpp \
#    filter/filter.cpp \
#    filter/filterset.cpp \
#    filter/typeitem.cpp \
    showTest/formshow.cpp \
    workflow/defectlevelthreads.cpp \
    config/dvconfig.cpp \
    config/dvfilterconfig.cpp \
    homeui/defectlevelpiechart.cpp  \
    homeui/uphbarchart.cpp  \
    homeui/defectchart.cpp    \
    homeui/defectheatmap.cpp    \
    homeui/dvdefecttypebarchart.cpp \
    mainframe/mainframe.cpp \
    toolbarui/toolbarbarui.cpp \
    dataprocess/cdataprocess.cpp \
    dataprocess/cdatabase.cpp \
    showTest/formlinechart.cpp \
    showTest/uphstatistics.cpp \
    homeui/heatmapper/heatmapper.cpp \
    homeui/heatmapper/gradientpalette.cpp \
    homeui/heatmapper/defectheatmapper.cpp \
    dlArithmetic/dlArithmetic.cpp \
    dlArithmetic/dlArithmetic0.cpp \
    homeui/thresholdui.cpp \
    glasssizeshow.cpp \
    homeui/flickerlabel.cpp	\
	homeui/regioncntbarchart.cpp\
	homeui/heatmapper/defectheatmapclassify.cpp \
    workflow/imagestation.cpp \
    outputDataSave.cpp \
    workflow/frontpc.cpp \
    workflow/backpc.cpp
    

FORMS += \
    homeui/homeview.ui \
    toolbox/clock.ui \
    defecttableview.ui \
    toolbox/defectimage.ui \
 #   filter/filter.ui \
 #   filter/filterset.ui \
 #   filter/typeitem.ui \
    showTest/formshow.ui \
    config/dvconfig.ui \
    config/dvfilterconfig.ui \
    mainframe/mainframe.ui \
    toolbarui/toolbarbarui.ui \
    showTest/formlinechart.ui \
    showTest/uphstatistics.ui \
    homeui/thresholdui.ui \
    glasssizeshow.ui


TRANSLATIONS += en_us.ts \
               cn_zh.ts 


LIBS += -L$$DESTDIR  \
        -lcallsdk \
        -ljthread \
        -luitoolbox \
        -lsdkproxy \
        -llog  \
        -ltinyxml   \
        -luicommon	\
        -lvisionimageuitool \
        -lg12common  \
        -lg12commonui  \
        -lbjcv
        
RESOURCES += \
    image.qrc

