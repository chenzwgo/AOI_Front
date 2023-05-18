# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = g12common
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = G12COMMON_RESOURCE_LIBRARY
include (../sdkbuild.pri)
DESTDIR = $$BUILD_PATH
INCLUDEPATH += $${RT_SDK_INCLUDE}/expressionparser

SOURCES +=  ./g12common.cpp \
            ./g12utils.cpp  \
            ./g12Configutils.cpp  \
            ./expressionparser/scriptLexer.cpp  \
#            ./expressionparser/computationalexpressions.cpp    \
            ./expressionparser/Features.cpp    \
            ./expressionparser/MathFunction.cpp    \
            ./expressionparser/csvparser.cpp \
            ./lightcontrol/lightcontrol_new.cpp \
			./robotcrl/robotcrl.cpp \
            ./lightcontrol/lightcontrol_tcp.cpp 
           
HEADERS +=  $${RT_SDK_INCLUDE}/g12common.h	\
            $${RT_SDK_INCLUDE}/g12utils.h   \
            $${RT_SDK_INCLUDE}/g12Configutils.h    \
            $${RT_SDK_INCLUDE}/expressionparser/scriptLexer.h  \
#            $${RT_SDK_INCLUDE}/expressionparser/computationalexpressions.h    \
            $${RT_SDK_INCLUDE}/expressionparser/Features.h    \
            $${RT_SDK_INCLUDE}/expressionparser/MathFunction.h    \
            $${RT_SDK_INCLUDE}/expressionparser/csvparser.h \
            $${RT_SDK_INCLUDE}/lightcontrol_new.h	\
            $${RT_SDK_INCLUDE}/robotcrl.h \
            ./lightcontrol/lightcontrol_tcp.h \

# UI needed    
#FORMS += ./

# UI needed  
#RESOURCES += ./    


# Load lib path and lib file
LIBS += -L$${DESTDIR}   \
        -lsdkcore   \
        -lcallsdk   \
        -ljthread   \
        -ltinyxml

