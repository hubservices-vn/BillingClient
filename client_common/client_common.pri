
DEPENDPATH += \
    $$PWD \
    $$PWD/Data \
    $$PWD/Network \
    $$PWD/Settings \
    $$PWD/ClientEvent \

INCLUDEPATH += \
    $$PWD \
    $$PWD/Data \
    $$PWD/Network \
    $$PWD/Settings \
    $$PWD/ClientEvent \

HEADERS += \
    $$PWD/ClientEvent/*.h \
    $$PWD/Data/*.h \
    $$PWD/Network/*.h \
    $$PWD/Settings/*.h
    
SOURCES += \
    $$PWD/Data/*.cpp \
    $$PWD/Network/*.cpp \
    $$PWD/Settings/*.cpp
