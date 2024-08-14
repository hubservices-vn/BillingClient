#ifndef CS2CL_NETWORKMANAGER_H
#define CS2CL_NETWORKMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

enum SocketStatus;
enum NetworkResult;
class CLHandshakeQtModel;
class EndConnectionRequestQtModel;
class BeginConnectionRequestQtModel;
class cs2clNetworkManager : public QObject, public Singleton_T<cs2clNetworkManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    bool isLockerConnected() const;

    NetworkResult lockerNetworkResult() const { return m_lockerNetworkResult; }

public: // client cmd handler
    void procCLHandshake( QTcpSocket* socket, const CLHandshakeQtModel& qtModel );

    void procNetworkStateRequest();

signals:
    void lockerNetworkStateChanged(/*NetworkResult*/int resultVal);
    void lockerConnected();
    void lockerDisconnected();

public slots:
    void socketStatusChangedSlot(/*SocketStatus*/ int statusVal);

private:
    void _updateNetworkResult(NetworkResult result);
    
private:
    NetworkResult m_lockerNetworkResult;
};

#endif // CS2CL_NETWORKMANAGER_H
