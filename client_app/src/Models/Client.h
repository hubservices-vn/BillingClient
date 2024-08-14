#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QScopedPointer>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class WorkThreadManager;
class cc2csSocketManager;
class cc2csNetworkManager;
class UserAccountManager;
class SessionManager;

class Client : public QObject, public Singleton_T<Client>
{
    Q_OBJECT
public:
    ~Client();
    bool init();
    bool connectSigSlot();

public: // methods
    // start the socket connection, begin to communicate with server
    bool startUp();

signals:

public slots:

private:
    // data
    WorkThreadManager* m_threadMgr;
    cc2csSocketManager* m_socketMgr;
    cc2csNetworkManager* m_networkMgr;
    UserAccountManager* m_userAccMgr;
    SessionManager* m_sessionMgr;
};

#endif // CLIENT_H
