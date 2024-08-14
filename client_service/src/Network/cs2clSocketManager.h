#ifndef CS2CL_SOCKETMANAGER_H
#define CS2CL_SOCKETMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QList>
#include <QMap>
#include <QTcpServer>
#include "NetworkStatus.h"
#include "QtTypePtrs.h"
#include "PendingSocketInfo.h"

class QTcpSocket;
enum CmdProcResults;
class cs2clSocketManager : 
    public QTcpServer, 
    public Singleton_T<cs2clSocketManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public:
    bool startListen();
    bool stopListen();

public: // client locker
    bool closeSocket();
    QTcpSocket* socket() const { return m_socket; }
    bool isSocketConnected();
    SocketStatus socketStatus() const { return m_SocketStatus; }

public:
    // the socket is accepted by service, so remove it from pending list and store it
    bool acceptSocket(QTcpSocket* socket);
    
    bool rejectPendingSocket(QTcpSocket* socket);
signals:
    void socketStatusChanged(/*SocketStatus*/int statusVal);

    public slots:
        void readyReadSlot();
        void onDisconnectedSlot();

protected:
    void incomingConnection(int socketfd);

private:
    void _addPendingSocket(QTcpSocket* socket);
    

private:
    bool _validateSocket(QTcpSocket* socket);
    bool _updateSocketProcResult(
        QTcpSocket* socket, CmdProcResults result);
private:
    void _updateSocketStatus(SocketStatus socketStatus);

private:
    QTcpSocket* m_socket;
    SocketStatus m_SocketStatus;

    QMap<QTcpSocket*, PendingSocketInfo> m_pendingSockets;
};

#endif // CS2CL_SOCKETMANAGER_H
