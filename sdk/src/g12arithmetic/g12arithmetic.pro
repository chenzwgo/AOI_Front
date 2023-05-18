# -------------------------------------------------
# Project created by QtCreator 2014-11-22T10:32:37
# -------------------------------------------------
QT -= gui core
TARGET = g12arithmetic
TEMPLATE = lib
CONFIG += exportlib
CONFIG   += dll
DEFINES = G12ARITHMETIC_RESOURCE_LIBRARY

include (../sdkbuild.pri)
include (../../../cvsdk/cvsdk.pri)

SOURCES +=  ./g12arithmetic.cpp     \
            ./arithmeticmeasuresize.cpp \
            ./measuresize.cpp \
            ./resource.cpp      \
            ./arithmeticstatic.cpp  \
            ./staticcorrection.cpp  \
            ./arithmeticcoordtransform.cpp \
            ./coordtransform.cpp \
            ./rmatrix.cpp
           
HEADERS +=  ./g12arithmetic.h           \
            ./arithmeticmeasuresize.h \
            ./measuresize.h \
            ./resource.h        \    
            ./arithmeticstatic.h    \
            ./staticcorrection.h    \
            ./arithmeticcoordtransform.h \
            ./coordtransform.h  \
            ./rmatrix.h

# Load lib path and lib file
LIBS += -L$${DESTDIR}   \
        -lsdkcore   \
        -ljthread   \
        -lg12common

