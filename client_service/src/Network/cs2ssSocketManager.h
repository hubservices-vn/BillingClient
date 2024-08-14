#ifndef CS2SSSOCKETMANAGER_H
#define CS2SSSOCKETMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QTcpSocket>
#include <QSharedPointer>
#include "NetworkStatus.h"
#include "QtTypePtrs.h"


class cs2ssSocketManager_p : public QObject
{
    Q_OBJECT

public:
    cs2ssSocketManager_p(int port);
    
public:
    // methods

    // call this method carefully, client will not auto-reconnect if this method is called, and all pending data
    // in the socket stream will be discarded
    void disconnectSocket(); 

    // dicsonnect and connect to server, after specific delay (default is -1, means immediate reconnect)
    void reconnectSocket(int delayInterval = -1);

    // accessors
    QTcpSocket* socket() const { return m_socket; }
    SocketStatus socketStatus() const { return m_socketStatus; }

    bool isSocketConnected();

signals:
    // the real "connected" should be "conn acked", not just the socket connected
    void serverSocketStatusChanged(/*SocketStatus*/int status);

public slots:
    void connectSocket();

    // This is a function we'll connect to a socket's readyRead()
    // signal, which tells us there's text to be read from the chat
    // server.
    void readyReadSlot();
    void onConnectedSlot();
    void onDisconnectedSlot();
    void socketErrorSlot(QAbstractSocket::SocketError error);

private:
    void _updateClientSocketStatus(SocketStatus socketStatus);


private:
    void restartReconnectTimer();
    void stopReconnectTimer();
    int _getProperConnectInterval();
    // after connected, should reset the connection interval back to default value
    // otherwise the connection interval will not decrease
    void _resetConnectInterval();
    void _keepSocketAlive( int fd );

    void _initSocket();
    void _destroySocket();
    
public:
    bool m_isAllowedToConnect;
private:
    QTcpSocket* m_socket;
    SocketStatus m_socketStatus;

    // when server tells client to shutdown, the client will actively disconnect the socket and attempts
    // to shutdown the pc, but shortly after disconnectSocket called, the "auto reconnect" mechanism works
    // and client gets re-connected before system starts to shutdown
    // so, if we actively invokes disconnectSocket method, this flag will be marked as true,
    // so that client will not tries to auto-reconnect
    bool m_isActiveClose;
    int m_port;

    // single shot timer (the interval varies)
    QTimer* m_reconnectTimer;

    friend class cs2ssSocketManager;
};

class cs2ssSocketManager : public QObject, public Singleton_T<cs2ssSocketManager>
{
    Q_OBJECT

public:
    bool init();
    bool connectSigSlot();

public:
    // methods

    // call this method carefully, client will not auto-reconnect if this method is called, and all pending data
    // in the socket stream will be discarded
    void disconnectSocket(); 

    // dicsonnect and connect to server, after specific delay (default is -1, means immediate reconnect)
    void reconnectSocket(int delayInterval = -1);

    // accessors
    QTcpSocket* socket() const { return m_socketManager->socket(); }
    SocketStatus socketStatus() const { return m_socketManager->socketStatus(); }

    bool isSocketConnected();

signals:
    // the real "connected" should be "conn acked", not just the socket connected
    void serverSocketStatusChanged(/*SocketStatus*/int status);

    public slots:
        void connectSocket();
        void connectBigSocket();
        void onServerSocketStatusChanged( /*SocketStatus*/int socketStatus );
        // This is a function we'll connect to a socket's readyRead()
        // signal, which tells us there's text to be read from the chat
        // server.
        /*void readyReadSlot();
        void onConnectedSlot();
        void onDisconnectedSlot();
        void socketErrorSlot(QAbstractSocket::SocketError error);*/

private:
    void _updateClientSocketStatus(SocketStatus socketStatus);

    // only used in network manager, to re-init a new TCP socket
    void _initSocket();
    void _destroySocket();

private:
    void restartReconnectTimer();
    void stopReconnectTimer();
    int _getProperConnectInterval();
    // after connected, should reset the connection interval back to default value
    // otherwise the connection interval will not decrease
    void _resetConnectInterval();
    void _keepSocketAlive( int fd );
private:
    cs2ssSocketManager_p* m_socketManager;
    cs2ssSocketManager_p* m_bigSocketManager;

    friend class cs2ssNetworkManager;
};

#endif // CS2SSSOCKETMANAGER_H
