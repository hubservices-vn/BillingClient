#ifndef CLIENTSETTINGMANAGER_H
#define CLIENTSETTINGMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

class SystemSettingReplyQtModel;
class AdminCodeReplyQtModel; // server changed admin code
class AdminOptionRequestQtModelC; // client UI requests some "server setting" change
class SystemSettingRequestQtModel;
class ClientSettingManager : public QObject, public Singleton_T<ClientSettingManager>
{
    Q_OBJECT

public:
    bool init();
    bool connectSigSlot();

    // getting different url from server setting
    QString getClientCodeDllUrlPath();
    void checkLocalAdminUserNamePassword();
signals:
    void shutdownAvailableClientMinChanged();
    void clearAppAvailableClientMinChanged();
    void autoScheduledShutdownTimeChanged();
    void adminCodeChangePasswordIncorrectly();
public slots:
    // when the client is connected, ask the server to reply the client settings
    void serverConnectedSlot();

    void procSystemSettingReply(const SystemSettingReplyQtModel &qtModel);
    void procAdminCodeReply(const AdminCodeReplyQtModel &qtModel);
    void procAdminOptionChangeRequest(const AdminOptionRequestQtModelC &qtModel);
    void procSystemSettingRequestC( SystemSettingRequestQtModel& qtModel );

private:
    QString _getServerUrl(const QString& key);

private:
    QMap<QString, QString> m_mapServerUrl;
};

#endif // CLIENTSETTINGMANAGER_H
