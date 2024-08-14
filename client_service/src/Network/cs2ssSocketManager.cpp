#include "cs2ssSocketManager.h"

#include <QTcpSocket>
#include <QStringList>
#include <QMutexLocker>

#include "Client.h"


#include "log.h"
#include "Utils.h"

#include "ConfigManager.h"
#include "CommonSettings.h"
#include "NetworkSettings.h"
#include "NetworkStatus.h"
#include "ss2csCmdProcs.h"
#include <winsock2.h>
#include <MSTcpIP.h>
//#include <winsock2.h>

// at the very beginning, tries to re-connect after 200 ms
// each time the connection failed, increase the interval by 100 ms
// the maximum interval wouldn't be longer than 1000 ms
const int Connect_Interval_Base = 100;
const int Connect_Interval_Step = 50;
const int Connect_Interval_Max = 500;

cs2ssSocketManager_p::cs2ssSocketManager_p( int port )
{
    m_port = port;
    m_isActiveClose = false;

    m_socketStatus = SocketStatus_Stopped;

    _initSocket();

    m_reconnectTimer = new QTimer();
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, SIGNAL(timeout()),
        this, SLOT(connectSocket()));
}

void cs2ssSocketManager_p::_initSocket()
{
    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(readyRead()), 
        this, SLOT(readyReadSlot()));
    connect(m_socket, SIGNAL(connected()), 
        this, SLOT(onConnectedSlot()));
    connect(m_socket, SIGNAL(disconnected()), 
        this, SLOT(onDisconnectedSlot()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), 
        this, SLOT(socketErrorSlot(QAbstractSocket::SocketError)));
}

void cs2ssSocketManager_p::_destroySocket()
{
    if (m_socket == NULL)
    {
        return;
    }

    m_socketStatus = SocketStatus_Stopped;

    disconnect(m_socket, SIGNAL(readyRead()), 
        this, SLOT(readyReadSlot()));
    disconnect(m_socket, SIGNAL(connected()), 
        this, SLOT(onConnectedSlot()));
    disconnect(m_socket, SIGNAL(disconnected()), 
        this, SLOT(onDisconnectedSlot()));
    disconnect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), 
        this, SLOT(socketErrorSlot(QAbstractSocket::SocketError)));

    m_socket->abort();
    m_socket->deleteLater();
    m_socket = NULL;
}

void cs2ssSocketManager_p::connectSocket()
{
    if (!m_isAllowedToConnect)
    {
        return;
    }
    // reset this flag to false as it's not in "actively closed" state
    m_isActiveClose = false;

    if (isSocketConnected() == true)
    {
        LOG_WARN << "attempting to connect to server, while the socket says already connected, port: " << m_port;
        onConnectedSlot();
        return;
    }

    _updateClientSocketStatus(SocketStatus_Connecting);
    if (m_socket->state() == QAbstractSocket::ConnectingState)
    {
        LOG_DEBUG << "socket still connecting, skip, port: " << m_port;
        return;
    }

    LOG_DEBUG4 << "try connecting to " << qPrintable(CConfigManager::serverAddress()) 
        << ", port: " << m_port;
    m_socket->connectToHost(CConfigManager::serverAddress(), m_port);
}

void cs2ssSocketManager_p::disconnectSocket()
{
    m_isActiveClose = true;

    LOG_DEBUG4 << "actively disconnect socket to server, port: " << m_port;
    if (m_socket == NULL)
    {
        LOG_ERROR << "socket null";
        return;
    }
    m_socket->disconnectFromHost();
    bool disconnected = m_socket->waitForDisconnected(1000);
    if ( ! disconnected
        && ! (m_socket->state() == QAbstractSocket::UnconnectedState)
        )
    {
        LOG_ERROR << "disconn error: " << qPrintable(m_socket->errorString());
    }

}

void cs2ssSocketManager_p::onConnectedSlot()
{
    LOG_DEBUG4 << "Socket connected to server, port: " << m_port;

    stopReconnectTimer();

    Q_ASSERT(m_socket != NULL);
    if (m_socket == NULL)
    {
        LOG_ERROR << "onconned: cs2ss socket is null, port: " << m_port;
        return;
    }
    // keeplive, notice that sktfd is only meaningful in connected state
    int sktfd = m_socket->socketDescriptor();
    if (sktfd >= 0)
    {
        _keepSocketAlive(sktfd);
    }
    else
    {
        LOG_ERROR << "onconned: sktfd is negative: " << sktfd << " port: " << m_port;
    }

    _updateClientSocketStatus(SocketStatus_Connected);
}

void cs2ssSocketManager_p::_keepSocketAlive( int fd )
{
    tcp_keepalive alive;
    alive.onoff = TRUE;
    alive.keepalivetime = 3000;
    alive.keepaliveinterval = 1000;
    DWORD bytes_ret = 0;
    int result = WSAIoctl(fd, SIO_KEEPALIVE_VALS, &alive, sizeof(alive), 
        NULL, 0, &bytes_ret, NULL, NULL);
    Q_UNUSED(bytes_ret);
    Q_UNUSED(result);
}

void cs2ssSocketManager_p::onDisconnectedSlot()
{
    LOG_DEBUG4 << "Socket disconnected from server, port: " << m_port;

    _updateClientSocketStatus(SocketStatus_Disconnected);

    if (m_isActiveClose == false)
    {
        // try reconnecting, don't directly call connectSocket in this method
        // otherwise if the server is off, then connect--disconnected--connect, it's
        // a deadloop
        restartReconnectTimer();
    }
}

void cs2ssSocketManager_p::readyReadSlot()
{
    // don't add locks here.
    // example: if here has a lock,
    // then in Process() method, we pop up a message box
    // then until user closes the message box, this lock won't
    // be released
    //static QMutex mutex;
    //QMutexLocker locker(&mutex);
    ss2csCmdProcs::GetInstance()->Process(m_socket);
}

bool cs2ssSocketManager_p::isSocketConnected()
{
    return (m_socketStatus == SocketStatus_Connected);
}

void cs2ssSocketManager_p::socketErrorSlot( QAbstractSocket::SocketError error )
{
    LOG_DEBUG4 << "Connecting to server error. code: " << error
        << ". errorString: " << qPrintable(m_socket->errorString()) << " port: " << m_port;
    // this method might be triggered from a worker thread, so 
    // have to use "QMetaObject::invokeMethod" to make the cross-thread call
    // https://bugreports.qt-project.org/browse/QTBUG-18082 for more details
    // don't call connectSocket here, use timer to control the speed
    // otherwise, connectSocket()->socketErrorSlot()->connectSocket(), will
    // be triggered very quickly

    // if the previous status is "connected" and the socket is actually connected, don't reconnect, just skip
    // scenario: the previous socket connection is still trying, and immediately after it gets connected,
    // this method is triggered (as this is called asyncly), then it's thought connection error and 
    // re-connect(force disconnecting the old connection & establish a new one), so the already-connected
    // socket will be closed.
    // and even worse is: this mey be a endless loop (connecting->connected->socket error trigged asyncly->disconnect
    // ->connecting-> again)
    if (m_socketStatus == SocketStatus_Connected)
    {
        if (m_socket != NULL &&
            m_socket->state() == QAbstractSocket::ConnectedState
           )
        {
            return;
        }
    }

    // if the previous status is "connecting", then don't change to "disconnected"
    // just pretend that we're keeping connecting
    if (m_socketStatus != SocketStatus_Connecting)
    {
        _updateClientSocketStatus(SocketStatus_Disconnected);
    }

    if (m_isActiveClose == false)
    {
        restartReconnectTimer();
    }
}

static int s_interval = -1;
int cs2ssSocketManager_p::_getProperConnectInterval()
{
    if (s_interval < 0)
    {
        s_interval = Connect_Interval_Base;
        return s_interval;
    }

    if (s_interval < Connect_Interval_Max)
    {
        s_interval += Connect_Interval_Step;
    }

    return s_interval;
}

void cs2ssSocketManager_p::_resetConnectInterval()
{
    s_interval = -1;
}

void cs2ssSocketManager_p::restartReconnectTimer()
{
    if ( m_reconnectTimer->isActive())
    {
        m_reconnectTimer->stop();
    }

    if (s_interval < 0)
    {
        _getProperConnectInterval();
    }

    LOG_DEBUG4 << "try reconnecting to socket in " << s_interval << " ms, port: " << m_port;;
    m_reconnectTimer->setSingleShot(true);
    m_reconnectTimer->start(s_interval);
}

void cs2ssSocketManager_p::stopReconnectTimer()
{
    LOG_DEBUG4 << "stop trying reconnecting to socket " << " port: " << m_port;;

    _resetConnectInterval();
    if (m_reconnectTimer->isActive())
    {
        m_reconnectTimer->stop();
    }
}

void cs2ssSocketManager_p::_updateClientSocketStatus( SocketStatus socketStatus )
{
    if (m_socketStatus != socketStatus)
    {
        LOG_DEBUG4 << "socket status change to: " << qPrintable(SocketStatustoQString(socketStatus)) << " port: " << m_port;;
        m_socketStatus = socketStatus;
        emit serverSocketStatusChanged(m_socketStatus);
    }
}

void cs2ssSocketManager_p::reconnectSocket(int delayInterval)
{
    disconnectSocket();

    if (delayInterval < 0)
    {
        s_interval = _getProperConnectInterval();
    }
    else
    {
        s_interval = delayInterval;
    }

    restartReconnectTimer();
}



bool cs2ssSocketManager::init()
{
    m_socketManager = new cs2ssSocketManager_p(DEFAULT_PORT);
    m_bigSocketManager = new cs2ssSocketManager_p(DEFAULT_BIG_SOCKET_PORT);
    // normal socket is always allowed to connect
    m_socketManager->m_isAllowedToConnect = true;
    // big socket can only be connected when normal socket finish performing handhshake-ack.
    m_bigSocketManager->m_isAllowedToConnect = false;
    return true;
}

bool cs2ssSocketManager::connectSigSlot()
{
    connect(m_socketManager, SIGNAL(serverSocketStatusChanged(int)),
        this, SIGNAL(serverSocketStatusChanged(int)));
    connect(m_socketManager, SIGNAL(serverSocketStatusChanged(int)),
        this, SLOT(onServerSocketStatusChanged(int)));
    return true;
}

void cs2ssSocketManager::onServerSocketStatusChanged( /*SocketStatus*/int socketStatus )
{
    // whenever normal socket' status is changed, dont allow big socket to connect
    // it will be allowed to connect immediately after normal socket finishes handshaking.
    m_bigSocketManager->m_isAllowedToConnect = false;
}

void cs2ssSocketManager::connectSocket()
{    
    m_socketManager->connectSocket();
}

void cs2ssSocketManager::connectBigSocket()
{
    m_bigSocketManager->m_isAllowedToConnect = true;
    m_bigSocketManager->connectSocket();
}

void cs2ssSocketManager::disconnectSocket()
{
    m_socketManager->disconnectSocket();
    m_bigSocketManager->disconnectSocket();
}

bool cs2ssSocketManager::isSocketConnected()
{
    return m_socketManager->isSocketConnected();
}

void cs2ssSocketManager::reconnectSocket(int delayInterval)
{
    m_socketManager->reconnectSocket(delayInterval);
    m_bigSocketManager->reconnectSocket(delayInterval);
}

void cs2ssSocketManager::_destroySocket()
{
    m_socketManager->_destroySocket();
    m_bigSocketManager->_destroySocket();
}

void cs2ssSocketManager::_initSocket()
{
    m_socketManager->_initSocket();
    m_bigSocketManager->_initSocket();
}
