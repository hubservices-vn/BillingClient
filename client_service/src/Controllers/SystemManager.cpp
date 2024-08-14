#include "SystemManager.h"

#include <QStringList>
#include <QTimer>
#include <QProcess>
#include <QCoreApplication>
#include <QFile>
#include <QSettings>

#include "OSAgentWinQt.h"
#include "CommonSettings.h"
#include "UserAccountManager.h"
#include "ConfigManager.h"
#include "ClientSettingManager.h"
#include "log.h"
#include "SecurityProfileManager.h"
#include "AppControlManager.h"
#include "cs2ssSocketManager.h"
#include "QueryTermAppQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ScreencaptureQtModel.h"
#include "ScreenLockQtModel.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "UserLoginQtModel.h"
#include "PcActionQtModel.h"
#include "cs2ccNetworkManager.h"
#include "UpdateClientQtModel.h"
#include "DateTimeUtils.h"
#include "SystemMessageQtModel.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"
#include "WindowsVolumeControl.h"



bool SystemManager::init()
{
    m_shutdownAvailablePcTimer = new QTimer();
    connect(m_shutdownAvailablePcTimer, SIGNAL(timeout()), 
        this, SLOT(shutdownAvailableClientTimeoutSlot()));

    m_clearAvailableAppTimer = new QTimer();
    connect(m_clearAvailableAppTimer, SIGNAL(timeout()), 
        this, SLOT(clearAppAvailableClientTimeoutSlot()));

    m_autoScheduledShutdownTimer = new QTimer();
    connect(m_autoScheduledShutdownTimer, SIGNAL(timeout()),
        this, SLOT(autoScheduledShutdownTimeoutSlot()));

    return true;
}

// when the application starts up, it's in the AVAILABLE mode so should listen to
// the minutes passed event (if server set to shutdown available computer in 10 mins,
// we should listen to it at the beginning)
bool SystemManager::startup()
{
    beginOrRefreshShutdownAvailableTimer();

    beginOrRefreshClearAppAvailableTimer();

    // auto scheduled shutdown the computer
    beginOrRefreshAutoScheduledShutdownTimer();

    return true;
}

bool SystemManager::connectSigSlot()
{
    // server changes the time to shutdown computer and the time to clear app
    // need to restart the timer (listen to ClientSettingManager)
    {
        ClientSettingManager* mgr = ClientSettingManager::GetInstance();
        // the "shutdown available clients" minutes setting changed
        connect(mgr, SIGNAL(shutdownAvailableClientMinChanged()),
            this, SLOT(shutdownAvailableClientMinChangedSlot()));
        // the "clear app" minutes setting changed
        connect(mgr, SIGNAL(clearAppAvailableClientMinChanged()),
            this, SLOT(clearAppAvailableClientMinChangedSlot()));
        // the "auto scheduled shutdown" time setting changed
        connect(mgr, SIGNAL(autoScheduledShutdownTimeChanged()),
            this, SLOT(autoScheduledShutdownTimeChangedSlot()));
    }

    // user log in/log out
    // after log in, stop the timer
    // after log out, begin the timer and prepare to shutdown computer
    // or clear applications
    {
        UserAccountManager* mgr = UserAccountManager::GetInstance();
        connect(mgr, SIGNAL(accLoggedOut(int,int)),
            this, SLOT(accLoggedOutSlot(int,int)));

        // user logged in, stop the timer
        connect(mgr, SIGNAL(accLoggedIn()),
            this, SLOT(accLoggedInSlot()));
    }

    return true;
}

void SystemManager::procQueryAppRequest( const QueryAppRequestQtModel &qtModel )
{
    // forward to client App
    cs2ccSender::send(cs2cc::QueryAppRequestCommandC(qtModel));
}

void SystemManager::procQueryAppReplyC( QueryAppReplyQtModel &qtModel )
{
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::QueryAppReplyCommand(qtModel));
}

void SystemManager::procTermAppRequest( const TerminateAppRequestQtModel &qtModel )
{
    // forward to client App
    cs2ccSender::send(cs2cc::TerminateAppRequestCommandC(qtModel));
}

void SystemManager::procScreenLockRequest( const ScreenLockRequestQtModel &qtModel )
{
    bool result = false;
    switch(qtModel.Mode_)
    {
    case LockMode_Lock:
        {
            QString tempPwd = qtModel.Password_;
            CConfigManager::setInTempLockMode(true);
            CConfigManager::setTempLockCode(tempPwd);
            ScreenLockReplyQtModel replyModel(qtModel.Mode_, /*result*/true);
            cs2ccSender::send(cs2cc::ScreenLockReplyCommandC(replyModel));

            cs2clSender::send(cs2cl::ScreenLockReplyCommandC(replyModel));
        }
        break;
    }
}

void SystemManager::procAccountAuthReply( const AccountAuthReplyQtModel &qtModel )
{
    bool unlockResult = (
        qtModel.Result_ == LoginResult_AlreadyLoggedIn || 
        qtModel.Result_ == LoginResult_OK
        );
    ScreenUnlockReplyQtModel replyModel(LockMode_UnlockByAcc, unlockResult);
    cs2ccSender::send(cs2cc::ScreenUnlockReplyCommandC(replyModel));
    cs2clSender::send(cs2cl::ScreenUnlockReplyCommandC(replyModel));
}

void SystemManager::procPcAction( const PcActionRequestQtModel &qtModel )
{
    _pcActionImp(qtModel.ActionType_);
}

void SystemManager::procVolumeQueryRequest( const VolumeQueryRequestQtModel& qtModel )
{
    VolumeQueryReplyQtModel rplQtModel;
    rplQtModel.ClientId_ = CConfigManager::clientId();
    rplQtModel.IsMuted_ = WindowsVolumeControl::isMuted();
    rplQtModel.VolumeLevel_ = WindowsVolumeControl::getMasterVolume();

    cs2ssSender::send(cs2ss::QueryVolumeReplyCommand(rplQtModel));
}

void SystemManager::procVolumeControlRequest( const VolumeControlRequestQtModel& qtModel )
{
    bool isToMute = qtModel.IsToMute_;
    int volLevel = qtModel.VolumeLevel_;

    WindowsVolumeControl::setMute(isToMute);
    WindowsVolumeControl::setMasterVolume(volLevel);
}

void SystemManager::procScreenCaptureRequest( const ScreencaptureRequestQtModel &qtModel )
{
    // forward to client App
    cs2ccSender::send(cs2cc::ScreenCaptureRequestCommandC(qtModel));
}

void SystemManager::_pcActionImp( PCActionType type )
{
    // forward to client app
    PcActionRequestQtModel qtModel(type);
    cs2ccSender::send(cs2cc::PCActionRequestCommandC(qtModel));

    // only in DEBUG mode
    if (CConfigManager::isDeveloperEnv() == true)
    { 
        LOG_DEBUG4 << "simulated PC action: " << qPrintable(PcActionTypeToQString(type));
        return;
    }
    else
    {
        LOG_DEBUG4 << "PC action: " << qPrintable(PcActionTypeToQString(type));

        const int NETWORK_DISCONNECT_INTERVAL = 30000; // 30,000ms = 30s
        // when reboot/shutdown, the client
        // doesn't close the socket connection immediately
        // so we do this actively by invoking the 
        // reconnectSocket() method of SocketManager
        // but DON'T do this to standby, imagine: server asks client to actively disconnect,
        // after standby, the client doesn't know to re-connect
        if (type == PCActionType_Shutdown)
        {
            cs2ssSocketManager::GetInstance()->reconnectSocket(NETWORK_DISCONNECT_INTERVAL);
        }
        else if (type == PCActionType_Reboot)
        {
            cs2ssSocketManager::GetInstance()->reconnectSocket(NETWORK_DISCONNECT_INTERVAL);
        }
    }
}

void SystemManager::procScreenCaptureReplyC( ScreencaptureReplyQtModel &qtModel )
{
    // forward to server
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::ScreenCaptureReplyCommand(qtModel));
}

void SystemManager::procUpdateClientRequest( const UpdateClientRequestQtModel &qtModel )
{
    cs2ccSender::send(cs2cc::UpdateClientRequestCommandC(qtModel));
}

void SystemManager::procSystemMessage( const SystemMessageQtModel& qtModel )
{
    cs2ccSender::send(cs2cc::SystemMessageCommandC(qtModel));
}

void SystemManager::shutdownAvailableClientMinChangedSlot()
{
    if (UserAccountManager::GetInstance()->isUserLoggedIn() == true)
    {
        return;
    }

    beginOrRefreshShutdownAvailableTimer();
}

void SystemManager::clearAppAvailableClientMinChangedSlot()
{
    if (UserAccountManager::GetInstance()->isUserLoggedIn() == true)
    {
        return;
    }

    beginOrRefreshClearAppAvailableTimer();
}

void SystemManager::autoScheduledShutdownTimeChangedSlot()
{
    beginOrRefreshAutoScheduledShutdownTimer();
}

void SystemManager::shutdownAvailableClientTimeoutSlot()
{
    LOG_DEBUG << "shutdown AVAILABLE pc timer reached. turn off pc";
    if (UserAccountManager::GetInstance()->isUserLoggedIn())
    {
        LOG_DEBUG << "some user is using, don't shutdown";
        return;
    }
    _pcActionImp(PCActionType_Shutdown);
}

void SystemManager::clearAppAvailableClientTimeoutSlot()
{
    LOG_DEBUG << "clear AVAILABLE pc app timer reached. clear apps";
    if (UserAccountManager::GetInstance()->isUserLoggedIn())
    {
        LOG_DEBUG << "some user is using, don't clear app";
        return;
    }
    _pcActionImp(PCActionType_ClearApp);
}

void SystemManager::autoScheduledShutdownTimeoutSlot()
{
    LOG_DEBUG << "autoScheduledShutdownTimeoutSlot reached. turn off pc";
    _pcActionImp(PCActionType_Shutdown);
}
void SystemManager::beginOrRefreshShutdownAvailableTimer()
{
    LOG_DEBUG << "begin/refresh shutdown available pc timer";
    if (m_shutdownAvailablePcTimer != NULL &&
        m_shutdownAvailablePcTimer->isActive() == true)
    {
        LOG_DEBUG4 << "timer was active, stop it. old interval(ms): " << m_shutdownAvailablePcTimer->interval();
        m_shutdownAvailablePcTimer->stop();
    }

    if (UserAccountManager::GetInstance()->isUserLoggedIn() == true)
    {
        LOG_DEBUG << "user is logged in, skip this";
        return;
    }

    int shutdownMin = CConfigManager::shutdownAvailableClientMinutes();
    LOG_DEBUG << "new shutdown available pc timer interval(ms) is: " << shutdownMin * 60 * 1000;
    // only start the timer if time is "GREATER" than 0
    if (shutdownMin > 0)
    {
        m_shutdownAvailablePcTimer->setSingleShot(true);
        m_shutdownAvailablePcTimer->start(shutdownMin * 60 * 1000);
    }
    else
    {
        LOG_DEBUG4 << "timer interval < 0, don't start";
    }
}

void SystemManager::beginOrRefreshClearAppAvailableTimer()
{
    if (m_clearAvailableAppTimer != NULL &&
        m_clearAvailableAppTimer->isActive() == true)
    {
        m_clearAvailableAppTimer->stop();
    }

    if (UserAccountManager::GetInstance()->isUserLoggedIn() == true)
    {
        return;
    }

    int clearAppMin = CConfigManager::clearAppAvailableClientMinutes();
    LOG_DEBUG4 << "clearApp timer interval: " << clearAppMin << " mins";
    // starts the timer is time is "NOT LESS" than 0
    if (clearAppMin >= 0)
    {
        m_clearAvailableAppTimer->setSingleShot(true);
        m_clearAvailableAppTimer->start(clearAppMin * 60 * 1000);
    }
}

void SystemManager::beginOrRefreshAutoScheduledShutdownTimer()
{
    if (m_autoScheduledShutdownTimer != NULL &&
        m_autoScheduledShutdownTimer->isActive() == true)
    {
        m_autoScheduledShutdownTimer->stop();
    }

    QTime autoShutdownTime = CConfigManager::autoPcShutdownTime();
    LOG_DEBUG4 << "autoScheduledShutdown time: " 
        << qPrintable(DateTimeUtils::timeToString(autoShutdownTime));

    if (! autoShutdownTime.isValid())
    {
        return;
    }

    QTime currentTime = QTime::currentTime();
    int secondsLeft = currentTime.secsTo(autoShutdownTime);
    // suppose scheduled time is 23:00, and now is 23:30, advance to next day
    if (secondsLeft < 0)
    {
        secondsLeft = 86400 + secondsLeft;
    }

    // starts the timer is time is "NOT LESS" than 0
    if (secondsLeft >= 0)
    {
        m_autoScheduledShutdownTimer->setSingleShot(true);
        m_autoScheduledShutdownTimer->start(secondsLeft * 1000);
    }
}

void SystemManager::accLoggedOutSlot(int userId, int accTypeVal)
{
    // the user logged out, begin the timer and prepare to shutdown computer
    // or clear applications
    beginOrRefreshShutdownAvailableTimer();
    beginOrRefreshClearAppAvailableTimer();
}

void SystemManager::accLoggedInSlot()
{
    if (m_shutdownAvailablePcTimer != NULL &&
        m_shutdownAvailablePcTimer->isActive())
    {
        m_shutdownAvailablePcTimer->stop();
    }

    if (m_clearAvailableAppTimer != NULL &&
        m_clearAvailableAppTimer->isActive())
    {
        m_clearAvailableAppTimer->stop();
    }
}

void SystemManager::procScreenUnlockRequest( const ScreenUnlockRequestQtModel& qtModel )
{
    bool result = false;
    switch(qtModel.Mode_)
    {
    case LockMode_UnlockByTmpPwd:
        {
            bool result = true;
            if (qtModel.Password_ != CConfigManager::tempLockCode())
            {
                result = false;
            }
            else
            {
                result = true;
                CConfigManager::setInTempLockMode(false);
                CConfigManager::setTempLockCode("");
            }
            ScreenUnlockReplyQtModel replyModel(qtModel.Mode_, result);
            cs2ccSender::send(cs2cc::ScreenUnlockReplyCommandC(replyModel));

            cs2clSender::send(cs2cl::ScreenUnlockReplyCommandC(replyModel));
        }
        break;
    case LockMode_UnlockByAcc:
        {
            if (UserAccountManager::GetInstance()->isLocalAdminAccount(qtModel.AccountName_, qtModel.AccountPwdRaw_))
            {
                CConfigManager::setInTempLockMode(false);
                CConfigManager::setTempLockCode("");
                ScreenUnlockReplyQtModel replyModel(qtModel.Mode_, /*result*/true);
                cs2ccSender::send(cs2cc::ScreenUnlockReplyCommandC(replyModel));

                cs2clSender::send(cs2cl::ScreenUnlockReplyCommandC(replyModel));
            }
            // send to server for auth, delay unlock until recved the acc-auth reply
            else
            {
                AccountAuthRequestQtModel fwdModel(CConfigManager::clientId(), qtModel.AccountName_, qtModel.AccountPwdRaw_);
                cs2ssSender::send(cs2ss::AccountAuthRequestCommand(fwdModel));
            }
        }
        break;
    }
}

void SystemManager::procRemoteControlReplyC( RemoteControlReplyQtModel &qtModel )
{
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::RemoteControlReplyCommand(qtModel));
}