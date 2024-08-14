#include "cs2clSocketManager.h"
#include "log.h"
#include "Utils.h"
#include "cs2ssSocketManager.h"
#include "cl2csCmdProcs.h"

// defined in ClientNetworkSettings.cpp
extern const int LOCKER_SERVICE_PORT;
// defined in CommonNetworkSettings.cpp
extern const int SOCKET_FAIL_COUNT;
extern const int SOCKET_WAIT_COUNT;



#pragma comment(lib, "ws2_32.lib")



bool cs2clSocketManager::init()
{
    m_socket = NULL;
    m_SocketStatus = SocketStatus_Stopped;

    return true;
}

bool cs2clSocketManager::connectSigSlot()
{
    return true;
}

bool cs2clSocketManager::startListen()
{
    if (this->listen(QHostAddress::Any, LOCKER_SERVICE_PORT) == false)
    {
        LOG_ERROR << "listen to port: " << LOCKER_SERVICE_PORT << " failed";
        return false;
    }

    return true;
}

bool cs2clSocketManager::stopListen()
{
    LOG_DEBUG << "stop listening to cl";
    closeSocket();
    this->close();

    return true;
}

void cs2clSocketManager::incomingConnection( int socketfd )
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

bool cs2clSocketManager::closeSocket()
{
    if (m_socket != NULL)
    {
        m_socket->abort();
    }
    return true;
}

// client socket sends some request, directly relays to server side
void cs2clSocketManager::readyReadSlot()
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
    CmdProcResults result = cl2csCmdProcs::GetInstance()->Process(socket);

    _updateSocketProcResult(socket, result);
}

void cs2clSocketManager::onDisconnectedSlot()
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

bool cs2clSocketManager::isSocketConnected()
{
    return (m_socket != NULL &&
        m_SocketStatus == SocketStatus_Connected);
}

void cs2clSocketManager::_updateSocketStatus( SocketStatus socketStatus )
{
    if (m_SocketStatus != socketStatus)
    {
        LOG_DEBUG4 << "client locker socket status changed from: " << qPrintable(SocketStatustoQString(m_SocketStatus))
            << " to: " << qPrintable(SocketStatustoQString(socketStatus));
        m_SocketStatus = socketStatus;
        emit socketStatusChanged(m_SocketStatus);
    }
}

void cs2clSocketManager::_addPendingSocket( QTcpSocket* socket )
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

bool cs2clSocketManager::acceptSocket( QTcpSocket* socket )
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

bool cs2clSocketManager::_validateSocket( QTcpSocket* socket )
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

bool cs2clSocketManager::_updateSocketProcResult( QTcpSocket* socket, CmdProcResults result )
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

bool cs2clSocketManager::rejectPendingSocket(QTcpSocket* socket)
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
