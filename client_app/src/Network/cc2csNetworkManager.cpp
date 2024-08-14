#include "cc2csNetworkManager.h"
#include "NetworkStateQtModel.h"
#include "NetworkStatus.h"
#include "cc2csSocketManager.h"
#include "log.h"
#include "NetworkStateQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "ClientConnectionQtModel.h"
#include "ConfigManager.h"
#include "UserAccountManager.h"
#include "SystemSettingQtModel.h"
#include "CCHandshakeQtModel.h"
#include "DateTimeUtils.h"

// at the very beginning, once socket connected, do handshake once;
// each time the handshake timeout/failed, tries to re-do handshake 
// starting from Handshake_Interval_Base ms, with the increment by Handshake_Interval_Step ms
// the maximum interval wouldn't be longer than Handshake_Interval_Max ms
const int Handshake_Interval_Base = 100;
const int Handshake_Interval_Step = 50;
const int Handshake_Interval_Max = 1000;
bool cc2csNetworkManager::init()
{
    m_serverNetworkResult = NetworkResult_Stopped;
    m_serviceNetworkResult = NetworkResult_Stopped;
    m_handShakeTimer.reset(new QTimer());
    m_handShakeTimer->setSingleShot(true);
    connect(m_handShakeTimer.data(), SIGNAL(timeout()), this, SLOT(doHandShake()));
    return true;
}

bool cc2csNetworkManager::connectSigSlot()
{
    {
        cc2csSocketManager* socketMgr = cc2csSocketManager::GetInstance();
        connect(socketMgr, SIGNAL(socketStatusChanged(int)), 
            this, SLOT(csSocketStatusChangedSlot(int)));
    }

    return true;
}

bool cc2csNetworkManager::isServerConnected() const
{
    return (
        m_serviceNetworkResult == NetworkResult_Connected &&
        m_serverNetworkResult == NetworkResult_Connected);
}

bool cc2csNetworkManager::isServiceConnected() const
{
    return (m_serviceNetworkResult == NetworkResult_Connected);
}

void cc2csNetworkManager::procNetworkStateReply( const NetworkStateReplyQtModel &qtModel )
{
    LOG_DEBUG << "Network result reply from client service: " << qtModel.toStdString();
    if (m_serverNetworkResult == qtModel.State)
    {
        return;
    }
    m_serverNetworkResult = qtModel.State;

    _notifyNetworkResultChange();


}

void cc2csNetworkManager::csSocketStatusChangedSlot( int socketStatus )
{
    if (socketStatus == SocketStatus_Connected)
    {
        doHandShake();
        restartHandshakeTimer();
    }
    else
    {
        stopHandshakeTimer();
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

    if (m_serviceNetworkResult == networkResult)
    {
        return;
    }
    m_serviceNetworkResult = networkResult;
    _notifyNetworkResultChange();   
}

void cc2csNetworkManager::_notifyNetworkResultChange()
{
    emit networkStateChanged(finalNetworkState());

    if (m_serviceNetworkResult == NetworkResult_Connected)
    {
        LOG_DEBUG4 << "connection to client service done. query latest Network state";
        // get currrent server network state
        {
            NetworkStateQtModel qtModel;
            cc2csSender::send(cc2cs::NetworkStateQueryCommand(qtModel));
        }
    }

    if (m_serverNetworkResult == NetworkResult_Connected)
    {
        // send begin connection command
        {
            TCHAR szApp[MAX_PATH+1]={0};
            GetModuleFileName(NULL,szApp,MAX_PATH);

            BeginConnectionRequestQtModel qtModel
                (CConfigManager::clientId(), 
                UserAccountManager::currentUserId(),
                UserAccountManager::currentUserType(),
                UserAccountManager::currentUserName(),
                "",
                GetCurrentProcessId(),
                QString::fromStdWString(szApp));
            cc2csSender::send(cc2cs::BeginConnectionRequestCommandC(qtModel));
        }

        // request client setting (suppose client service has already got system settings while
        // client app is not connected, then the service won't push to app, app needs to request for it)
        {
            SystemSettingRequestQtModel qtModel(CConfigManager::clientId());
            cc2csSender::send(cc2cs::SystemSettingRequestCommandC(qtModel));
        }

        emit serverConnected();
    }
    else
    {
        emit serverDisconnected();
    }
}

static int s_interval = -1;
int cc2csNetworkManager::_getProperHandshakeInterval()
{
    if (s_interval < 0)
    {
        s_interval = Handshake_Interval_Base;
        return s_interval;
    }

    if (s_interval < Handshake_Interval_Max)
    {
        s_interval += Handshake_Interval_Step;
    }

    return s_interval;
}

void cc2csNetworkManager::_resetHandshakeInterval()
{
    s_interval = -1;
}

void cc2csNetworkManager::restartHandshakeTimer()
{
    stopHandshakeTimer();

    LOG_DEBUG << "begin handshake timer";
    m_handShakeTimer->start(Handshake_Interval_Base);
}

void cc2csNetworkManager::stopHandshakeTimer()
{
    if (m_handShakeTimer->isActive())
    {
        LOG_DEBUG << "stop handshake timer";
        m_handShakeTimer->stop();
    }
}

NetworkResult cc2csNetworkManager::finalNetworkState() const
{
    if (m_serviceNetworkResult != NetworkResult_Connected)
    {
        return m_serviceNetworkResult;
    }
    else
    {
        return m_serverNetworkResult;
    }
}

void cc2csNetworkManager::doHandShake()
{
    CCHandshakeQtModel qtModel;
    qtModel.Version_ = QCoreApplication::applicationVersion();
    qtModel.Timestamp_ = UtcDTUtils::dtToUtcTimestamp(QDateTime::currentDateTime());

    cc2csSender::send(cc2cs::CCHandshakeCommandC(qtModel));
}

void cc2csNetworkManager::procCCHandshakeAck( const CCHandshakeAckQtModel& qtModel )
{
    stopHandshakeTimer();

    m_serviceNetworkResult = NetworkResult_Connected;

    emit handshakeSucceeded();
}
