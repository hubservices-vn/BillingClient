
DEPENDPATH += . Dialogs uiDialogs uic/include
INCLUDEPATH += .

HEADERS += CssLoader.h \
           GuiController.h \
           Dialogs/FullScreenDialog2.h \
           Dialogs/LoginDialog.h \
           Dialogs/InfoDialog.h \
           Dialogs/TempLockDialog.h \
           Dialogs/uiFullScreenDialog2.h
FORMS += uiDialogs/LoginDialog.ui uiDialogs/InfoDialog.ui uiDialogs/TempLockDialog.ui
SOURCES += CssLoader.cpp \
           GuiController.cpp \
           Dialogs/FullScreenDialog2.cpp \
           Dialogs/InfoDialog.cpp \
           Dialogs/LoginDialog.cpp \
           Dialogs/TempLockDialog.cpp \
           Dialogs/uiFullScreenDialog2.cpp
