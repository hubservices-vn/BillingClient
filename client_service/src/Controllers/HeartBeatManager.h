#ifndef HEARTBEATMANAGER_H
#define HEARTBEATMANAGER_H

#include <QObject>
#include <QScopedPointer>
#include <QDateTime>
#include <QTimer>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;


class HeartBeatQtModel;
class SvrHeartBeatSender : 
    public QObject, 
    public Singleton_T<SvrHeartBeatSender>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public:
    void procHeartBeatReply(const HeartBeatQtModel &qtModel);

public slots:
    void sendHeartbeatSlot();

private:
    QDateTime m_lastHeartBeatRecvDT;
    QScopedPointer<QTimer> m_heartBeatTimer;
};



class ClientHeartBeatChecker : 
    public QObject, 
    public Singleton_T<ClientHeartBeatChecker>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public:
    void procCCHeartBeat(const HeartBeatQtModel &qtModel);

    void procCLHeartBeat(const HeartBeatQtModel &qtModel);

private:
    QDateTime m_appLastHeartBeatRecvDT;
    QDateTime m_lockerLastHeartBeatRecvDT;
};




#endif // HEARTBEATMANAGER_H
