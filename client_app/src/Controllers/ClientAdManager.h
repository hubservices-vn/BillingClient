#ifndef CLIENTADMANAGER_H
#define CLIENTADMANAGER_H

#include <QObject>
#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QFileSystemWatcher>
#include <QTimer>

enum ClientAdType
{
    ClientAdType_NA = -1,
    ClientAdType_Local = 1,
    ClientAdType_Embed = 2,
    //ClientAdType_Server = 3,
};
class ClientAdTransferReplyQtModel;
class ClientAdManager : public QObject, public Singleton_T<ClientAdManager>
{
    Q_OBJECT
public:
    ~ClientAdManager();

    bool init();
    bool connectSigSlot();

public:
    QString adUrl() const;
    QString localClientAdFilePath() const;
    QString localBakFilePath() const;

    void restoreDefaultClientAdFile();

signals:
    // tell UI to refresh self
    void clientAdFileChanged();

public slots:
    void requestClientAdSlot();

    void verifyClientAdSlot();

    void procClientAdReplyC(const ClientAdTransferReplyQtModel& qtModel);

private: // helper
    QString _getClientAdDirPath() const;

private:
    // verify the local clientAd file
    void _verifyLocalClientAdImp();

    // calc the embed clientAd file hash
    void _getClientAdHashAsync(/*val of ClientAdType*/int typeVal);
    void _getClientAdHashImp(/*val of ClientAdType*/int typeVal);

    void _watchClientAdFile();

    void _saveNewClientAdFile(const QByteArray& clientAdContent);

private:
    QString m_adUrl;
    QString m_localClientAdFilePath;
    QString m_localClientAdHash;

    QString m_embedClientAdHash;

    QTimer* m_updateClientAdTimer;
    QTimer* m_verifyClientAdTimer;

    // if this is true, then m_verifyClientAdTimer won't 
    // do anything
    bool m_isSelfChange; 
};

#endif // CLIENTADMANAGER_H
