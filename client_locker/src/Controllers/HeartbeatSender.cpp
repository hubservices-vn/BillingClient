#include "HeartBeatSender.h"

#include "HeartBeatQtModel.h"
#include "log.h"
#include "DateTimeUtils.h"
#include "cl2csSender.h"
#include "cl2csCmds.h"
#include "ConfigManager.h"
#include "SessionManager.h"

// heartbeat every 9s (client_service UIAppManager treats 30s as "NO_RESPONSE")
#ifdef NDEBUG
const int MS_HeartbeatInterval = 9000;
#else
const int MS_HeartbeatInterval = 4000;
#endif

//////////////////////////////////////////////////////////////////////////
// HeartBeatSender
//////////////////////////////////////////////////////////////////////////
bool HeartBeatSender::init()
{
    qRegisterMetaType<HeartBeatQtModel>("HeartBeatQtModel");

    m_heartBeatTimer.reset(new QTimer);
    m_heartBeatTimer->setInterval(MS_HeartbeatInterval);
    connect(m_heartBeatTimer.data(), SIGNAL(timeout()),
        this, SLOT(sendHeartbeatSlot()));
    m_heartBeatTimer->start();

    // send it once at init (so it's not too late)
    sendHeartbeatSlot();

    return true;
}

bool HeartBeatSender::connectSigSlot()
{
    return true;
}

void HeartBeatSender::procHeartBeatReply( const HeartBeatQtModel &qtModel )
{
    m_lastHeartBeatRecvDT = QDateTime::currentDateTime();
    if (qtModel.IsSessionIdReady_)
    {
        // sync our ssid with service's
        SessionManager::GetInstance()->onSessionIdReceived(qtModel.SessionId_);
    }
}

void HeartBeatSender::startHeartbeat()
{
    if (m_heartBeatTimer->isActive())
    {
        m_heartBeatTimer->stop();
    }

    m_heartBeatTimer->start(MS_HeartbeatInterval);
}

void HeartBeatSender::stopHeartbeat()
{
    if (m_heartBeatTimer->isActive())
    {
        m_heartBeatTimer->stop();
    }
}

void HeartBeatSender::sendHeartbeatSlot()
{
    HeartBeatQtModel qtModel(CConfigManager::clientId(), HB_C_Locker, qApp->applicationPid(), UtcDTUtils::currTimestamp(), 
        SessionManager::GetInstance()->currentSessionId(), true);
    cl2csSender::send(cl2cs::HeartbeatCommand(qtModel));
}


