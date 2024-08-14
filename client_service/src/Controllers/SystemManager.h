#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QStringList>

enum PCActionType;
class QTimer;
class QueryAppRequestQtModel;
class QueryAppReplyQtModel;
class TerminateAppRequestQtModel;
class ScreenLockRequestQtModel;
class ScreenUnlockRequestQtModel;
class AccountAuthReplyQtModel;
class PcActionRequestQtModel;
class ScreencaptureRequestQtModel;
class ScreencaptureReplyQtModel;
class UpdateClientRequestQtModel;
class SystemMessageQtModel;
class VolumeQueryRequestQtModel;
class VolumeControlRequestQtModel;
class RemoteControlReplyQtModel;
// this class is to handle the client control actions
// like: terminate client applications, take screenshots, respond to server's RDP requests, reboot/shutdown PC, etc
class SystemManager : public QObject, public Singleton_T<SystemManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();
    bool startup();

public: // server cmd handler
    // find all the running processes and report back to server
    void procQueryAppRequest( const QueryAppRequestQtModel &qtModel );

    void procTermAppRequest( const TerminateAppRequestQtModel &qtModel );

    void procScreenLockRequest( const ScreenLockRequestQtModel &qtModel );

    void procScreenUnlockRequest( const ScreenUnlockRequestQtModel& qtModel );

    void procAccountAuthReply( const AccountAuthReplyQtModel &qtModel );

    void procPcAction( const PcActionRequestQtModel &qtModel );

    void procVolumeQueryRequest( const VolumeQueryRequestQtModel& qtModel );

    void procVolumeControlRequest( const VolumeControlRequestQtModel& qtModel );

    void procScreenCaptureRequest( const ScreencaptureRequestQtModel &qtModel );

    void procUpdateClientRequest( const UpdateClientRequestQtModel &qtModel );

    void procQueryAppReplyC( QueryAppReplyQtModel &qtModel );

    void procSystemMessage( const SystemMessageQtModel& qtModel );
    
    void procRemoteControlReplyC( RemoteControlReplyQtModel &qtModel );
public: // client cmd handler
    void procScreenCaptureReplyC( ScreencaptureReplyQtModel &qtModel );

signals:

public slots:
    // restart the timer
    void shutdownAvailableClientMinChangedSlot();
    void clearAppAvailableClientMinChangedSlot();
    void autoScheduledShutdownTimeChangedSlot();

    // is invoked by "AVAILABLE" clients (to shutdown the computer/clear applications after a specified time)
    void shutdownAvailableClientTimeoutSlot();
    void clearAppAvailableClientTimeoutSlot();
    void autoScheduledShutdownTimeoutSlot();

    // if "run" == true, start the timer (if the minutes >= 0)
    // if "run" == false, stop the timer
    void beginOrRefreshShutdownAvailableTimer();
    void beginOrRefreshClearAppAvailableTimer();
    void beginOrRefreshAutoScheduledShutdownTimer();
    
    void accLoggedOutSlot(int userId, int accTypeVal);
    void accLoggedInSlot();

private:
    void _pcActionImp( PCActionType type );
    
private:
    // this is for "temp lock" actoin
    QString m_secureCode;

    // after user logs out, begin these 2 timers
    QTimer* m_shutdownAvailablePcTimer;
    QTimer* m_clearAvailableAppTimer;

    // after client launched, begin this timer
    QTimer* m_autoScheduledShutdownTimer;
};

#endif // SYSTEMMANAGER_H
