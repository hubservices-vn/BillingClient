#ifndef CL2CSNETWORKMANAGER_H
#define CL2CSNETWORKMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include "QtTypePtrs.h"

#include "CcCore/CcSingleton.h"

class CLHandshakeAckQtModel;
class NetworkStateReplyQtModel;
enum NetworkResult;
class cl2csNetworkManager : public QObject, public Singleton_T<cl2csNetworkManager>
{
    Q_OBJECT
public:


public: // accessors
    bool isServiceConnected() const;

public: // cmd handlers
    void procCLHandshakeAck( const CLHandshakeAckQtModel& qtModel );

signals:
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

private:
    // this timer keeps running until the handshake successfully finishes
    QTimerPtr_c m_handShakeTimer;

    // this actually stores the socket status of client service
    // (socket status are converted to network status)
    NetworkResult m_serviceNetworkResult;
};

#endif // CC2CSNETWORKMANAGER_H
