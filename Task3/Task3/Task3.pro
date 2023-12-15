TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Devices/Consol.cpp \
        Devices/FileManager.cpp \
        Devices/Planner.cpp \
        Main/Application.cpp \
        Main/main.cpp \
        Missions/Dive.cpp \
        Missions/Lift.cpp \
        Missions/Mission.cpp \
        Missions/Move.cpp \
        Missions/Return.cpp \
        Missions/Tack.cpp \
        Utilities/Command.cpp \
        Utilities/Parameter.cpp \
        Utilities/Parser.cpp \
        Utilities/Setting.cpp \

HEADERS += \
    Devices/Consol.h \
    Devices/FileManager.h \
    Devices/Planner.h \
    Main/Application.h \
    Missions/Dive.h \
    Missions/Lift.h \
    Missions/Mission.h \
    Missions/Move.h \
    Missions/Return.h \
    Missions/Tack.h \
    Utilities/Command.h \
    Utilities/Parameter.h \
    Utilities/Parser.h \
    Utilities/Setting.h
