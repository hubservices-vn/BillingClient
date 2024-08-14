#ifndef CC2CSSOCKETMANAGER_H
#define CC2CSSOCKETMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QTcpSocket>
#include "QtTypePtrs.h"

enum SocketStatus;

Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
class cc2csSocketManager : public QObject, public Singleton_T<cc2csSocketManager>
{
    Q_OBJECT
public:
    virtual ~cc2csSocketManager();

public:
    //bool init();
    //bool connectSigSlot();

public:// methods
    void disconnectSocket(); // after disconnected, will auto-reconnect

    // accessors
    QTcpSocket* socket() const { return m_socket; }
    SocketStatus socketStatus() const { return m_socketStatus; }

    bool isSocketConnected();

signals:
    // the real "connected" should be "conn acked", not just the socket connected
    void socketStatusChanged(int status);

public slots:
    bool init();
    bool connectSigSlot();

    void connectSocket(); 

    // This is a function we'll connect to a socket's readyRead()
    // signal, which tells us there's text to be read from the chat
    // server.
    void readyReadSlot();
    void onConnectedSlot();
    void onDisconnectedSlot();
    void socketErrorSlot(QAbstractSocket::SocketError error);

private:
    // if connection fails, then increase the re-try interval
    int _getProperConnectInterval();
    // after connected, should reset the connection interval back to default value
    // otherwise the connection interval will not decrease
    void _resetConnectInterval();

private:
    void _updateClientSocketStatus(SocketStatus socketStatus);

private:
    void restartReconnectTimer();
    void stopReconnectTimer();

private:
    QTcpSocket* m_socket;
    SocketStatus m_socketStatus;

    // single shot timer (the interval varies)
    QTimer* m_reconnectTimer;
};
#endif // CC2CSSOCKETMANAGER_H
