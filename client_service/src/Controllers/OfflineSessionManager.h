#ifndef OFFLINESESSIONMANAGER_H
#define OFFLINESESSIONMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QTimer>
#include "stdTypes.h"

class OfflineSession;
class OfflineSessionDB;
enum SocketStatus;
enum UserGroupType;
class OfflineSessionReplyQtModel;
class OfflineSessionManager : public QObject, public Singleton_T<OfflineSessionManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    OfflineSession* userOfflineSession();

    // check whether the offline session db has some offline sessions;
    bool hasOfflineSessions();

    bool hasActiveOfflineSession();

    // the offline time has been recorded and handled by server, 
    // need to clear the offline session from client
    void procOfflineSessionReply(const OfflineSessionReplyQtModel &model);

    OfflineSession* adminOfflineSession()
    {
        return m_adminOfflineSession;
    }
    //void addAdminOfflineSession(quint64 currentSessionId, const QDateTime& startDT, 
    //    int accId, UserGroupType accType);
signals:
    void offlineSessionUpdated(OfflineSession* offlineSession);

public slots:
    // if status changed to disconnected, we start the offline time calculation
    void serverSocketStatusChangedSlot(/*SocketStatus*/int status);

    // server connected, report all offline sessions to server
    void serverConnectedSlot();

    // if the session is ended, we should freeze the offline session and stop the timer
    // (but should keep the offline session file, so next time when the handshake happens,
    // we can read data from the offline session file and report to server)
    void sessionEndedSlot();

    // the client doesn't connect to server, and ADMIN logs in
    // in this case there will be an offline session
    void accLoggedInSlot();

    void increaseOfflineTime();

private:
    bool beginUserOfflineSession();
    bool beginAdminOfflineSession();
    bool stopOfflineSession();
    bool clearOfflineSessionRecord();
    void endHistoricalOfflineSession();
private:
    OfflineSessionDB* m_db;
    QTimer* m_offlineTimer;
    OfflineSession* m_adminOfflineSession;
};

#endif // OFFLINESESSIONMANAGER_H
