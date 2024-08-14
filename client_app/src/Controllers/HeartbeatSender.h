#ifndef HEARTBEAT_SENDER_H
#define HEARTBEAT_SENDER_H


#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

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
    QDateTime m_lastHeartBeatRecvDT;

    QScopedPointer<QTimer> m_heartBeatTimer;
};

#endif // HEARTBEAT_SENDER_H
