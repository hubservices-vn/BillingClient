#include "cl2csNetworkManager.h"
#include "NetworkStatus.h"
#include "cl2csSocketManager.h"
#include "log.h"
#include "ConfigManager.h"
#include "DateTimeUtils.h"
#include "cl2csSender.h"
#include "cl2csCmds.h"
#include "CLHandshakeQtModel.h"

// at the very beginning, once socket connected, do handshake once;
// each time the handshake timeout/failed, tries to re-do handshake 
// starting from Handshake_Interval_Base ms, with the increment by Handshake_Interval_Step ms
// the maximum interval wouldn't be longer than Handshake_Interval_Max ms
const int Handshake_Interval_Base = 100;
const int Handshake_Interval_Step = 50;
const int Handshake_Interval_Max = 1000;

bool cl2csNetworkManager::init()
{
    m_serviceNetworkResult = NetworkResult_Stopped;

    m_handShakeTimer.reset(new QTimer());
    m_handShakeTimer->setSingleShot(true);
    connect(m_handShakeTimer.data(), SIGNAL(timeout()), this, SLOT(doHandShake()));

    return true;
}

bool cl2csNetworkManager::connectSigSlot()
{
    {
        cl2csSocketManager* socketMgr = cl2csSocketManager::GetInstance();
        connect(socketMgr, SIGNAL(socketStatusChanged(int)), 
            this, SLOT(csSocketStatusChangedSlot(int)));
    }

    return true;
}

bool cl2csNetworkManager::isServiceConnected() const
{
    return (m_serviceNetworkResult == NetworkResult_Connected);
}

void cl2csNetworkManager::csSocketStatusChangedSlot( int socketStatus )
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
}

static int s_interval = -1;
int cl2csNetworkManager::_getProperHandshakeInterval()
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

void cl2csNetworkManager::_resetHandshakeInterval()
{
    s_interval = -1;
}

void cl2csNetworkManager::restartHandshakeTimer()
{
    stopHandshakeTimer();

    LOG_DEBUG << "begin handshake timer";
    m_handShakeTimer->start(Handshake_Interval_Base);
}

void cl2csNetworkManager::stopHandshakeTimer()
{
    if (m_handShakeTimer->isActive())
    {
        LOG_DEBUG << "stop handshake timer";
        m_handShakeTimer->stop();
    }
}

void cl2csNetworkManager::doHandShake()
{
    CLHandshakeQtModel qtModel;
    qtModel.Version_ = QCoreApplication::applicationVersion();
    qtModel.Timestamp_ = UtcDTUtils::dtToUtcTimestamp(QDateTime::currentDateTime());

    cl2csSender::send(cl2cs::CLHandshakeCommandC(qtModel));
}

void cl2csNetworkManager::procCLHandshakeAck( const CLHandshakeAckQtModel& qtModel )
{
    stopHandshakeTimer();

    m_serviceNetworkResult = NetworkResult_Connected;

    emit handshakeSucceeded();
}

