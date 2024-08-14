#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "CcCore/CcSingleton.h"
#include <QObject>

class ScreenLockReplyQtModel;
class ScreenUnlockReplyQtModel;
class EndConnectionReplyQtModel;
class SystemManager : public QObject, public Singleton_T<SystemManager>
{
    Q_OBJECT
public:
    ~SystemManager();
    bool init();
    bool connectSigSlot();

    bool restriceOSAccess();
    bool resumeOSAccess();

public: // cmd handler
    // lock screen
    void procScreenLockReply(const ScreenLockReplyQtModel &qtModel);
    // unlock screen
    void procScreenUnlockReply( const ScreenUnlockReplyQtModel& qtModel );
    // end connection, close locker app
    void procEndConnectionReply( const EndConnectionReplyQtModel &qtModel );
signals:
    // modeVal: value of enum "ScreenLockMode", defined in ScreenLockQtModel.h
    void screenUnlockReplyRecved(int modeVal, bool result);

    void lockScreenRequested();
    void unlockScreenRequested();

public slots:

private:
    static bool s_osAppearanceRestricted;
};

#endif // SYSTEMMANAGER_H
