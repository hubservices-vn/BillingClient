#include "SystemManager.h"
#include "ScreenLockQtModel.h"
#include "log.h"
#include "OSAgentWinQt.h"
#include <QPixmap>
#include <QtCore>
#include <QMessageBox>
#include <QApplication>
#include "cc2csCmds.h"
#include "ScreencaptureQtModel.h"
#include <QDesktopWidget>
#include <QCoreApplication>
#include <QByteArray>
#include <QBuffer>
#include "cc2csSender.h"
#include "ConfigManager.h"
#include "ClientConnectionQtModel.h"
#include "UpdateManager.h"
#include "QueryTermAppQtModel.h"
#include "CommonSettings.h"
#include "PcActionQtModel.h"
#include "GuiController.h"
#include "SystemMessageQtModel.h"
#include "ClientSettingManager.h"
#include "UserAccountManager.h"
#include "DateTimeUtils.h"
#include "UpdateClientQtModel.h"
#include "RegionConsts.h"

const QString TIGHT_VNC_SERVER_NAME = "tvnserver.exe";
const QString ULTRA_VNC_SERVER_NAME = "winvnc.exe";

// TODO: put this into encrypted config files later
const QString GB_CLIENT_SVC_NAME = "gbClientService.exe";
const QString GB_CLIENT_APP_NAME = "gbClientApp.exe";
const QString AUTO_UPD_NAME = "AutoUpdateH.exe";
const QString AUTO_UPD2_NAME = "AutoUpdateH2.exe";
const QString GB_CLIENT_LOCKER_NAME = "gbClientLocker.exe";

const QString GARENA_CIG_NAME = "garenacig.exe";
const QString GCAFE_PRO_NAME = "barclientview.exe";


const QString EXPLORER_NAME = "explorer.exe";
const QString GCAFE_PRO_CLINET_NAME = "CafeClient.exe";
const QString GCAFE_PRO_SERVER_NAME = "CafeServer.exe";
const QString GCAFE_PRO_CONSOLE_NAME = "CafeConsole.exe";
const QString GGHOST_NAME = "ggdllhost.exe";

SystemManager::~SystemManager()
{
}

bool SystemManager::init()
{
    // don't report these apps to server / kill them when "kill all applications"
    m_excludeAppKillList.clear();
    m_excludeAppKillSet.clear();
	m_excludeAppKillList 
        << TIGHT_VNC_SERVER_NAME 
        << ULTRA_VNC_SERVER_NAME
        << GB_CLIENT_APP_NAME 
        << GB_CLIENT_SVC_NAME
        << AUTO_UPD_NAME
        << AUTO_UPD2_NAME
        << GB_CLIENT_LOCKER_NAME
        << GARENA_CIG_NAME
        << GCAFE_PRO_NAME 
        << EXPLORER_NAME
        << GCAFE_PRO_CLINET_NAME
        << GCAFE_PRO_SERVER_NAME
        << GCAFE_PRO_CONSOLE_NAME
        << GGHOST_NAME;
    return true;
}

bool SystemManager::connectSigSlot()
{
    return true;
}

bool SystemManager::startup()
{
    //runVncServer();

    return true;
}

void SystemManager::procScreenLockReply( const ScreenLockReplyQtModel &qtModel )
{
    emit screenLockReplyRecved(qtModel.Mode_, qtModel.Result_);
}

void SystemManager::procScreenUnlockReply( const ScreenUnlockReplyQtModel& qtModel )
{
    emit screenUnlockReplyRecved(qtModel.Mode_, qtModel.Result_);
}

void SystemManager::procScreenCaptureRequest( const ScreencaptureRequestQtModel &qtModel )
{
    QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    QByteArray byteArray;
    QBuffer buffer( &byteArray );
    buffer.open( QIODevice::WriteOnly );
    bool result = pixmap.save( &buffer, "PNG" ); // writes pixmap into byteArray in PNG format
    if (result == false)
    {
        LOG_DEBUG4 << "Failed to save the pixmap to PNG format";
        return;
    }

    ScreencaptureReplyQtModel replyModel(CConfigManager::clientId(), byteArray);
    cc2csSender::send(cc2cs::ScreenCaptureReplyCommandC(replyModel));
}

bool SystemManager::runVncServer()
{
    if (CConfigManager::isDeveloperEnv() == true)
    {
        return true;
    }

    if (RegionConsts::isID())
    {
        _runUltraVNC();
    }
    else
    {
        _runTightVNC();
    }
	return true;
}

void SystemManager::procUpdateClientRequest( const UpdateClientRequestQtModel &qtModel )
{
    UpdateManager::GetInstance()->beginUpdate(
        qtModel.ClientTargetVerNum_, qtModel.Hash_);
}

void SystemManager::procQueryAppRequest( const QueryAppRequestQtModel &qtModel )
{
    QList<QueryTermApp_AppItem> procList;
    OSAgentWinQt::GetInstance()->getRunningProcessNameListEx(procList, m_excludeAppKillList);

    QueryAppReplyQtModel replyModel(CConfigManager::clientId());
    replyModel.AppList_.append(procList);

    cc2csSender::send(cc2cs::QueryAppReplyCommandC(replyModel));
}


void SystemManager::procTermAppRequest( const TerminateAppRequestQtModel &qtModel )
{
    foreach(const QueryTermApp_AppItem &item, qtModel.AppList_)
    {
        ProcessType type = item.Type_;
        QString name = item.Name_;
        int pid = item.Pid_;
        int handle = item.Handle_;

        if (type == ProcessType_Process)
        {
            OSAgentWinQt::GetInstance()->killProcessByPid(pid);
        }
        else if (type == ProcessType_Window)
        {
            OSAgentWinQt::GetInstance()->killProcessByWnd(handle);
        }
    }
}

void SystemManager::procSystemMessage( const SystemMessageQtModel& qtModel )
{
    GuiController::GetInstance()->showSystemMessage(qtModel.Type_, qtModel.Level_, qtModel.Msg_);
}

void SystemManager::procPcAction( const PcActionRequestQtModel& qtModel )
{
    _pcActionImp(qtModel.ActionType_);
}

void SystemManager::_pcActionImp( PCActionType type )
{
    // only in DEBUG mode
    if (CConfigManager::isDeveloperEnv() == true)
    {
        QString msg = "simulated PC action: " + PcActionTypeToQString(type);
        QMessageBox::about(NULL, "info", msg);
        LOG_DEBUG4 << "simulated PC action: " << qPrintable(PcActionTypeToQString(type));
        return;
    }
    else
    {
        LOG_DEBUG4 << "PC action: " << qPrintable(PcActionTypeToQString(type));
        // when reboot/shutdown, the client
        // doesn't close the socket connection immediately
        // so we do this actively by invoking the 
        // closeSocket() method of SocketManager
        // but DON"T do this to standby, imagine: server asks client to actively disconnect,
        // after standby, the client doesn't know to re-connect
        if (type == PCActionType_Shutdown)
        {
            OSAgentWinQt::GetInstance()->killAllRunningApps(m_excludeAppKillList);
            ::Sleep(3000);
            OSAgentWinQt::GetInstance()->shutdownComputer();
        }
        else if (type == PCActionType_Reboot)
        {
            OSAgentWinQt::GetInstance()->killAllRunningApps(m_excludeAppKillList);
            ::Sleep(3000);
            OSAgentWinQt::GetInstance()->restartComputer();
        }
        else if (type == PCActionType_Standby)
        {
            //cs2ssSocketManager::GetInstance()->disconnectSocket();
            OSAgentWinQt::GetInstance()->standbyComputer();
        }
        else if (type == PCActionType_ClearApp)
        {
            OSAgentWinQt::GetInstance()->killAllRunningApps(m_excludeAppKillList);
        }
        else if (type == PCActionType_RemoteControl)
        {
            bool ret = runVncServer();

            RemoteControlReplyQtModel replyModel(CConfigManager::clientId(), ret);
            cc2csSender::send(cc2cs::RemoteControlReplyCommandC(replyModel));
        }
    }
}

void SystemManager::_runTightVNC()
{
    OSAgentWin* osAgent = OSAgentWinQt::GetInstance();
    if (osAgent->isProcessRunning(TIGHT_VNC_SERVER_NAME.toStdWString()) == true)
    {
        return;
    }

    // set the registry, to disable password-based authentication (so that server can connect directly without a password
    QString keyHive = "HKEY_CURRENT_USER\\Software\\TightVNC\\Server";
    QSettings settings( keyHive, QSettings::NativeFormat );
    settings.setValue( QString("UseVncAuthentication"), 0);
    settings.setValue( QString("UseControlAuthentication"), 0);
    settings.setValue( QString("RunControlInterface"), 0);
    settings.setValue( QString("AcceptHttpConnections"), 0);
    settings.setValue( QString("RemoveWallpaper"), 0);
    settings.setValue( QString("PollingInterval"), 200);

    // run the vnc program
    QString appFolder = QCoreApplication::applicationDirPath();
    QString vncFullPath = appFolder + "/" + TIGHT_VNC_SERVER_NAME;
    if (!QFile::exists(vncFullPath))
    {
        return;
    }

    QStringList args;
    QProcess::startDetached(vncFullPath, args);
}

void SystemManager::_runUltraVNC()
{
    OSAgentWin* osAgent = OSAgentWinQt::GetInstance();
    if (osAgent->isProcessRunning(ULTRA_VNC_SERVER_NAME.toStdWString()) == true)
    {
        return;
    }

    // run the vnc program
    QString appFolder = QCoreApplication::applicationDirPath();
    QString vncFullPath = appFolder + "/" + ULTRA_VNC_SERVER_NAME;
    if (!QFile::exists(vncFullPath))
    {
        return;
    }

    QStringList args;
    QProcess::startDetached(vncFullPath, args);
}

void SystemManager::addExcludeApp(const QString& appName)
{
    if(!m_excludeAppKillSet.contains(appName))
    {
        m_excludeAppKillList.append(appName);
        m_excludeAppKillSet.insert(appName);
        LOG_DEBUG4 << "add exclude app " << qPrintable(appName);
    }
}

void SystemManager::addExcludeApps(const QList<QString>& appNames)
{
    foreach(QString appName, appNames)
    {
        if(!m_excludeAppKillSet.contains(appName))
        {
            m_excludeAppKillList.append(appName);
            m_excludeAppKillSet.insert(appName);
            LOG_DEBUG4 << "add exclude app " << qPrintable(appName);
        }
    }
}

void SystemManager::removeExcludeApp(const QString& appName)
{
    if(m_excludeAppKillSet.contains(appName))
    {
        m_excludeAppKillList.removeOne(appName);
        m_excludeAppKillSet.remove(appName);
        LOG_DEBUG4 << "remove exclude app " << qPrintable(appName);
    }
}
