#ifndef CS2SSNETWORKMANAGER_H
#define CS2SSNETWORKMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include "QtTypePtrs.h"

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

enum NetworkResult;
enum SocketStatus;
class QTimer;
class HandshakeAckQtModel;
class cs2ssNetworkManager : public QObject, public Singleton_T<cs2ssNetworkManager>
{
    Q_OBJECT
public:
    ~cs2ssNetworkManager();

public:
    bool init();
    bool connectSigSlot();

    bool isServerConnected() const;

    NetworkResult serverNetworkResult() const { return m_serverNetworkResult; }

    // received the handshake ACK msg from server
    void procHandshakeAck(const HandshakeAckQtModel &ackModel);

signals:
    void serverConnected();
    void serverDisconnected();
    void serverNetworkStateChanged(/*NetworkResult*/int result);
    void handShakeSucceeded();

public slots:
    // if status is Connected, send the "init connection" command to server and wait for its responds
    // if status is disconnected and heartbeat is running, stop it
    void serverSocketStatusChangedSlot(/*SocketStatus*/int status);

    void clientAppNetworkStateChangedSlot(int state);
    //void beginConnectionReplySlot(bool result);

    // check whether client received the handshake ack
    void checkHandShakeResult();

private:
    void startHandshakeCheckTimer();
    void stopHandshakeCheckTimer();

    void doHandShake();

private:
    void _updateNetworkResult(NetworkResult result);

    // tell client App to update the network status to UI
    bool _sendNetworkStateReply();

private:
    // after handshake sent out, this timer runs single-shot and check whether it receives ack after specific interval
    QTimerPtr_c m_handShakeCheckTimer;
    NetworkResult m_serverNetworkResult;

    //// after client sent out the handshake command, this value will be set to true;
    //// after either received handshakeAck or after the Handshake_Check_Interval passed (still didn't get ack),
    //// this will be set as false
    ////bool m_hasActiveHandshakeRequest;
    //bool m_hasRecvHandshakeAck;
};
#endif // CS2SSNETWORKMANAGER_H
