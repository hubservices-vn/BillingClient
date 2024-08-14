#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include "stdTypes.h"

class Session;
class OfflineSession;
class QTimer;
enum SocketStatus;
class SessionBeginQtModel;
class SessionEndQtModel;
class SessionStatusQtModel;
class SessionManager : public QObject, public Singleton_T<SessionManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    void endSession(bool lockUI = true);

public: // cmd handler
    void procSessionBegin(const SessionBeginQtModel &qtModel);

    // when user clicks "log out", will end the active session
    void procSessionEnd(const SessionEndQtModel &qtModel);

    void procSessionStatus( const SessionStatusQtModel &qtModel );
public: // cmd sender
    void broadcaseSessionState();

public: // helper
    Session* CurrentSession() const { return m_session; }

    bool hasLiveSession() const;

    int currentSessionId() const;

    OfflineSession* offlineSession() const;

    int totalMinutes() const;

    int minutesUsed() const;

    int minutesLeft() const;

    double timeFee() const;

    double serviceFee() const;
    
    void onAppHBRecved( quint64 ssId );
    void onLockerHBRecved( quint64 ssId );
    bool isSessionIdSynced();
signals:
    // possible case: 
    // 1) offline session recv this signal, then stop updating offline session
    // 2) user account manager clears the current account
    void sessionEnded();
    
public slots:
    // OfflineSessionManager reports there's a new update, need to update UI
    void offlineSessionUpdatedSlot( OfflineSession* offlineSession );
    void adminCodeChangeCorrectlySlot();
private:
    void _updateSessionStatusFromServer(uint64_t sessionId, int minsUsed, 
        int minsLeft, double timeFee, double serviceFee);
    void proceedSessionIdMismatch( quint64 ssId );
private:
    Session* m_session;
    bool m_isSyncedWithServer;
};

#endif // SESSIONMANAGER_H
