#include "ClientSettingManager.h"
#include "Client.h"
#include "CommonSettings.h"
#include "ConfigManager.h"
#include "cs2ssNetworkManager.h"
#include "SystemSettingQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "AdminCodeQtModel.h"
#include "AdminOptionQtModelC.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"
#include "Utils.h"
#include "cs2ssSender.h"

bool ClientSettingManager::init()
{
    return true;
}

bool ClientSettingManager::connectSigSlot()
{
    return true;
}

void ClientSettingManager::serverConnectedSlot()
{
}

void ClientSettingManager::procSystemSettingReply( const SystemSettingReplyQtModel &qtModel )
{
    int shutdownAvailableMins = qtModel.TurnOffAvailPcMins_;
    int clearAppAvailableMins = qtModel.ClearAppAvailPcMins_;
    QTime autoShutdownTime = qtModel.AutoShutdownTime_;
    QMap<QString, QString> serverUrlMap = qtModel.ServerUrlMap;
    m_mapServerUrl.clear();
    foreach (QString key, serverUrlMap.keys())
    {
        m_mapServerUrl[key] = serverUrlMap[key];
    }

    CConfigManager::setShutdownAvailableClientMinutes(shutdownAvailableMins);
    emit shutdownAvailableClientMinChanged();

    CConfigManager::setClearAppAvailableClientMinutes(clearAppAvailableMins);
    emit clearAppAvailableClientMinChanged();

    CConfigManager::setAutoPcShutdownTime(autoShutdownTime);
    // refresh the QTimer defined in SystemManager
    emit autoScheduledShutdownTimeChanged();


    // forward to client UI (so client UI knows whether to pop up a message reminding
    // user that the computer is about to shutdown)
    {
        cs2ccSender::send(cs2cc::SystemSettingReplyCommandC(qtModel));
    }
}

void ClientSettingManager::procAdminCodeReply( const AdminCodeReplyQtModel &qtModel )
{
    bool match = true;
    QString rawAdminNameEnc = Utils::symDecrypt(qtModel.AdminNameEnc_);
    QString expAdminNameEnc = Utils::symEncrypt(rawAdminNameEnc);
    // if the staff only changes the minutes settings, the "admin name" or "admin code" may be empty
    if (!qtModel.AdminNameEnc_.isEmpty() &&
        qtModel.AdminNameEnc_.compare(CConfigManager::adminNameEnc()))
    {
        match = false;
        CConfigManager::setAdminNameEnc(qtModel.AdminNameEnc_);
    }

    if (!qtModel.AdminCode_.isEmpty() &&
        qtModel.AdminCode_.compare(CConfigManager::adminCodeHash()))
    {
        match = false;
        CConfigManager::setAdminCodeHash(qtModel.AdminCode_);
    }

    // if this happens, mean we need to kick out current user
    if (!match && qtModel.Result_ == ChangeAccPwd_Failed)
    {
        emit adminCodeChangePasswordIncorrectly();
    }
}

void ClientSettingManager::procAdminOptionChangeRequest( const AdminOptionRequestQtModelC &qtModel )
{
    if ((qtModel.OpType_ & AdminOp_ServerAddress) != 0)
    {
        CConfigManager::setServerAddress(qtModel.ServerAddress_);
        cs2ssSocketManager::GetInstance()->reconnectSocket();

        AdminOptionReplyQtModelC replyModel(AdminOp_ServerAddress, AdminOpResult_OK);
        replyModel.ServerAddress_ = qtModel.ServerAddress_;
        cs2ccSender::send(cs2cc::AdminOptionReplyCommandC(replyModel));
    }
    
    if ((qtModel.OpType_ & AdminOp_LocalAdminAcc) != 0)
    {
        QString oldAdminPwdRaw = qtModel.OldAdminPwdRaw_;
        QString oldPwdHashExpected = Utils::hashUserData(
            Utils::symDecrypt(CConfigManager::adminNameEnc()), 
            oldAdminPwdRaw, 
            HashType_GCB_MD5);
        // the user inputs wrong data
        if (QString::compare(oldPwdHashExpected, CConfigManager::adminCodeHash(), Qt::CaseSensitive) != 0)
        {
            AdminOptionReplyQtModelC replyModel(AdminOp_LocalAdminAcc, AdminOpResult_WrongNamePwd);
            cs2ccSender::send(cs2cc::AdminOptionReplyCommandC(replyModel));
        }
        else
        {
            QString oldAdminNameEnc = CConfigManager::adminNameEnc();
            QString newAdminNameEnc = Utils::symEncrypt(qtModel.NewAdminName_);
            QString newPwdHash = Utils::hashUserData(
                qtModel.NewAdminName_, 
                qtModel.NewAdminPwdRaw_, 
                HashType_GCB_MD5);

            // temporary set this, we will try to sync with server.
            CConfigManager::setAdminNameEnc(newAdminNameEnc);
            CConfigManager::setAdminCodeHash(newPwdHash);
            AdminCodeRequestQtModel requestModel(CConfigManager::clientId(), oldAdminNameEnc, newAdminNameEnc, 
                oldPwdHashExpected, newPwdHash);
            cs2ssSender::send(cs2ss::AdminCodeRequestCommand(requestModel));

            AdminOptionReplyQtModelC replyModel(AdminOp_LocalAdminAcc, AdminOpResult_OK);
            replyModel.NewAdminName_ = qtModel.NewAdminName_;
            cs2ccSender::send(cs2cc::AdminOptionReplyCommandC(replyModel));
        }
    }
}

void ClientSettingManager::procSystemSettingRequestC( SystemSettingRequestQtModel&  qtModel )
{
    if (cs2ssNetworkManager::GetInstance()->isServerConnected())
    {
        qtModel.ClientId_ = CConfigManager::clientId();
        cs2ssSender::send(cs2ss::SystemSettingRequestCommand(qtModel));

        checkLocalAdminUserNamePassword();
    }
    else
    {
        SystemSettingReplyQtModel replyModel(
            CConfigManager::shutdownAvailableClientMinutes(),
            CConfigManager::clearAppAvailableClientMinutes(),
            0,/*if server is not connected, no need to report client information*/
            CConfigManager::autoPcShutdownTime(),
            QString(), // in this case, we don't know the msg,
            QMap<QString, QString>()
            );

        // do 2 jobs: 1. restart the timers (in SystemManager)
        // and 2. forward this message to client App
        procSystemSettingReply(replyModel);
    }
}

const QString ClientCodeDllUrlPathKey = "ClientCodeDllUrlPath";

QString ClientSettingManager::getClientCodeDllUrlPath()
{
    return _getServerUrl(ClientCodeDllUrlPathKey);
}

QString ClientSettingManager::_getServerUrl(const QString& key)
{
    if (m_mapServerUrl.contains(key))
    {
        return m_mapServerUrl[key];
    } else {
        return QString();
    }
    
}

void ClientSettingManager::checkLocalAdminUserNamePassword()
{
    // in order to check local admin user name, password, we always send an admin code change request to server
    // using old admin code = new admin code = current code, so that server can verify this client
    QString oldAdminNameEnc = CConfigManager::adminNameEnc();
    QString newAdminNameEnc = CConfigManager::adminNameEnc();
    QString oldAdminCodehash = CConfigManager::adminCodeHash();
    QString newAdminCodehash = CConfigManager::adminCodeHash();
    AdminCodeRequestQtModel requestModel(CConfigManager::clientId(), 
        oldAdminNameEnc, newAdminNameEnc, oldAdminCodehash, newAdminCodehash);
    cs2ssSender::send(cs2ss::AdminCodeRequestCommand(requestModel));
}
