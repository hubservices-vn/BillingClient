#include "cs2ccSocketManager.h"
#include <QLocalSocket>
#include "log.h"
#include "Utils.h"
#include "cs2ssSocketManager.h"
#include "cc2csCmdProcs.h"

// defined in ClientNetworkSettings.cpp
extern const int APP_SERVICE_PORT;
// defined in CommonNetworkSettings.cpp
extern const int SOCKET_FAIL_COUNT;

extern const int SOCKET_WAIT_COUNT;


bool cs2ccSocketManager::init()
{
    m_socket = NULL;
    m_SocketStatus = SocketStatus_Stopped;

    return true;
}

bool cs2ccSocketManager::connectSigSlot()
{
    return true;
}

bool cs2ccSocketManager::startListen()
{
    if (this->listen(QHostAddress::Any, APP_SERVICE_PORT) == false)
    {
        LOG_ERROR << "listen to port: " << APP_SERVICE_PORT << " failed";
        return false;
    }

    //LOG_DEBUG4 << "listen to  port: " << CLIENT_SERVICE_PORT << " succeed";
    return true;
}

bool cs2ccSocketManager::stopListen()
{
    LOG_DEBUG << "stop listening to cc";
    closeSocket();
    this->close();

    return true;
}

void cs2ccSocketManager::incomingConnection( int socketfd )
{
    LOG_DEBUG << "new conn, socketfd: " << socketfd;

    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(socketfd);

    // store to pending list
    _addPendingSocket(socket);

    connect(socket, SIGNAL(readyRead()), 
        this, SLOT(readyReadSlot()));
    connect(socket, SIGNAL(disconnected()), 
        this, SLOT(onDisconnectedSlot()));
}

bool cs2ccSocketManager::closeSocket()
{
    if (m_socket != NULL)
    {
        m_socket->abort();
    }
    return true;
}

// client socket sends some request, directly relays to server side
void cs2ccSocketManager::readyReadSlot()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == NULL)
    {
        return;
    }

    // if in pending list, check whether it's an invalid/malicious connection
    bool isValid = _validateSocket(socket);
    if ( ! isValid)
    {
        return;
    }

    // try to parse and process the socket
    CmdProcResults result = cc2csCmdProcs::GetInstance()->Process(socket);

    _updateSocketProcResult(socket, result);
}

void cs2ccSocketManager::onDisconnectedSlot()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == NULL)
    {
        LOG_ERROR << "skt nulldisconnected";
        return;
    }

    // a pending client gets disconnected, close the socket
    {
        if (m_pendingSockets.contains(socket))
        {
            LOG_DEBUG4 << "pending socket disconnected";
            m_pendingSockets.remove(socket);
            socket->deleteLater();
            return;
        }
    }

    // the client socket disconnected
    if (socket == m_socket)
    {
        LOG_DEBUG4 << "client socket disconnected";
        //emit clientSocketDisconnected();
        _updateSocketStatus(SocketStatus_Disconnected);

        m_socket->deleteLater();
        m_socket = NULL;
    }
}

bool cs2ccSocketManager::isSocketConnected()
{
    return (m_socket != NULL &&
        m_SocketStatus == SocketStatus_Connected);
}

void cs2ccSocketManager::_updateSocketStatus( SocketStatus socketStatus )
{
    if (m_SocketStatus != socketStatus)
    {
        LOG_DEBUG4 << "client app socket status changed from: " << qPrintable(SocketStatustoQString(m_SocketStatus))
            << " to: " << qPrintable(SocketStatustoQString(socketStatus));
        m_SocketStatus = socketStatus;
        emit socketStatusChanged(m_SocketStatus);
    }
}

void cs2ccSocketManager::_addPendingSocket( QTcpSocket* socket )
{
    if (socket == NULL)
    {
        return;
    }
    LOG_DEBUG << "pend skt " << socket 
        << ", sktfd " << socket->socketDescriptor();
    {
        if (m_pendingSockets.contains(socket))
        {
            LOG_ERROR << "skt " << socket
                << " already pend";
        }
        m_pendingSockets[socket] = PendingSocketInfo(0, 0);
    }
}

bool cs2ccSocketManager::acceptSocket( QTcpSocket* socket )
{
    if (socket == NULL)
    {
        LOG_ERROR << "skt NULL acpt " << socket;
        return false;
    }
    // remove from pending list
    if (m_pendingSockets.contains(socket))
    {
        m_pendingSockets.remove(socket);
    }
    else
    {
        int socketfd = socket->socketDescriptor();
        LOG_WARN << "sktfd " << socketfd << " not in pending list";
    }

    if (m_socket != NULL)
    {
        if (m_socket == socket)
        {
            // same socket stored, don't handle repeatedly
            return true;
        }
        else
        {
            LOG_ERROR << "different skt stored: " << m_socket;
            m_socket->deleteLater();
            m_socket = NULL;
        }
    }
    m_socket = socket;

    // keeplive, notice that sktfd is only meaningful in connected state
    int sktfd = m_socket->socketDescriptor();
    if (sktfd >= 0)
    {
        //_keepSocketAlive(sktfd);
    }
    else
    {
        LOG_ERROR << "onconned: sktfd is negative: " << sktfd;
    }

    _updateSocketStatus(SocketStatus_Connected);
    return true;
}

bool cs2ccSocketManager::_validateSocket( QTcpSocket* socket )
{
    if (socket == NULL)
    {
        return false;
    }

    if ( ! m_pendingSockets.contains(socket))
    {
        return true;
    }
    PendingSocketInfo sktInfo = m_pendingSockets[socket];
    if (sktInfo.FailCount < SOCKET_FAIL_COUNT ||
        sktInfo.WaitingCount < SOCKET_WAIT_COUNT)
    {
        return true;
    }
    LOG_DEBUG << "reject pending skt " << socket->socketDescriptor()
        << sktInfo.FailCount
        << " / " << sktInfo.WaitingCount;
    m_pendingSockets.remove(socket);
    socket->deleteLater();
    return false;
}

bool cs2ccSocketManager::_updateSocketProcResult( QTcpSocket* socket, CmdProcResults result )
{
    if (socket == NULL)
    {
        return false;
    }

    if ( ! m_pendingSockets.contains(socket))
    {
        return true;
    }

    PendingSocketInfo sktInfo = m_pendingSockets[socket];
    if (result == CmdProcResults_Handled)
    {
        m_pendingSockets[socket] = PendingSocketInfo(0, 0);
    }
    else if (result == CmdProcResults_Waiting)
    {
        m_pendingSockets[socket] = PendingSocketInfo(
            sktInfo.FailCount,
            sktInfo.WaitingCount + 1);
    }
    else if (result == CmdProcResults_Incompatible ||
        result == CmdProcResults_Failed ||
        result == CmdProcResults_NA)
    {
        m_pendingSockets[socket] = PendingSocketInfo(
            sktInfo.FailCount + 1,
            sktInfo.WaitingCount);
    }
    return true;
}

bool cs2ccSocketManager::rejectPendingSocket(QTcpSocket* socket)
{
    if (socket == NULL)
    {
        LOG_WARN << "rej pending NULL skt";
        return false;
    }

    if ( ! m_pendingSockets.contains(socket))
    {
        LOG_WARN << "rej pending skt not stored: " << socket;
        return false;
    }

    LOG_DEBUG4 << "rej pending skt";
    m_pendingSockets.remove(socket);
    socket->deleteLater();
    return true;
}
