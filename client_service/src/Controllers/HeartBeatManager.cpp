#include "HeartBeatManager.h"
#include "HeartBeatQtModel.h"
#include "log.h"
#include <time.h>
#include "cs2ssSender.h"
#include "ConfigManager.h"
#include "cs2ssCmds.h"
#include "cs2ccNetworkManager.h"
#include "cs2ssNetworkManager.h"
#include "DateTimeUtils.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"
#include "UIAppManager.h"
#include "SessionManager.h"

//////////////////////////////////////////////////////////////////////////
// SvrHeartBeatSender
//////////////////////////////////////////////////////////////////////////
// heartbeat every 29s (server checks every 60s)
#ifdef NDEBUG
const int MS_Server_HeartbeatInterval = 29000;
#else
const int MS_Server_HeartbeatInterval = 4000;
#endif
bool SvrHeartBeatSender::init()
{
    qRegisterMetaType<HeartBeatQtModel>("HeartBeatQtModel");

    m_heartBeatTimer.reset(new QTimer);
    m_heartBeatTimer->setInterval(MS_Server_HeartbeatInterval);
    connect(m_heartBeatTimer.data(), SIGNAL(timeout()),
        this, SLOT(sendHeartbeatSlot()));
    m_heartBeatTimer->start();

    return true;
}

bool SvrHeartBeatSender::connectSigSlot()
{
    return true;
}

// do nothing
void SvrHeartBeatSender::procHeartBeatReply( const HeartBeatQtModel &qtModel )
{
    m_lastHeartBeatRecvDT = QDateTime::currentDateTime();
}

void SvrHeartBeatSender::sendHeartbeatSlot()
{
    HeartBeatQtModel qtModel(CConfigManager::clientId());

    bool serverConnected = cs2ssNetworkManager::GetInstance()->isServerConnected();
    bool clientAppConnected = cs2ccNetworkManager::GetInstance()->isClientAppConnected();
    if (clientAppConnected && serverConnected)
    {
        cs2ssSender::send(cs2ss::HeartbeatCommand(qtModel));
    }
    else
    {
        LOG_DEBUG4 << "HB skp. s: " << serverConnected 
            << ", ca: " << clientAppConnected;
    }
}



bool ClientHeartBeatChecker::init()
{
    return true;
}

bool ClientHeartBeatChecker::connectSigSlot()
{
    return true;
}

void ClientHeartBeatChecker::procCCHeartBeat( const HeartBeatQtModel &qtModel )
{
    m_appLastHeartBeatRecvDT = QDateTime::currentDateTime();

    HeartBeatQtModel replyModel(HB_C_Service);
    if (SessionManager::GetInstance()->isSessionIdSynced())
    {
        // only send out our session id when we can confirm it (from server);
        replyModel.SessionId_  = SessionManager::GetInstance()->currentSessionId();
        replyModel.IsSessionIdReady_ = true;
    } else {
        replyModel.IsSessionIdReady_ = false;
    }
    cs2ccSender::send(cs2cc::HeartbeatCommand(replyModel));

    UIAppManager::GetInstance()->onAppHBRecved(qtModel.Pid_);
    SessionManager::GetInstance()->onAppHBRecved(qtModel.SessionId_);
}

void ClientHeartBeatChecker::procCLHeartBeat( const HeartBeatQtModel &qtModel )
{
    m_lockerLastHeartBeatRecvDT = QDateTime::currentDateTime();

    HeartBeatQtModel replyModel(HB_C_Service);
    if (SessionManager::GetInstance()->isSessionIdSynced())
    {
        // only send out our session id when we can confirm it (from server);
        replyModel.SessionId_  = SessionManager::GetInstance()->currentSessionId();
        replyModel.IsSessionIdReady_ = true;
    } else {
        replyModel.IsSessionIdReady_ = false;
    }
    cs2clSender::send(cs2cl::HeartbeatCommand(replyModel));

    UIAppManager::GetInstance()->onLockerHBRecved(qtModel.Pid_);
    SessionManager::GetInstance()->onLockerHBRecved(qtModel.SessionId_);
}
