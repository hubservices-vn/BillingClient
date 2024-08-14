
DEPENDPATH += . \
              Components \
              Dialogs \
              uiDialogs \
              Widgets \
              uic/include \
              Widgets/QcIPWidget
INCLUDEPATH += .

HEADERS += CssLoader.h \
           GuiController.h \
           Components/SystemTray.h \
           Dialogs/AdminOptionDialog.h \
           Dialogs/ChangePwdDialog.h \
           Dialogs/LockOutSettingDialog.h \
           Dialogs/MiniDialog.h \
           Dialogs/OrderDialog.h \
           Dialogs/OrderHistoryDialog.h \
           Dialogs/TextMessageDialog.h \
           Dialogs/TopupByCouponDialog.h \
           Widgets/GClickableLabel.h \
           Widgets/GPlainTextEdit.h \
           Widgets/GSkinDialog.h \
           Widgets/GSkinMainWindow.h \
           Widgets/QcRibbonPushButton.h \
           Widgets/QSkinMainWindow.h \
           Widgets/QSkinWidget.h \
           Widgets/QWidgetResizeHandler_p.h \
           Widgets/QcIPWidget/ConstConfig.h \
           Widgets/QcIPWidget/QcIPSectorWidget.h \
           Widgets/QcIPWidget/QcIPWidget.h
FORMS += uiDialogs/AdminOptionDialog.ui \
         uiDialogs/ChangePwdDialog.ui \
         uiDialogs/LockOutSettingDialog.ui \
         uiDialogs/MiniDialog.ui \
         uiDialogs/OrderDialog.ui \
         uiDialogs/OrderHistoryDialog.ui \
         uiDialogs/TextMessageDialog.ui \
         uiDialogs/TopupByCouponDialog.ui \
         Widgets/QcIPWidget/QcIPWidget.ui
SOURCES += CssLoader.cpp \
           GuiController.cpp \
           Components/SystemTray.cpp \
           Dialogs/AdminOptionDialog.cpp \
           Dialogs/ChangePwdDialog.cpp \
           Dialogs/LockOutSettingDialog.cpp \
           Dialogs/MiniDialog.cpp \
           Dialogs/OrderDialog.cpp \
           Dialogs/OrderHistoryDialog.cpp \
           Dialogs/TextMessageDialog.cpp \
           Dialogs/TopupByCouponDialog.cpp \
           Widgets/GClickableLabel.cpp \
           Widgets/GPlainTextEdit.cpp \
           Widgets/GSkinDialog.cpp \
           Widgets/GSkinMainWindow.cpp \
           Widgets/QcRibbonPushButton.cpp \
           Widgets/QSkinMainWindow.cpp \
           Widgets/QSkinWidget.cpp \
           Widgets/QWidgetResizeHandler_p.cpp \
           Widgets/QcIPWidget/QcIPSectorWidget.cpp \
           Widgets/QcIPWidget/QcIPWidget.cpp
