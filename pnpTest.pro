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
    LICENSE \
    README.md
