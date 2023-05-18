
DLAPI_ROOT_PATH = $$PWD

INCLUDEPATH += $${DLAPI_ROOT_PATH}/include
QMAKE_LIBS += -L$${DLAPI_ROOT_PATH}/lib

            
LIBS += -L$${DLAPI_ROOT_PATH}/lib \
        -lclassification   \
        -lglx_log \
        -llibsvm \
        -lgrading




