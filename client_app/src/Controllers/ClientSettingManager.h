#ifndef CLIENTSETTINGMANAGER_H
#define CLIENTSETTINGMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QTime>

class QTimer;
class SystemSettingReplyQtModel;
class ClientSettingManager : public QObject, public Singleton_T<ClientSettingManager>
{
    Q_OBJECT

public:
    bool init();
    bool connectSigSlot();

public: // accessors
    QTime autoShutdownTime() const { return m_autoShutdownTime; }
    void setAutoShutdownTime(const QTime &val) { m_autoShutdownTime = val; }

    QString autoShutdownMsg() const { return m_autoShutdownMsg; }
    void setAutoShutdownMsg(const QString &val) { m_autoShutdownMsg = val; }

signals:
    void aboutToShutdownComputer(const QTime &shutdownTime);

public slots:
    void serverConnectedSlot();

    void procSystemSettingReply(const SystemSettingReplyQtModel &qtModel);
    void scheduledShutdownHintTimeoutSLot();

private:
    void _handleAutoShutdownCmdReply(const QTime& time, const QString& msg);
    
private:
    QTimer* m_scheduledShutdownHintTimer;

    QTime m_autoShutdownTime;
    QString m_autoShutdownMsg;
};

#endif // CLIENTSETTINGMANAGER_H
