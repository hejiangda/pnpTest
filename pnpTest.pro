TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
#Libraries
unix: CONFIG += link_pkgconfig

#OpenCV
unix: PKGCONFIG += opencv
#V4L2 摄像头驱动
unix:!macx: LIBS += -lpthread
unix:!macx: LIBS += -lv4l2
LIBS += -lzbar
SOURCES += \
        Driver/RMVideoCapture.cpp \
        main.cpp

HEADERS += \
    Driver/RMVideoCapture.hpp

DISTFILES += \
    Img/img01.jpg \
    Img/img03.jpg \
    Img/img04.jpg \
    Img/img05.jpg \
    Img/img06.jpg \
    Img/img07.png \
    Img/img08.jpg \
    LICENSE \
    README.md \
    out_camera_data.xml
