#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;


#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QSet>

class ScreenUnlockReplyQtModel;
class ScreenLockReplyQtModel;
class ScreencaptureRequestQtModel;
class EndConnectionReplyQtModel;
class UpdateClientRequestQtModel;
class QueryAppRequestQtModel;
class TerminateAppRequestQtModel;
class PcActionRequestQtModel;
class SystemMessageQtModel;
enum PCActionType;
class SystemManager : public QObject, public Singleton_T<SystemManager>
{
    Q_OBJECT
public:
    ~SystemManager();

    bool init();
    bool connectSigSlot();
    bool startup();

    bool runVncServer();

public: // cmd handler
    // find all the running processes and report back to server
    void procQueryAppRequest( const QueryAppRequestQtModel &qtModel );

    void procTermAppRequest( const TerminateAppRequestQtModel &qtModel );

    void procPcAction( const PcActionRequestQtModel& qtModel );

    // lock screen
    void procScreenLockReply(const ScreenLockReplyQtModel &qtModel);
    // unlock screen
    void procScreenUnlockReply( const ScreenUnlockReplyQtModel& qtModel );

    // capture image
    void procScreenCaptureRequest( const ScreencaptureRequestQtModel &qtModel );

    void procUpdateClientRequest( const UpdateClientRequestQtModel &qtModel );

    void procSystemMessage( const SystemMessageQtModel& qtModel );

    void addExcludeApp( const QString& appName);

    void addExcludeApps( const QList<QString>& appNames); 

    void removeExcludeApp( const QString& appName);
signals:
    // modeVal: value of enum "ScreenLockMode", defined in ScreenLockQtModel.h
    void screenLockReplyRecved(int modeVal, bool result);
    void screenUnlockReplyRecved(int modeVal, bool result);

public slots:

private:
    void _pcActionImp(PCActionType actionType);

    void _runTightVNC();
    void _runUltraVNC();

private:
    QStringList m_excludeAppKillList;
    QSet<QString> m_excludeAppKillSet;
};

#endif // SYSTEMMANAGER_H
