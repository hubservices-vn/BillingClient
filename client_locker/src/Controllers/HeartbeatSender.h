#ifndef HEARTBEAT_SENDER_H
#define HEARTBEAT_SENDER_H


#include "CcCore/CcSingleton.h"

#include <QTimer>
#include <QObject>
#include <QScopedPointer>
#include <QDateTime>


class HeartBeatQtModel;
class HeartBeatSender : public QObject, public Singleton_T<HeartBeatSender>
{
    Q_OBJECT

public:
    bool init();
    bool connectSigSlot();

public:
    void startHeartbeat();
    void stopHeartbeat();

public:
    void procHeartBeatReply(const HeartBeatQtModel &qtModel);

public slots:
    void sendHeartbeatSlot();

private:
    void _checkServerHeartbeat();
    void _sendHeartbeatToServer();

    bool _isPeerLostReponse() const;

private:
    QDateTime m_lastHeartBeatRecvDT;

    QScopedPointer<QTimer> m_heartBeatTimer;
};

#endif // HEARTBEAT_SENDER_H
