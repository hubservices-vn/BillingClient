#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QObject>
#include <QtGui>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class MiniDialog;
enum MessageLevel;
class GuiController : public QObject, public Singleton_T<GuiController>
{
    Q_OBJECT
public:
    // init the fullscreen, connect to signals 
    bool init();
    bool connectSigSlot();

public:
    void restoreMiniDialog();
    void showSystemMessage( int type, MessageLevel level, const QString& msg );

signals:

public slots:
    void showMiniWindow() const;
    void hideMiniWindow() const;

    // if a new user logs in, should show the mini window
    // if the client restores connection with server (server will also send
    // out a SSBegin to unlock the client), no need to show it
    void sessionStartedSlot();
    void sessionEndedSlot();

    void screenUnlockReplyRecvedSlot(int mode, bool result);
    void screenLockReplyRecvedSlot(int mode, bool result);

private:
    MiniDialog* m_miniDialog;
};

#endif // GUICONTROLLER_H
