#include "cs2clNetworkManager.h"
#include "cs2clSocketManager.h"
#include "cs2clSender.h"
#include "NetworkStatus.h"
#include "cs2ssSocketManager.h"
#include "cs2ssNetworkManager.h"
#include "NetworkStateQtModel.h"
#include "ConfigManager.h"
#include "cs2clCmds.h"
#include "ClientConnectionQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "UserAccountManager.h"
#include "UIAppManager.h"
#include "DateTimeUtils.h"
#include "CLHandshakeQtModel.h"
#include "SessionManager.h"
#include "SessionBeginQtModel.h"


#ifdef NDEBUG
// release
const qint64 HANDSHAKE_TIME_GAP = 30; // 30s
#else
// debug
const qint64 HANDSHAKE_TIME_GAP = 300; // 300s
#endif

bool cs2clNetworkManager::init()
{
    m_lockerNetworkResult = NetworkResult_Stopped;
    return true;
}

bool cs2clNetworkManager::connectSigSlot()
{
    {
        cs2clSocketManager* mgr = cs2clSocketManager::GetInstance();
        connect(mgr, SIGNAL(socketStatusChanged(int)),
            this, SLOT(socketStatusChangedSlot(int)));
    }

    return true;
}

void cs2clNetworkManager::procNetworkStateRequest()
{
    NetworkStateReplyQtModel replyModel (
        CConfigManager::clientId(), 
        cs2ssNetworkManager::GetInstance()->serverNetworkResult());

    cs2clSender::send(cs2cl::NetworkStateReplyCommand(replyModel));
}

void cs2clNetworkManager::socketStatusChangedSlot( /*SocketStatus*/ int socketStatus )
{
    NetworkResult networkResult = NetworkResult_NA;
    switch (socketStatus)
    {
    case SocketStatus_NA:
        networkResult = NetworkResult_NA;
        break;
    case SocketStatus_Stopped:
        networkResult = NetworkResult_Stopped;
        break;
    case SocketStatus_Connected:
        networkResult = NetworkResult_Connected;
        break;
    case SocketStatus_Connecting:
        networkResult = NetworkResult_Connecting;
        break;
    case SocketStatus_Disconnected:
        networkResult = NetworkResult_Disconnected;
        break;
    }

    _updateNetworkResult(networkResult);
}

void cs2clNetworkManager::_updateNetworkResult( NetworkResult networkResult )
{
    if (m_lockerNetworkResult == networkResult)
    {
        return;
    }

    LOG_DEBUG4 << "locker network state changed from: "
        << qPrintable(NetworkResulttoQString(m_lockerNetworkResult))
        << " to: " << qPrintable(NetworkResulttoQString(networkResult));

    m_lockerNetworkResult = networkResult;
    emit socketStatusChangedSlot(m_lockerNetworkResult);

    if (m_lockerNetworkResult == NetworkResult_Connected)
    {
        emit lockerConnected();
    }
    else
    {
        emit lockerDisconnected();
    }
}

bool cs2clNetworkManager::isLockerConnected() const
{
    return m_lockerNetworkResult == NetworkResult_Connected;
}

void cs2clNetworkManager::procCLHandshake( QTcpSocket* socket, const CLHandshakeQtModel& qtModel )
{
    QString ver = qtModel.Version_;
    qint64 timestamp = qtModel.Timestamp_;

    qint64 currentTS = UtcDTUtils::dtToUtcTimestamp(QDateTime::currentDateTime());
    if (qAbs(currentTS - timestamp) < HANDSHAKE_TIME_GAP)
    {
        bool alreadyConnected = cs2clSocketManager::GetInstance()->isSocketConnected();
        if (alreadyConnected)
        {
            QTcpSocket* existingSocket = cs2clSocketManager::GetInstance()->socket();
            if (existingSocket != NULL &&
                existingSocket != socket)
            {
                LOG_DEBUG4 << "the locker conenction is dead . close the dead one";
                // the client with same clientId now connects with a different socket
                // that means the "old" socket stored on server is dead now, go to socketManager to close the "dead" socket
                cs2clSocketManager::GetInstance()->closeSocket();
            }
        }

        // the socket won't be accepted as a valid one until the workstation manager finishes handling it (as the clientId may be wrong from the client side)
        cs2clSocketManager::GetInstance()->acceptSocket(socket);

        CLHandshakeAckQtModel replyModel(CConfigManager::clientId());
        cs2clSender::send(cs2cl::CLHandshakeAckCommandC(replyModel));

        if (SessionManager::GetInstance()->hasLiveSession())
        {
            SessionBeginQtModel ssbQtModel;
            cs2clSender::send(cs2cl::SessionBeginCommandC(ssbQtModel));
        }
    }
    // handshake failed, close the socket
    else
    {
        LOG_ERROR << "rej cl skt. ts is " << timestamp 
            << " cuts is " << currentTS;
        cs2clSocketManager::GetInstance()->rejectPendingSocket(socket);
        return;
    }
}


