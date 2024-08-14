#include "ClientSettingManager.h"
#include "Client.h"
#include "CommonSettings.h"
#include "ConfigManager.h"
#include "SystemSettingQtModel.h"
#include "Utils.h"
#include "cc2csNetworkManager.h"

// warn 15 minutes in advance
const int SHUTDOWN_WARN_SECONDS = 900; // in seconds

bool ClientSettingManager::init()
{
    m_scheduledShutdownHintTimer = new QTimer();
    connect(m_scheduledShutdownHintTimer, SIGNAL(timeout()),
        this, SLOT(scheduledShutdownHintTimeoutSLot()));
    return true;
}

bool ClientSettingManager::connectSigSlot()
{
    // when the client is connected, ask the server to reply the client settings
    {
        cc2csNetworkManager* networkMgr = cc2csNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    return true;
}

void ClientSettingManager::serverConnectedSlot()
{
    // already done in cc2csNetworkManager::_notifyNetworkResultChange()
    // TODO: maybe we can move the code from networkManager to here

    //SystemSettingRequestQtModel qtModel(CConfigManager::clientId());
    //cs2ssSender::send(cs2ss::SystemSettingRequestCommand(qtModel));
}

void ClientSettingManager::procSystemSettingReply( const SystemSettingReplyQtModel &qtModel )
{
    QTime autoShutdownTime = qtModel.AutoShutdownTime_;
    QString autoShutdownMsg = qtModel.AutoShutdownMsg_;

    if (autoShutdownTime != CConfigManager::autoPcShutdownTime())
    {
        CConfigManager::setAutoPcShutdownTime(autoShutdownTime);
    }

    if (autoShutdownMsg != CConfigManager::autoPcShutdownMsg())
    {
        CConfigManager::setAutoPcShutdownMsg(autoShutdownMsg);
    }

    _handleAutoShutdownCmdReply(autoShutdownTime, autoShutdownMsg);
}

void ClientSettingManager::_handleAutoShutdownCmdReply( const QTime& time, const QString& msg )
{
    if (m_scheduledShutdownHintTimer != NULL &&
        m_scheduledShutdownHintTimer->isActive() == true)
    {
        m_scheduledShutdownHintTimer->stop();
    }

    m_autoShutdownTime = time;
    m_autoShutdownMsg = msg;

    if (! m_autoShutdownTime.isValid())
    {
        return;
    }

    QTime currentTime = QTime::currentTime();
    int secondsLeft = currentTime.secsTo(m_autoShutdownTime);
    // suppose scheduled time is 23:00, and now is 23:30, advance to next day
    if (secondsLeft < 0)
    {
        secondsLeft = 86400 + secondsLeft;
    }

    // if minutes is less then the "warn_in_advance" time, then emit signal to warn user immediately
    // suppose the previous auto_shutdown_time was set to 23:30, current_time is 23:10
    // and now cafe admin changed to auto_shutdown_time to 23:15, so when client app get this update,
    // there's only 5 minutes left, need to warn the user immediately
    if (secondsLeft <= SHUTDOWN_WARN_SECONDS)
    {
        emit aboutToShutdownComputer(m_autoShutdownTime);
    }
    else
    {
        int timerInterval = (secondsLeft - SHUTDOWN_WARN_SECONDS) * 1000;
        m_scheduledShutdownHintTimer->setSingleShot(true);
        m_scheduledShutdownHintTimer->start(timerInterval);
    }
}

void ClientSettingManager::scheduledShutdownHintTimeoutSLot()
{
    if ( ! m_autoShutdownTime.isValid())
    {
        return;
    }

    QTime currentTime = QTime::currentTime();
    int secondsLeft = currentTime.secsTo(m_autoShutdownTime);
    if (secondsLeft < 0)
    {
        secondsLeft = 0;
    }

    // if minutes is less then the "warn_in_advance" time, then emit signal to warn user immediately
    // suppose the previous auto_shutdown_time was set to 23:30, current_time is 23:10
    // and now cafe admin changed to auto_shutdown_time to 23:15, so when client app get this update,
    // there's only 5 minutes left, need to warn the user immediately
    if (secondsLeft <= SHUTDOWN_WARN_SECONDS)
    {
        emit aboutToShutdownComputer(m_autoShutdownTime);
    }
}
