#ifndef CS2CCNETWORKMANAGER_H
#define CS2CCNETWORKMANAGER_H

#include <QObject>
#include <QScopedPointer>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

enum SocketStatus;
enum NetworkResult;
class CCHandshakeQtModel;
class EndConnectionRequestQtModel;
class BeginConnectionRequestQtModel;
class cs2ccNetworkManager : public QObject, public Singleton_T<cs2ccNetworkManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    bool isClientAppConnected() const;

    NetworkResult clientAppNetworkResult() const { return m_clientAppNetworkResult; }

public: // client cmd handler
    void procNetworkStateRequest();

    void procCCHandshake( QTcpSocket* socket, const CCHandshakeQtModel& qtModel );

    void procBeginConnectionRequest( BeginConnectionRequestQtModel &qtModel );

    void procEndConnectionRequestC( EndConnectionRequestQtModel &qtModel );

signals:
    void clientAppNetworkStateChanged(/*NetworkResult*/int result);
    void clientAppConnected();
    void clientAppDisconnected();

    // "begin connection" recved & handled
    void clientBegunConnection(); 
    // "end connection" recved & handled
    void clientEndedConnection();

public slots:
    void socketStatusChangedSlot(/*SocketStatus*/ int status);

private:
    void _updateNetworkResult(NetworkResult result);
    


private:
    NetworkResult m_clientAppNetworkResult;
};

#endif // CS2CCNETWORKMANAGER_H
