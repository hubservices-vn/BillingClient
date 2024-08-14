#include "cl2csSocketManager.h"

#include <QStringList>
#include <QMutexLocker>
#include <QTimer>
#include <winsock2.h>
#include <MSTcpIP.h>

#include "Client.h"

#include "log.h"
#include "Utils.h"

#include "ConfigManager.h"
#include "CommonSettings.h"
#include "NetworkSettings.h"
#include "NetworkStatus.h"
#include "ClientNetworkSettings.h"
#include "cs2clCmdProcs.h"

// at the very beginning, tries to re-connect after 50ms
// each time the connection failed, increase the interval by 25ms
// the maximum interval wouldn't be longer than 2500ms
// (it connects to service via named pipe, so wouldn't take too long)
#ifdef NDEBUG
const int Connect_Interval_Base = 50;
const int Connect_Interval_Step = 25;
const int Connect_Interval_Max = 1000;
#else
const int Connect_Interval_Base = 50;
const int Connect_Interval_Step = 25;
const int Connect_Interval_Max = 1000;
//const int Connect_Interval_Base = 5000;
//const int Connect_Interval_Step = 5000;
//const int Connect_Interval_Max = 500000;
#endif


cl2csSocketManager::~cl2csSocketManager()
{
    disconnectSocket();
    if (m_socket != NULL)
    {
        m_socket->deleteLater();
        m_socket = NULL;
    }
}

bool cl2csSocketManager::init()
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocketError");

    m_socketStatus = SocketStatus_Stopped;

    m_socket = new QTcpSocket();

    connect(m_socket, SIGNAL(readyRead()), 
        this, SLOT(readyReadSlot()));
    connect(m_socket, SIGNAL(connected()), 
        this, SLOT(onConnectedSlot()));
    connect(m_socket, SIGNAL(disconnected()), 
        this, SLOT(onDisconnectedSlot()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), 
        this, SLOT(socketErrorSlot(QAbstractSocket::SocketError)));

    m_reconnectTimer = new QTimer();
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, SIGNAL(timeout()),
        this, SLOT(connectSocket()));

    return true;
}

bool cl2csSocketManager::connectSigSlot()
{
    return true;
}

void cl2csSocketManager::connectSocket()
{
    if (isSocketConnected() == true)
    {
        onConnectedSlot();
        return;
    }

    _updateClientSocketStatus(SocketStatus_Connecting);

    if (m_socket->state() == QAbstractSocket::ConnectingState)
    {
        return;
    }

    LOG_DEBUG4 << "connect to client service: " << qPrintable(LOCKER_SERVICE_ADDRESS)
        << ", port: " << LOCKER_SERVICE_PORT;
    m_socket->connectToHost(LOCKER_SERVICE_ADDRESS, LOCKER_SERVICE_PORT);
}

void cl2csSocketManager::disconnectSocket()
{
    if (isSocketConnected() &&
        m_socket != NULL)
    {
        m_socket->disconnectFromHost();
    }
}

void cl2csSocketManager::onConnectedSlot()
{
    LOG_DEBUG4 << "client app Socket connected to client service";
    stopReconnectTimer();
    _updateClientSocketStatus(SocketStatus_Connected);
}

void cl2csSocketManager::onDisconnectedSlot()
{
    LOG_DEBUG4 << "Socket disconnected from client service";

    // if the previous status is "connecting", then don't change to "disconnected"
    // just pretend that we're keeping connecting
    if (m_socketStatus != SocketStatus_Connecting)
    {
        _updateClientSocketStatus(SocketStatus_Disconnected);
    }

    // try reconnecting, don't directly call connectSocket in this method
    // otherwise if the server is off, then connect--disconnected--connect, it's
    // a deadloop
    restartReconnectTimer();
}

//static QMutex s_sktReadMutex;
void cl2csSocketManager::readyReadSlot()
{
    cs2clCmdProcs::GetInstance()->Process(m_socket);
}

bool cl2csSocketManager::isSocketConnected()
{
    return (m_socketStatus == SocketStatus_Connected);
}

void cl2csSocketManager::socketErrorSlot( QAbstractSocket::SocketError error )
{
    LOG_DEBUG4 << "socket connecting return error: " << error;
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
    restartReconnectTimer();
    //QMetaObject::invokeMethod(this, "connectSocket", Qt::QueuedConnection);
}

static int s_interval = -1;
int cl2csSocketManager::_getProperConnectInterval()
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

void cl2csSocketManager::_resetConnectInterval()
{
    s_interval = -1;
}

void cl2csSocketManager::restartReconnectTimer()
{
    if ( m_reconnectTimer->isActive())
    {
        m_reconnectTimer->stop();
    }

    int interval = _getProperConnectInterval();

    LOG_DEBUG4 << "try reconnecting to socket in " << interval << " ms";
    m_reconnectTimer->setSingleShot(true);
    m_reconnectTimer->start(interval);
}

void cl2csSocketManager::stopReconnectTimer()
{
    LOG_DEBUG4 << "stop trying reconnecting to socket";

    _resetConnectInterval();
    if (m_reconnectTimer->isActive())
    {
        m_reconnectTimer->stop();
    }
}

void cl2csSocketManager::_updateClientSocketStatus( SocketStatus socketStatus )
{
    if (m_socketStatus != socketStatus)
    {
        LOG_DEBUG4 << "socket status " << qPrintable(SocketStatustoQString(m_socketStatus))
            << " change to: " << qPrintable(SocketStatustoQString(socketStatus));
        m_socketStatus = socketStatus;
        emit socketStatusChanged(m_socketStatus);
    }
}
