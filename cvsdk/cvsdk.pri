
CV_ROOT_PATH = $$PWD
INCLUDEPATH += $${CV_ROOT_PATH}/include
INCLUDEPATH += $${CV_ROOT_PATH}/include/opencv
INCLUDEPATH += $${CV_ROOT_PATH}/include/opencv2

			
LIBS += -L$${CV_ROOT_PATH}/lib    \
        -lopencv_core331d   \
        -lopencv_imgproc331d    \
        -lopencv_imgcodecs331d  \
        -lopencv_features2d331d	



