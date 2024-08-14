#ifndef CC2CSNETWORKMANAGER_H
#define CC2CSNETWORKMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include "QtTypePtrs.h"

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class CCHandshakeAckQtModel;
class NetworkStateReplyQtModel;
enum NetworkResult;
class cc2csNetworkManager : public QObject, public Singleton_T<cc2csNetworkManager>
{
    Q_OBJECT
public:


public: // accessors
    bool isServerConnected() const;

    bool isServiceConnected() const;

    // the state of service & server network connection
    NetworkResult finalNetworkState() const;

public: // methods
    void procNetworkStateReply(const NetworkStateReplyQtModel &qtModel);
    void procCCHandshakeAck( const CCHandshakeAckQtModel& qtModel );

signals:
    void serverConnected();
    void serverDisconnected();
    void networkStateChanged(int result);
    void handshakeSucceeded();

public slots:
    bool init();
    bool connectSigSlot();
    void csSocketStatusChangedSlot( int socketStatus );

    void doHandShake();
private:
    void restartHandshakeTimer();
    void stopHandshakeTimer();
    int _getProperHandshakeInterval();
    // after connected, should reset the connection interval back to default value
    // otherwise the connection interval will not decrease
    void _resetHandshakeInterval();
	
    void _notifyNetworkResultChange();
    
private:
    // this timer keeps running until the handshake successfully finishes
    QTimerPtr_c m_handShakeTimer;
	
    // this only stores the server "network state" passed from client service side
    NetworkResult m_serverNetworkResult;

    // this actually stores the socket status of client service
    // (socket status are converted to network status)
    NetworkResult m_serviceNetworkResult;
};

#endif // CC2CSNETWORKMANAGER_H
