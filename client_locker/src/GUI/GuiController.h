#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QObject>
#include <QtGui>
#include "CcCore/CcSingleton.h"

class FullScreenDialog2;
class LoginDialog;
class InfoDialog;
class TempLockDialog;

enum MessageLevel;
class GuiController : public QObject, public Singleton_T<GuiController>
{
    Q_OBJECT
public:
    // init the fullscreen, connect to signals 
    bool init();
    bool connectSigSlot();

    // init other GUI components, connect to signals 
    bool delayInit();
    bool delayConnectSigSlot();

signals:

public slots:
    // when the user press some keys or double-click on the full-screen dialog,
    // we should pop up the log-in dialog
    void popupLoginDialog();

    // command handler for log in/log out
    bool lockClient();
    bool unlockClient();

    // cmd handler for temp lock/temp unlock
    void tempLockScreen();
    void unlockScreen();


private:
    FullScreenDialog2* m_fullScreenDialog;
    LoginDialog* m_loginDialog;
    InfoDialog* m_infoDialog;
    TempLockDialog* m_tempLockDialog;
};

#endif // GUICONTROLLER_H
