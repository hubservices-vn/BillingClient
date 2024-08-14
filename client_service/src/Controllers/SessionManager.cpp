#include "SessionManager.h"
#include "Session.h"
#include <QTimer>
#include "UserAccount.h"
#include "UserAccountManager.h"
#include "CommonSettings.h"
#include "OfflineSessionManager.h"
#include "OfflineSession.h"
#include "SessionBeginQtModel.h"
#include "SessionEndQtModel.h"
#include "cs2ccSender.h"
#include "SessionStatusQtModel.h"
#include "ss2csCmds.h"
#include "cs2ccCmds.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"
#include "ClientSettingManager.h"
bool SessionManager::init()
{
    m_session = NULL;
    m_isSyncedWithServer = false;
    return true;
}

bool SessionManager::connectSigSlot()
{
    // the offline session gets updated
    OfflineSessionManager* offlineMgr = OfflineSessionManager::GetInstance();
    connect(offlineMgr, SIGNAL(offlineSessionUpdated(OfflineSession*)),
        this, SLOT(offlineSessionUpdatedSlot(OfflineSession*)));
    connect(ClientSettingManager::GetInstance(), SIGNAL(adminCodeChangePasswordIncorrectly()),
        this, SLOT(adminCodeChangeCorrectlySlot()));
    return true;
}

// helper accessor
OfflineSession* SessionManager::offlineSession() const
{
    return OfflineSessionManager::GetInstance()->userOfflineSession();
}

void SessionManager::_updateSessionStatusFromServer(
    uint64_t sessionId, int minsUsed, int minsLeft,
    double timeFee, double serviceFee)
{
    // this is a new session
    if (m_session == NULL)
    {
        m_session = new Session(sessionId, minsUsed, minsLeft, timeFee, serviceFee);
    }
    else
    {
        if (m_session->isAlive() == false ||
            m_session->sessionId() != sessionId)
        {
            delete m_session;
            m_session = NULL;

            m_session = new Session(sessionId, minsUsed, minsLeft, timeFee, serviceFee);
        }
        m_session->updateValue(minsUsed, minsLeft, timeFee, serviceFee);
    }

    broadcaseSessionState();
}

void SessionManager::offlineSessionUpdatedSlot( OfflineSession* offlineSession )
{
    // case: the server crashes, ADMIN login with local admin account
    // in this case, when the offlineSessionUpdated signal raises,
    // the minutes left is always 0, so for local admin account
    // always fire sessionUpdated() signal only
    // (otherwise the sessionOOT signal will always be fired)
    if (UserAccountManager::GetInstance()->currentUserType() == UserGroup_LocalSuperAdmin)
    {
        broadcaseSessionState();
    }
    else
    {
        if (minutesLeft() > 0)
        {
            broadcaseSessionState();
        }
        else
        {
            SessionEndQtModel qtModel;
            procSessionEnd(qtModel);
        }
    }
}

int SessionManager::totalMinutes() const
{
    if (UserAccountManager::currentUserId() == 1 &&
        UserAccountManager::currentUserType() == UserGroup_LocalSuperAdmin)
    {
        return 0;
    }

    int totalMinutes = 0;
    if (m_session != NULL)
    {
        totalMinutes = totalMinutes + m_session->MinutesLeft() + m_session->MinutesUsed();
    }

    return totalMinutes;
}

int SessionManager::minutesUsed() const
{
    //if (UserAccountManager::currentUserId() == 1 &&
    //    UserAccountManager::currentUserType() == UserGroup_LocalSuperAdmin)
    //{
    //    return 0;
    //}
    
    int minutesUsed = 0;
    if (m_session != NULL)
    {
        minutesUsed += m_session->MinutesUsed();
    }

    OfflineSession* currentOfflineSession = NULL;
    
    if(UserAccountManager::GetInstance()->currentUserType() 
        == UserGroup_LocalSuperAdmin)
    {
        currentOfflineSession = OfflineSessionManager::GetInstance()->adminOfflineSession();
    }
    else
    {
        currentOfflineSession = offlineSession();
    }

    if(currentOfflineSession != NULL
        && currentOfflineSession->isDeleted() == false)
    {
        minutesUsed += currentOfflineSession->OfflineMinutes();
    }

    return minutesUsed;
}

int SessionManager::minutesLeft() const
{
    if (UserAccountManager::currentUserId() == 1 &&
        UserAccountManager::currentUserType() == UserGroup_LocalSuperAdmin)
    {
        return 0;
    }

    int minutesLeft = 0;
    if (m_session != NULL)
    {
        minutesLeft += m_session->MinutesLeft();
    }

    // the offline session is ACTIVE (didn't handled by server)
    if (offlineSession() != NULL &&
        offlineSession()->isDeleted() == false)
    {
        minutesLeft -= offlineSession()->OfflineMinutes();
    }

    return minutesLeft;
}

double SessionManager::timeFee() const
{
    if (m_session != NULL)
    {
        return m_session->TimeFee();
    }

    return 0;
}

double SessionManager::serviceFee() const
{
    if (m_session != NULL)
    {
        return m_session->ServiceFee();
    }

    return 0;
}

int SessionManager::currentSessionId() const
{
    if (m_session != NULL)
    {
        return m_session->sessionId();
    }

    // don't return -1, as in protocalBuffer, this is defined as uint64_t
    return 0;
}

void SessionManager::procSessionBegin( const SessionBeginQtModel &qtModel )
{
    // any time we receive server's command related to session, this mean from this point one, our ssid is updated
    m_isSyncedWithServer = true;
    // unlock the client UI app
    cs2ccSender::send(cs2cc::SessionBeginCommandC(qtModel));

    cs2clSender::send(cs2cl::SessionBeginCommandC(qtModel));
}

void SessionManager::procSessionEnd( const SessionEndQtModel &qtModel )
{
    // any time we receive server's command related to session, this mean from this point one, our ssid is updated
    m_isSyncedWithServer = true;
    endSession();

    UserAccountManager::GetInstance()->logoutFromClient();
}

void SessionManager::endSession(bool lockUI)
{
    if (m_session != NULL)
    {
        delete m_session;
        m_session = NULL;
    }

    SessionEndQtModel qtModel;
    cs2ccSender::send(cs2cc::SessionEndCommandC(qtModel));
    if (lockUI)
    {
        // lock the client UI app
        cs2clSender::send(cs2cl::SessionEndCommandC(qtModel));
    }

    emit sessionEnded();
}

// tell client UI to update the text display
void SessionManager::broadcaseSessionState()
{
    SessionStatusQtModel qtModel;
    qtModel.SessionId = currentSessionId();
    qtModel.MinutesLeft = minutesLeft();
    qtModel.MinutesUsed = minutesUsed();
    qtModel.TimeFee = timeFee();
    qtModel.ServiceFee = serviceFee();

    cs2ccSender::send(cs2cc::SessionDisplayCommandC(qtModel));
}

void SessionManager::procSessionStatus( const SessionStatusQtModel &qtModel )
{
    // any time we receive server's command related to session, this mean from this point one, our ssid is updated
    m_isSyncedWithServer = true;
    _updateSessionStatusFromServer(qtModel.SessionId, qtModel.MinutesUsed, qtModel.MinutesLeft,
        qtModel.TimeFee, qtModel.ServiceFee);
}

bool SessionManager::hasLiveSession() const
{
    return (m_session != NULL 
        && m_session->isAlive());
}

void SessionManager::onAppHBRecved( quint64 ssId )
{
    if (ssId != currentSessionId())
    {
        // some discrepancies between session id in app and service. Sync with server to be sure.
        proceedSessionIdMismatch(ssId);
    }
}

void SessionManager::onLockerHBRecved( quint64 ssId )
{
    if (ssId != currentSessionId())
    {
        // some discrepancies between session id in app and service. Sync with server to be sure.
        proceedSessionIdMismatch(ssId);
    }
}

void SessionManager::proceedSessionIdMismatch( quint64 ssId )
{
    if (m_isSyncedWithServer)
    {
        // if we have already synced our session with server, we can determine what to do if this problem occurs;
        if (currentSessionId() == 0 && ssId > 0){
            // we have no session, but locker/app has session (!!)
            // need to lock immediately because customer is cheating
            SessionEndQtModel qtModel;
            procSessionEnd(qtModel);
        } else {
            // this occurs when one of app / locker is restarted.
            // this is ok, will be corrected when app/locker receives packets from service later.
            // moreover, this means: client is allowed to login (currentSSID > 0), but app and locker
            // does not know about this yet, so customer is (probably) not cheating here. Just chill and wait for a while
        } 
    }
}

bool SessionManager::isSessionIdSynced()
{
    return m_isSyncedWithServer;
}

void SessionManager::adminCodeChangeCorrectlySlot()
{
    // Local super admin still login, but admin code is incorrect
    if(UserAccountManager::GetInstance()->currentUserType() 
        == UserGroup_LocalSuperAdmin)
    {
        // lock client;
        SessionEndQtModel qtModel;
        procSessionEnd(qtModel);
    }
}