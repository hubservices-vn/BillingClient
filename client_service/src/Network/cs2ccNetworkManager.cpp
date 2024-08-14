#include "cs2ccNetworkManager.h"
#include "cs2ccSocketManager.h"
#include "cs2ccSender.h"
#include "NetworkStatus.h"
#include "cs2ssSocketManager.h"
#include "cs2ssNetworkManager.h"
#include "NetworkStateQtModel.h"
#include "ConfigManager.h"
#include "cs2ccCmds.h"
#include "ClientConnectionQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "UserAccountManager.h"
#include "UIAppManager.h"
#include "CCHandshakeQtModel.h"
#include "DateTimeUtils.h"
#include "cs2clSender.h"
#include "cs2clCmds.h"
#include "ConfigManager.h"
#include "CommonSettings.h"
#include "GbProtectManager.h"

#ifdef NDEBUG
// release
const qint64 HANDSHAKE_TIME_GAP = 30; // 30s
#else
// debug
const qint64 HANDSHAKE_TIME_GAP = 300; // 300s
#endif

bool cs2ccNetworkManager::init()
{
    m_clientAppNetworkResult = NetworkResult_Stopped;
    return true;
}

bool cs2ccNetworkManager::connectSigSlot()
{
    {
        cs2ccSocketManager* ccSktMgr = cs2ccSocketManager::GetInstance();
        connect(ccSktMgr, SIGNAL(socketStatusChanged(int)),
            this, SLOT(socketStatusChangedSlot(int)));
    }

    return true;
}

void cs2ccNetworkManager::procNetworkStateRequest()
{
    NetworkStateReplyQtModel replyModel (
        CConfigManager::clientId(), 
        cs2ssNetworkManager::GetInstance()->serverNetworkResult());

    cs2ccSender::send(cs2cc::NetworkStateReplyCommand(replyModel));
}

void cs2ccNetworkManager::socketStatusChangedSlot( /*SocketStatus*/ int socketStatus )
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

void cs2ccNetworkManager::_updateNetworkResult( NetworkResult networkResult )
{
    if (m_clientAppNetworkResult == networkResult)
    {
        return;
    }

    LOG_DEBUG4 << "client app network state changed from: "
        << qPrintable(NetworkResulttoQString(m_clientAppNetworkResult))
        << " to: " << qPrintable(NetworkResulttoQString(networkResult));

    m_clientAppNetworkResult = networkResult;
    emit clientAppNetworkStateChanged(m_clientAppNetworkResult);

    if (m_clientAppNetworkResult == NetworkResult_Connected)
    {
        emit clientAppConnected();
    }
    else
    {
        emit clientAppDisconnected();
    }

}

bool cs2ccNetworkManager::isClientAppConnected() const
{
    return m_clientAppNetworkResult == NetworkResult_Connected;
}

// everytime client APP recved "client service connected" event, it will send out 
// the "BeginConnection" command to client service (handld here), and then fwd to server
// tell server that client UI App has connected to service
void cs2ccNetworkManager::procBeginConnectionRequest( BeginConnectionRequestQtModel &qtModel )
{
    // suppose client service crashed, while client UI is running and user is logged in,
    // after service restarts, it doesn't know the current user
    UserAccountManager::GetInstance()->tryResumeAccount(qtModel.AccountId_, qtModel.AccType_, qtModel.AccName_);

    QString accCodeHash = "";
    if (UserAccountManager::GetInstance()->currentUserType() == UserGroup_LocalSuperAdmin)
    {
        accCodeHash = CConfigManager::adminCodeHash();
    }
    qtModel = BeginConnectionRequestQtModel(
        CConfigManager::clientId(),
        UserAccountManager::currentUserId(), 
        UserAccountManager::currentUserType(),
        UserAccountManager::currentUserName(),
        accCodeHash,
        qtModel.client_app_pid_,
        qtModel.client_app_path_);
    cs2ssSender::send(cs2ss::BeginConnectionRequestCommand(qtModel));

    UIAppManager::GetInstance()->onAppBeginConn(qtModel.client_app_pid_);
}
void cs2ccNetworkManager::procEndConnectionRequestC( EndConnectionRequestQtModel &qtModel )
{
    // tell server that client App disconnected
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::EndConnectionRequestCommand(qtModel));

    // tell client locker to close itself
    EndConnectionReplyQtModel replyModel;
    cs2clSender::send(cs2cl::EndConnectionReplyCommandC(replyModel));

    // tell service to stop service-app-locker process inter checking
    UIAppManager::GetInstance()->onAppEndConn();

    // tell sys driver to clear its PID list
    // reason: If cafe owner closes the app & locker, he may kill the service as well
    GbProtectManager::GetInstance()->onAppEndConnection();

    // tell system manager to stop "shutdown available pc" or "clear available pc apps" timer
    // re-enable the security features as when admin clicks "exit", it sends out "logout" before "end connection"
    emit clientEndedConnection();
}

void cs2ccNetworkManager::procCCHandshake( QTcpSocket* socket, const CCHandshakeQtModel& qtModel )
{
    QString ver = qtModel.Version_;
    qint64 timestamp = qtModel.Timestamp_;

    qint64 currentTS = UtcDTUtils::dtToUtcTimestamp(QDateTime::currentDateTime());
    if (qAbs(currentTS - timestamp) < HANDSHAKE_TIME_GAP)
    {
        bool alreadyConnected = cs2ccSocketManager::GetInstance()->isSocketConnected();
        if (alreadyConnected)
        {
            QTcpSocket* existingSocket = cs2ccSocketManager::GetInstance()->socket();
            if (existingSocket != NULL &&
                existingSocket != socket)
            {
                LOG_DEBUG4 << "the locker conenction is dead . close the dead one";
                // the client with same clientId now connects with a different socket
                // that means the "old" socket stored on server is dead now, go to socketManager to close the "dead" socket
                cs2ccSocketManager::GetInstance()->closeSocket();
            }
        }

        // the socket won't be accepted as a valid one until the workstation manager finishes handling it (as the clientId may be wrong from the client side)
        cs2ccSocketManager::GetInstance()->acceptSocket(socket);

        CCHandshakeAckQtModel replyModel(CConfigManager::clientId());
        cs2ccSender::send(cs2cc::CCHandshakeAckCommandC(replyModel));
    }
    // handshake failed, close the socket
    else
    {
        LOG_ERROR << "rej cc skt. ts is " << timestamp 
            << " cuts is " << currentTS;
        cs2ccSocketManager::GetInstance()->rejectPendingSocket(socket);
        return;
    }
}


