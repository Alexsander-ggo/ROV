CONFIG += c++17

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

SOURCES += \
        ImageProcessing.cpp \
        main.cpp

HEADERS += \
    ImageProcessing.h
