
DEPENDPATH += . cs2cl s2c
INCLUDEPATH += .

HEADERS += cs2clCmdProcs.h \
           cs2cl/CLHandshakeAckProcessorC.h \
           cs2cl/EndConnectionReplyProcessorC.h \
           cs2cl/ScreenLockReplyProcessorC.h \
           cs2cl/ScreenUnlockReplyProcessorC.h \
           cs2cl/ClientCodeDllUpdatedProcessorC.h \
           s2c/FlashTransferReplyProcessor.h \
           s2c/SessionBeginProcessor.h \
           s2c/SessionEndProcessor.h \
           s2c/UserLoginReplyProcessor.h \
           s2c/UserLogoutReplyProcessor.h
SOURCES += cs2clCmdProcs.cpp \
           cs2cl/CLHandshakeAckProcessorC.cpp \
           cs2cl/EndConnectionReplyProcessorC.cpp \
           cs2cl/ScreenLockReplyProcessorC.cpp \
           cs2cl/ScreenUnlockReplyProcessorC.cpp \
           cs2cl/ClientCodeDllUpdatedProcessorC.cpp \
           s2c/FlashTransferReplyProcessor.cpp \
           s2c/SessionBeginProcessor.cpp \
           s2c/SessionEndProcessor.cpp \
           s2c/UserLoginReplyProcessor.cpp \
           s2c/UserLogoutReplyProcessor.cpp
