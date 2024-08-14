#include "cs2ssNetworkManager.h"
#include "NetworkEntry.h"
#include "HandshakeQtModel.h"
#include "HttpSecurityHelper.h"
#include "NetworkStatus.h"
#include "ConfigManager.h"
#include "CommonSettings.h"
#include "OSAgentWinQt.h"
#include "cs2ssSocketManager.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "cs2ccNetworkManager.h"
#include "cs2ccSocketManager.h"
#include "NetworkStateQtModel.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "ClientConnectionQtModel.h"
#include "UserAccountManager.h"
#include "OfflineSessionManager.h"
#include "OfflineSessionQtModel.h"
#include "OfflineSession.h"
#include "revert_api.h"


#ifdef NDEBUG //release
const int Handshake_CHECK_Interval = 10000; // 10,000ms = 10s
#else
const int Handshake_CHECK_Interval = 10000; // 60,000ms
#endif


cs2ssNetworkManager::~cs2ssNetworkManager()
{
}

bool cs2ssNetworkManager::init()
{
    m_serverNetworkResult = NetworkResult_NA;

    m_handShakeCheckTimer.reset(new QTimer());
    m_handShakeCheckTimer->setSingleShot(true);
    connect(m_handShakeCheckTimer.data(), SIGNAL(timeout()), this, SLOT(checkHandShakeResult()));

    return true;
}

bool cs2ssNetworkManager::connectSigSlot()
{
    {
        // listen to the socket connected/disconnected event
        // and update server's NetworkResult
        cs2ssSocketManager* socketMgr = cs2ssSocketManager::GetInstance();
        connect(socketMgr, SIGNAL(serverSocketStatusChanged(int)), 
            this, SLOT(serverSocketStatusChangedSlot(int)));
    }

    {
        // listen to the socket connected/disconnected event
        // and update server's NetworkResult
        cs2ccNetworkManager* ccNetMgr = cs2ccNetworkManager::GetInstance();
        connect(ccNetMgr, SIGNAL(clientAppNetworkStateChanged(int)), 
            this, SLOT(clientAppNetworkStateChangedSlot(int)));
    }
    return true;
}

void cs2ssNetworkManager::serverSocketStatusChangedSlot( /*SocketStatus*/int socketStatus )
{
    if (socketStatus == SocketStatus_Connected)
    {
        doHandShake();
    }

    NetworkResult networkResult = NetworkResult_NA;
    switch (socketStatus)
    {
    case SocketStatus_NA:
        networkResult = NetworkResult_NA;
        break;
    case SocketStatus_Stopped:
        networkResult = NetworkResult_Stopped;
        break;
        // "socket connected" doesn't mean really connected
        // only "handshake ack" finished means connected
    case SocketStatus_Connected:
        networkResult = NetworkResult_Connecting;
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

void cs2ssNetworkManager::stopHandshakeCheckTimer()
{
    if (m_handShakeCheckTimer->isActive())
    {
        m_handShakeCheckTimer->stop();
    }
}

void cs2ssNetworkManager::startHandshakeCheckTimer()
{
    if (m_handShakeCheckTimer->isActive())
    {
        m_handShakeCheckTimer->stop();
    }

    m_handShakeCheckTimer->start(Handshake_CHECK_Interval);
}

void cs2ssNetworkManager::checkHandShakeResult()
{
    LOG_DEBUG << "failed to recv handshakeack";
    // if the server is connected, but can not receive handshake
    // then this TCP socket might be in broken state (found in 1 VN cafe),
    // abandon this tocket and remake a new one
    if (cs2ssSocketManager::GetInstance()->isSocketConnected())
    {
        LOG_DEBUG << "abandon socket";
        cs2ssSocketManager::GetInstance()->_destroySocket();
        cs2ssSocketManager::GetInstance()->_initSocket();
    }

    LOG_DEBUG << "reconnect to server";
    cs2ssSocketManager::GetInstance()->reconnectSocket();
}

void cs2ssNetworkManager::doHandShake()
{
    NetworkAddress networkAddr;
    QTcpSocket* socket = cs2ssSocketManager::GetInstance()->socket();
    if (socket == NULL)
    {
        LOG_ERROR << "do handshake, but socket is null";
        return;
    }

    if (socket->localAddress().protocol() == QAbstractSocket::IPv4Protocol)
    {
        //LOG_DEBUG << "IP v4";
        quint32 ipv4Addr = socket->localAddress().toIPv4Address();
        //LOG_DEBUG << "ipv4Addr is " << ipv4Addr;
        networkAddr = NetworkAddressManager::getNetworkAddress(ipv4Addr);
    }
    else if (socket->localAddress().protocol() == QAbstractSocket::IPv6Protocol)
    {
        LOG_DEBUG << "IP v6";
        Q_IPV6ADDR ipv6Addr = socket->localAddress().toIPv6Address();
        networkAddr = NetworkAddressManager::getNetworkAddress(ipv6Addr);
    }
    else
    {
        LOG_ERROR << "do handshake, can't get socket's protocal";
        // unknown network protocol, report error
        _updateNetworkResult(NetworkResult_UnknownNetworkProtocol);
        return;
    }

    // in a cafe we find the network address is always invalid, no idea why
    if (false == networkAddr.isValid())
    {
        LOG_ERROR << "do handshake, network address is invalid";
        networkAddr.setMac(NetworkAddressManager::getMacAddress());
        //_updateNetworkResult(NetworkResult_UnknownNetworkAddress);
        //return;
    }

    // check whether we can auto-update the cafe
    bool canAutoUpdate = false;
    {
        RevertAPI api;
        canAutoUpdate = api.canAutoUpdate();
    }

    HandshakeQtModel qtModel;
    qtModel.HostName = networkAddr.LocalHostName();
    qtModel.Ipv4 = networkAddr.ipv4();
    qtModel.Ipv6 = networkAddr.ipv6();
    qtModel.MacAddress = networkAddr.mac();
    qtModel.CVolSN = HttpSecurityHelper::GetSysVolSn();
    qtModel.AppVersion = QCoreApplication::applicationVersion();
    qtModel.CanAutoUpdate_ = canAutoUpdate;

    OfflineSession* userOfflineSession = OfflineSessionManager::GetInstance()->userOfflineSession();
    OfflineSession* adminOfflineSession = OfflineSessionManager::GetInstance()->adminOfflineSession();
    if (userOfflineSession != NULL)
    {
        qtModel.OfflineUserSessionList_.append(OfflineSession_ReportItem(
            userOfflineSession->SessionId(),
            userOfflineSession->Id(),
            userOfflineSession->OfflineStartTime(),
            userOfflineSession->OfflineStopTime(),
            userOfflineSession->getRealOfflineMinutes(),
            userOfflineSession->AccountId(),
            userOfflineSession->accountType(),
            "",
            userOfflineSession->status())
            );
    }
    if (adminOfflineSession != NULL)
    {
        QString accCodeHash = CConfigManager::adminCodeHash();
        qtModel.OfflineUserSessionList_.append(OfflineSession_ReportItem(
            adminOfflineSession->SessionId(),
            adminOfflineSession->Id(),
            adminOfflineSession->OfflineStartTime(),
            adminOfflineSession->OfflineStopTime(),
            adminOfflineSession->getRealOfflineMinutes(),
            adminOfflineSession->AccountId(),
            adminOfflineSession->accountType(),
            accCodeHash,
            adminOfflineSession->status())
            );
    }
    qtModel.CurrentAccId_ = UserAccountManager::currentUserId();
    qtModel.CurrentAccType_ = UserAccountManager::currentUserType();

    cs2ssSender::send(cs2ss::HandshakeCommand(qtModel));

    // after we send out the hand shake cmd, we wait for the result
    startHandshakeCheckTimer();
}

void cs2ssNetworkManager::procHandshakeAck(const HandshakeAckQtModel &ackModel)
{
    // recved handshake ack, so no need to do the "check whether we have recved handshake ack"
    stopHandshakeCheckTimer();

    LOG_DEBUG << "recved handshake ack from server. Detail: "
        << ackModel.toStdString();

    if (ackModel.ClientId < 0)
    {
        _updateNetworkResult(NetworkResult_NoAvailableClientId);
        return;
    }

    if (ackModel.Result == HandShakeResult::HandShakeResult_OK ||
        ackModel.Result == HandShakeResult::HandShakeResult_SucceedAssign ||
        ackModel.Result == HandShakeResult::HandShakeResult_SucceedReg)
    {
        // store new clientID to client side
        CConfigManager::setClientId(ackModel.ClientId);

        // 1. let UI displays as "Connected"
        // 2. reprt offline sessions to server;
        // 3. let client service query latest products/security profiles, etc
        _updateNetworkResult(NetworkResult_Connected);
        // can connect to server big file socket now
        cs2ssSocketManager::GetInstance()->connectBigSocket();

        // update client's date and time
        if (CConfigManager::isDeveloperEnv() == false)
        {
            QDateTime dt = QDateTime::fromMSecsSinceEpoch(ackModel.TimestampMSecs);
            OSAgentWinQt::GetInstance()->setLocalTime(
                dt.date(), dt.time(), ackModel.TimezoneBias, ackModel.TimezoneName.toStdString());
        }
    }
    else if(ackModel.Result == HandShakeResult::HandShakeResult_FailReg ||
        ackModel.Result == HandShakeResult::HandShakeResult_FailAssign)
    {
        _updateNetworkResult(NetworkResult_NoAvailableClientId);
    }
    else
    {
        _updateNetworkResult(NetworkResult_FailedUnknownError);
    }
}

bool cs2ssNetworkManager::isServerConnected() const
{
    return m_serverNetworkResult == NetworkResult_Connected;
}

void cs2ssNetworkManager::_updateNetworkResult( NetworkResult networkResult )
{
    if (m_serverNetworkResult == networkResult)
    {
        return;
    }

    LOG_DEBUG << "server network state changed to: " << qPrintable(NetworkResulttoQString(networkResult));

    m_serverNetworkResult = networkResult;

    // tell client UI App
    _sendNetworkStateReply();

    // emit signals
    emit serverNetworkStateChanged(m_serverNetworkResult);

    if (m_serverNetworkResult == NetworkResult_Connected)
    {
        LOG_DEBUG << "emitting serverConnected signal";
        emit serverConnected();
    }
    else
    {
        LOG_DEBUG << "emitting serverDisconnected signal";
        emit serverDisconnected();
    }
}

bool cs2ssNetworkManager::_sendNetworkStateReply()
{
    if (cs2ccSocketManager::GetInstance()->isSocketConnected() == false)
    {
        return false;
    }

    NetworkStateReplyQtModel qtModel (
        CConfigManager::clientId(), m_serverNetworkResult);
    cs2ccSender::send(cs2cc::NetworkStateReplyCommand(qtModel));

    return true;
}

void cs2ssNetworkManager::clientAppNetworkStateChangedSlot( /*NetworkResult*/int result )
{
    if (result == NetworkResult_Connected)
    {
        // tell client UI the latest server status
        _sendNetworkStateReply();
    }
}
