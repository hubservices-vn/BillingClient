TARGET = gbClientLocker
TEMPLATE = app
LANGUAGE = C++

QT += core gui network xml

CONFIG += qaxcontainer #this line is required to use activeX widgets
CONFIG += debug_and_release
CONFIG -= flat

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += _SCL_SECURE_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS

CONFIG(debug, debug|release){
    BUILD_OPTION = Debug
}
CONFIG(release, debug|release){
    BUILD_OPTION = Release
}

DESTDIR = ../bin/$${BUILD_OPTION}
INTERMEDIATE_DIR = ../bin/intermediate/$${BUILD_OPTION}
OBJECTS_DIR = $${INTERMEDIATE_DIR}/.obj
MOC_DIR = $${INTERMEDIATE_DIR}/.moc
RCC_DIR = $${INTERMEDIATE_DIR}/.rcc

UI_HEADERS_DIR = ./GUI/uic/include
UI_SOURCES_DIR = ./GUI/uic/src


COMMON_DIR = ../../../Common #the common utility code folder for all projects
COMMON_UI_DIR = ../../../CommonUi
COMMON_TP_DIR = ../../../3rdparty
CLIENT_COMMON_DIR = ../../client_common
TOOLS_DIR = ../../../../../GBill-Tools
SHARED_COMMON_DIR = ../../../../../gbill_shared/common
SHARED_DIR = ../../../../../gbill_shared
COMMON_PB_DIR = $$SHARED_DIR/ProtoBuffer_common
CLIENT_PB_DIR = $$SHARED_DIR/ProtoBuffer_client

DEPENDPATH += .\
    ./CommandProc ./CommandProc/cs2cc \
    ./CommandSender \
    ./Config \
    ./Controllers \
    ./CrashHandler \
    ./GUI \
    ./GUI/Components \
    ./GUI/Dialogs \
    ./GUI/uic \
    ./Helper \
    ./misc \
    ./Models \
    ./Network \
    ./Resource \
    $$CLIENT_COMMON_DIR \
    $$CLIENT_COMMON_DIR/Data \
    $$CLIENT_COMMON_DIR/Network \
    $$CLIENT_COMMON_DIR/ProtoBuffer \
    $$CLIENT_COMMON_DIR/Settings \
    $$CLIENT_COMMON_DIR/ClientEvent \
    $$COMMON_DIR \
    $$COMMON_DIR/Cmd \
    $$COMMON_DIR/Network \
    $$COMMON_DIR/Settings \
    $$COMMON_DIR/Utils \
    $$COMMON_UI_DIR \
    $$COMMON_UI_DIR/Validator \
    $$COMMON_UI_DIR/Settings \
    $$COMMON_PB_DIR \
    $$CLIENT_PB_DIR \
    $$COMMON_TP_DIR/SDK \
    $$SHARED_COMMON_DIR/Cipher \
    $$SHARED_COMMON_DIR/Cmd \
    $$SHARED_COMMON_DIR/Data $$SHARED_COMMON_DIR/Data/cs2ss $$SHARED_COMMON_DIR/Data/ss2cs \
    $$SHARED_COMMON_DIR/Helper \
    $$SHARED_COMMON_DIR/Logger \
    $$SHARED_COMMON_DIR/mySqlUtil \
    $$SHARED_COMMON_DIR/TxtLoader \

INCLUDEPATH += .\
    ./CommandProc ./CommandProc/cs2cc \
    ./CommandSender \
    ./Config \
    ./Controllers \
    ./CrashHandler \
    ./GUI \
    ./GUI/Components \
    ./GUI/Dialogs \
    ./GUI/uic \
    ./Helper \
    ./misc \
    ./Models \
    ./Network \
    ./Resource \
    $$CLIENT_COMMON_DIR \
    $$CLIENT_COMMON_DIR/Data \
    $$CLIENT_COMMON_DIR/Network \
    $$CLIENT_COMMON_DIR/ProtoBuffer \
    $$CLIENT_COMMON_DIR/Settings \
    $$CLIENT_COMMON_DIR/ClientEvent \
    $$COMMON_DIR \
    $$COMMON_DIR/Cmd \
    $$COMMON_DIR/Network \
    $$COMMON_DIR/Settings \
    $$COMMON_DIR/Utils \
    $$COMMON_UI_DIR \
    $$COMMON_UI_DIR/Validator \
    $$COMMON_UI_DIR/Settings \
    $$COMMON_PB_DIR \
    $$CLIENT_PB_DIR \
    $$COMMON_TP_DIR/include \
    $$COMMON_TP_DIR/SDK \
    $$TOOLS_DIR/CcSDK/src \
    $$TOOLS_DIR/DisableCAD/disableCadLib \
    $$SHARED_COMMON_DIR/Cipher \
    $$SHARED_COMMON_DIR/Cmd \
    $$SHARED_COMMON_DIR/Data $$SHARED_COMMON_DIR/Data/cs2ss $$SHARED_COMMON_DIR/Data/ss2cs \
    $$SHARED_COMMON_DIR/Helper \
    $$SHARED_COMMON_DIR/Logger \
    $$SHARED_COMMON_DIR/mySqlUtil \
    $$SHARED_COMMON_DIR/TxtLoader \

include(CommandProc/CommandProc.pri)
include(CommandSender/CommandSender.pri)
include(Config/Config.pri)
include(Controllers/Controllers.pri)
include(CrashHandler/CrashHandler.pri)
include(GUI/GUI.pri)
include(Helper/Helper.pri)
include(misc/misc.pri)
include(Models/Models.pri)
include(Network/Network.pri)
include(resource/resource.pri)

include($$CLIENT_COMMON_DIR/client_common.pri)

include($$COMMON_DIR/Common.pri)
include($$COMMON_UI_DIR/CommonUI.pri)
include($$COMMON_PB_DIR/ProtoBuffer.pri)
include($$CLIENT_PB_DIR/ProtoBuffer.pri)
include($$COMMON_TP_DIR/SDK/SDK.pri)
include($$SHARED_COMMON_DIR/Common.pri)

#HEADERS +=

SOURCES += main.cpp


#compile options
QMAKE_LFLAGS_DEBUG += /MAP
QMAKE_LFLAGS_RELEASE += /MAP
win32{
    #customize the compiler flags
    #-Od is used to disable the c++ optimization in project properties
    QMAKE_CXXFLAGS_DEBUG += -Od -wd4100 -wd4189 -wd4996 -wd4018
    QMAKE_CXXFLAGS_RELEASE += -Od -wd4100 -wd4189 -wd4996 -wd4018
    QMAKE_CXXFLAGS_RELEASE += /Zi
    QMAKE_CXXFLAGS_RELEASE += /Od
    #customize the linker flags
    QMAKE_LFLAGS_RELEASE += /DEBUG
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL
    QMAKE_LFLAGS_RELEASE += /INCREMENTAL
    QMAKE_LFLAGS_DEBUG += /MACHINE:X86
    QMAKE_LFLAGS_RELEASE += /MACHINE:X86
    #QMAKE_EXTRA_COMPILERS += nooptimize
    #QMAKE_LFLAGS_DEBUG += /ignore: 4099
    #QMAKE_LFLAGS_RELEASE += /ignore: 4099
}

#the common libs (no matter debug/release)
LIBS += wininet.lib Ws2_32.lib version.lib

CONFIG(debug, debug|release){
LIBS += \
    $$TOOLS_DIR/CcSDK/bin/debug/CcSDKd.lib \
    $$TOOLS_DIR/DisableCAD/debug/disableCadLib.lib \
    $$COMMON_TP_DIR/lib/debug/libprotobuf.lib \
    $$COMMON_TP_DIR/lib/debug/libprotobuf-lite.lib \
}

CONFIG(release, debug|release){
LIBS += \
    $$TOOLS_DIR/CcSDK/bin/release/CcSDK.lib \
    $$TOOLS_DIR/DisableCAD/release/disableCadLib.lib \
    $$COMMON_TP_DIR/lib/release/libprotobuf.lib \
    $$COMMON_TP_DIR/lib/release/libprotobuf-lite.lib \
}

#execute the "pre build" event to update the version number
#QMAKE_PRE_BUILD= PreBuild.bat $${BUILD_OPTION}
QMAKE_POST_LINK= PostBuild.bat $${BUILD_OPTION}

#for setting the app icon in windows explorer
RC_FILE = resource/clientLocker.rc

OTHER_FILES += \
    resource/clientLocker.rc
