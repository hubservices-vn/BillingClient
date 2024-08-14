TARGET = gbClientService
TEMPLATE = app
LANGUAGE = C++

QT += core network sql xml
QT -= gui

CONFIG   += console
CONFIG += debug_and_release
CONFIG -= flat

DEFINES += UNICODE
DEFINES += _UNICODE
DEFINES += _SCL_SECURE_NO_WARNINGS
DEFINES += _CRT_SECURE_NO_WARNINGS
# fix the problem with winsock/winsock2 redifinition
DEFINES += _WINSOCKAPI_

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
    ./CommandProc ./CommandProc/ss2cs ./CommandProc/cc2cs \
    ./CommandSender \
    ./Config \
    ./Controllers \
    ./CrashHandler \
    ./DB \
    ./Helper \
    ./Models \
    ./Network \
    ./Resource \
    ./Update \
    ./Security \
    ./WebHistory \
    ./WindowsService \
    ./GbCode \
    ../3rdparty ../3rdparty/include \
    $$CLIENT_COMMON_DIR \
    $$CLIENT_COMMON_DIR/CmdProc \
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
    $$COMMON_DIR/Version \
    $$COMMON_PB_DIR \
    $$CLIENT_PB_DIR \
    $$COMMON_TP_DIR/SDK \
    $$COMMON_TP_DIR/src/qtJson \
    $$SHARED_COMMON_DIR/Cipher \
    $$SHARED_COMMON_DIR/Cmd \
    $$SHARED_COMMON_DIR/Data $$SHARED_COMMON_DIR/Data/cs2ss $$SHARED_COMMON_DIR/Data/ss2cs \
    $$SHARED_COMMON_DIR/Helper \
    $$SHARED_COMMON_DIR/Logger \
    $$SHARED_COMMON_DIR/mySqlUtil \
    $$SHARED_COMMON_DIR/TxtLoader \

INCLUDEPATH += .\
    ./CommandProc ./CommandProc/ss2cs ./CommandProc/cc2cs \
    ./CommandSender \
    ./Config \
    ./Controllers \
    ./CrashHandler \
    ./DB \
    ./Helper \
    ./Models \
    ./Network \
    ./Resource \
    ./Update \
    ./Security \
    ./WebHistory \
    ./WindowsService \
    ./GbCode \
    ../3rdparty ../3rdparty/include \
    $$CLIENT_COMMON_DIR \
    $$CLIENT_COMMON_DIR/CmdProc \
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
    $$COMMON_DIR/Version \
    $$COMMON_PB_DIR \
    $$CLIENT_PB_DIR \
    $$COMMON_TP_DIR/include \
    $$COMMON_TP_DIR/SDK \
    $$COMMON_TP_DIR/src/qtJson \
    $$TOOLS_DIR/CcSDK/src \
    $$TOOLS_DIR/processprotect/include \
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
include(DB/DB.pri)
include(GbCode/GbCode.pri)
include(Helper/Helper.pri)
include(Models/Models.pri)
include(Network/Network.pri)
include(resource/resource.pri)
include(Security/Security.pri)
include(WebHistory/WebHistory.pri)
include(WindowsService/WindowsService.pri)

include(../3rdparty/Include/Include.pri)
include($$CLIENT_COMMON_DIR/client_common.pri)

include($$COMMON_DIR/Common.pri)
include($$COMMON_PB_DIR/ProtoBuffer.pri)
include($$CLIENT_PB_DIR/ProtoBuffer.pri)
include($$COMMON_TP_DIR/SDK/SDK.pri)
include($$COMMON_TP_DIR/src/qtJson/qtJson.pri)
include($$SHARED_COMMON_DIR/Common.pri)

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = stdafx.h
PRECOMPILED_SOURCE = stdafx.cpp

HEADERS += stdafx.h

SOURCES += \
    stdafx.cpp \
    main3.cpp

#compile options

QMAKE_LFLAGS_DEBUG += /MAP
QMAKE_LFLAGS_RELEASE += /MAP
win32{
    #customize the compiler flags
    #-Od is used to disable the c++ optimization in project properties
    QMAKE_CXXFLAGS_DEBUG += -Od -wd4100 -wd4189 -wd4996 -wd4018
    QMAKE_CXXFLAGS_RELEASE += -Od -wd4100 -wd4189 -wd4996 -wd4018
    #/Zi is to set pdb information
    QMAKE_CXXFLAGS_RELEASE += /Zi
    #/Od is to disable the optimization
    QMAKE_CXXFLAGS_RELEASE += /Od
    #this is to set the vc90.idb & vc90.pdb path
    QMAKE_CXXFLAGS += /Fd$(IntDir)
    #customize the linker flags
    QMAKE_LFLAGS_RELEASE += /DEBUG
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
    QMAKE_LFLAGS_RELEASE += /INCREMENTAL:NO
    QMAKE_LFLAGS_DEBUG += /MACHINE:X86
    QMAKE_LFLAGS_RELEASE += /MACHINE:X86
    QMAKE_LFLAGS_DEBUG += /DELAYLOAD:"WININET.DLL" /DELAYLOAD:"PROCHOST.DLL" /DELAYLOAD:"SHELL32.DLL" /DELAYLOAD:"SHLWAPI.DLL" /DELAYLOAD:"VERSION.DLL" /DELAYLOAD:"IPHLPAPI.DLL" /DELAYLOAD:"PSAPI.DLL" /DELAYLOAD:"POWRPROF.DLL"
    QMAKE_LFLAGS_RELEASE += /DELAYLOAD:"WININET.DLL" /DELAYLOAD:"PROCHOST.DLL" /DELAYLOAD:"SHELL32.DLL" /DELAYLOAD:"SHLWAPI.DLL" /DELAYLOAD:"VERSION.DLL" /DELAYLOAD:"IPHLPAPI.DLL" /DELAYLOAD:"PSAPI.DLL" /DELAYLOAD:"POWRPROF.DLL"
    #QMAKE_EXTRA_COMPILERS += nooptimize
    #QMAKE_LFLAGS_DEBUG += /ignore: 4099
    #QMAKE_LFLAGS_RELEASE += /ignore: 4099
}

#the common libs (no matter debug/release)
LIBS += wininet.lib delayimp.lib Ws2_32.lib

CONFIG(debug, debug|release){
LIBS += \
    ../3rdparty/lib/debug/gcaclientlibdmd90.lib \
    ../3rdparty/lib/debug/gbprotectd.lib \
    ../3rdparty/lib/debug/gbsharedd.lib \
    $$COMMON_TP_DIR/lib/debug/libprotobuf.lib \
    $$COMMON_TP_DIR/lib/debug/libprotobuf-lite.lib \
    $$TOOLS_DIR/processprotect/lib/prochost.lib \
    $$TOOLS_DIR/CcSDK/bin/debug/CcSDKd.lib \
}
CONFIG(release, debug|release){
LIBS += \
    ../3rdparty/lib/release/gcaclientlibmd90.lib \
    ../3rdparty/lib/release/gbprotect.lib \
    ../3rdparty/lib/release/gbshared.lib \
    $$COMMON_TP_DIR/lib/release/libprotobuf.lib \
    $$COMMON_TP_DIR/lib/release/libprotobuf-lite.lib \
    $$TOOLS_DIR/CcSDK/bin/release/CcSDK.lib \
    $$TOOLS_DIR/processprotect/lib/prochost.lib \
}


#execute the "pre build" event to update the version number
#QMAKE_PRE_BUILD= PreBuild.bat $${BUILD_OPTION}
QMAKE_POST_LINK= PostBuild.bat $${BUILD_OPTION}

#for setting the app icon in windows explorer
RC_FILE = resource/clientSvc.rc

OTHER_FILES += \
    resource/clientSvc.rc \
