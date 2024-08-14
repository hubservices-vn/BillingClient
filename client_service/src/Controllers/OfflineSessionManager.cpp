#include "OfflineSessionManager.h"
#include "OfflineSession.h"
#include "UserAccountManager.h"
#include "UserAccount.h"
#include "SessionManager.h"
#include "CommonSettings.h"
#include "OfflineSessionDB.h"
#include "SessionStatus.h"
#include "NetworkStatus.h"
#include "ConfigManager.h"
#include "OfflineSessionQtModel.h"
#include "cs2ssNetworkManager.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"


#ifndef NDEBUG
// debug
const int OFFLINE_CALC_INTERVAL = 5000; //5s
#else
// release
const int OFFLINE_CALC_INTERVAL = 60000; //60000ms = 60s = 1min
#endif


bool OfflineSessionManager::init()
{
    m_db = new OfflineSessionDB();
    m_db->loadDB();
    m_adminOfflineSession = NULL;

    m_offlineTimer = new QTimer();
    connect(m_offlineTimer, SIGNAL(timeout()), this, SLOT(increaseOfflineTime()));


    return true;
}

bool OfflineSessionManager::connectSigSlot()
{
    {
        // the connections breaks, start the offline timer to calculate the offline time
        // (note that "stop offline time" will be handled by HandshakeAck event, noe the socket event,
        // as we must ensure the server handled the offline time correctly)
        cs2ssSocketManager* socketMgr = cs2ssSocketManager::GetInstance();
        connect(socketMgr, SIGNAL(serverSocketStatusChanged(int)), 
            this, SLOT(serverSocketStatusChangedSlot(int)));
    }

    {
        // the server connected, report all offline sessions to server
        cs2ssNetworkManager* networkMgr = cs2ssNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    // user ends the session, should stop the offline session calculation
    {
        SessionManager* sessionMgr = SessionManager::GetInstance();
        connect(sessionMgr, SIGNAL(sessionEnded()),
            this, SLOT(sessionEndedSlot()));
    }

    // "ADMIN" logs in when the network is OFF, ends the historical session
    {
        UserAccountManager* userAccMgr = UserAccountManager::GetInstance();
        connect(userAccMgr, SIGNAL(accLoggedIn()), 
            this, SLOT(accLoggedInSlot()));
    }

    return true;
}

void OfflineSessionManager::increaseOfflineTime()
{
    if (! hasActiveOfflineSession())
    {
        return;
    }

    if(UserAccountManager::GetInstance()->currentUserType() 
        == UserGroup_LocalSuperAdmin)
    {
        int offlineMins = m_adminOfflineSession->OfflineMinutes() + 1;
        m_adminOfflineSession->SetOfflineRecTime(QDateTime::currentDateTime());
        m_adminOfflineSession->SetOfflineMinutes(offlineMins);
        emit offlineSessionUpdated(m_adminOfflineSession);
        return;
    }

    int offlineMins = userOfflineSession()->OfflineMinutes() + 1;
    m_db->updateOfflineSession(userOfflineSession(), QDateTime::currentDateTime(), offlineMins);

    emit offlineSessionUpdated(userOfflineSession());
    
}

bool OfflineSessionManager::beginUserOfflineSession()
{
    UserAccount* currentUser = UserAccountManager::CurrentUser();
    // don't record local admin sessions
    if (currentUser == NULL)
    {
        return true;
    }

    endHistoricalOfflineSession();

    int currentSessionId = SessionManager::GetInstance()->currentSessionId();
    m_db->addOfflineSession(currentSessionId, QDateTime::currentDateTime(), 
        currentUser->Id(), currentUser->accType());

    if (!m_offlineTimer->isActive())
    {
        m_offlineTimer->start(OFFLINE_CALC_INTERVAL);
    }

    return true;
}

bool OfflineSessionManager::beginAdminOfflineSession()
{
    UserAccount* currentUser = UserAccountManager::CurrentUser();
    if (currentUser == NULL)
    {
        return true;
    }

    endHistoricalOfflineSession();

    quint64 id = 0;
    int currentSessionId = SessionManager::GetInstance()->currentSessionId();
    QDateTime startDT = QDateTime::currentDateTime();
    int accId = currentUser->Id();
    UserGroupType accType = currentUser->accType();

    QDateTime recordDT = startDT;
    int offlineMins = 0;
    QDateTime stopDT = QDateTime();

    m_adminOfflineSession = new OfflineSession(
         id, currentSessionId, startDT, recordDT, stopDT, 
         offlineMins, accId, accType, STATUS_ACTIVE);

    if (!m_offlineTimer->isActive())
    {
        m_offlineTimer->start(OFFLINE_CALC_INTERVAL);
    }
    emit offlineSessionUpdated(m_adminOfflineSession);
    return true;
}
void OfflineSessionManager::serverSocketStatusChangedSlot( /*SocketStatus*/int statusVal )
{
    UserAccount* currentUser = UserAccountManager::GetInstance()->CurrentUser();
    if (currentUser == NULL )//|| currentUser->accType() == UserGroup_LocalSuperAdmin)
    {
        return;
    }

    if (statusVal == SocketStatus_Disconnected)
    {
        if(currentUser->accType() == UserGroup_LocalSuperAdmin)
        {
            beginAdminOfflineSession();
        }
        else
        {
            beginUserOfflineSession();
        }
    }
}

void OfflineSessionManager::sessionEndedSlot()
{
    stopOfflineSession();
}

bool OfflineSessionManager::stopOfflineSession()
{

    if (m_offlineTimer->isActive())
    {
        m_offlineTimer->stop();
    }

    if (userOfflineSession() != NULL)
    {
        m_db->endOfflineSession(userOfflineSession(), QDateTime::currentDateTime(), STATUS_LOGGEDOUT);
    }

    if (m_adminOfflineSession != NULL)
    {
        m_adminOfflineSession->SetOfflineStopTime(QDateTime::currentDateTime());
        m_adminOfflineSession->setStatus(STATUS_LOGGEDOUT);
    }
    return true;
}

// network is off; and user is super admin account, ends all the historical session
void OfflineSessionManager::accLoggedInSlot()
{
    if (cs2ssNetworkManager::GetInstance()->isServerConnected() == true)
    {
        return;
    }

    if (UserAccountManager::GetInstance()->currentUserType() 
        != UserGroup_LocalSuperAdmin)
    {
        return;
    }

    // begin a new offline session
    beginAdminOfflineSession();
}

bool OfflineSessionManager::hasOfflineSessions()
{
    //need add &&m_AdminOfflineSeeion !=NULL 
    return (userOfflineSession() != NULL);
}

// after connected, remove offline session
void OfflineSessionManager::serverConnectedSlot()
{
    // stop the timer
    if (m_offlineTimer->isActive())
    {
        m_offlineTimer->stop();
    }

    m_db->deleteOfflineSessionFromDB();
    if (m_adminOfflineSession != NULL)
    {
        delete m_adminOfflineSession;
        m_adminOfflineSession = NULL;
    }
}

void OfflineSessionManager::endHistoricalOfflineSession()
{
    if (userOfflineSession() != NULL)
    {
        m_db->endOfflineSession(userOfflineSession(), 
            userOfflineSession()->OfflineRecTime(), STATUS_EXPIRED);
    }

    if (m_adminOfflineSession != NULL)
    {
        delete m_adminOfflineSession;
        m_adminOfflineSession = NULL;
    }
}

OfflineSession* OfflineSessionManager::userOfflineSession()
{
    return m_db->m_userOfflineSession;
}

bool OfflineSessionManager::hasActiveOfflineSession()
{
    return (
        (userOfflineSession() != NULL && 
        userOfflineSession()->status() == STATUS_ACTIVE)
        ||
        (m_adminOfflineSession !=NULL && 
        m_adminOfflineSession->status() == STATUS_ACTIVE)
        );
}

void OfflineSessionManager::procOfflineSessionReply( const OfflineSessionReplyQtModel &qtModel )
{
    
}


