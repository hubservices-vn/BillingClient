#include "Client.h"
#include "cc2csSocketManager.h"
#include "log.h"
#include "ConfigManager.h"
#include "OSAgentWinQt.h"
#include "cc2csNetworkManager.h"
#include "MiniDialog.h"
#include "cs2ccCmdProcs.h"
#include "WorkThreadManager.h"
#include "UserAccountManager.h"
#include "SessionManager.h"
#include "AdminOptionManager.h"
#include "ProductManager.h"
#include "TextMessageManager.h"
#include "SystemManager.h"
#include "WebHistoryManager.h"
#include "SecurityProfileManager.h"
#include "AppControlManager.h"
#include "ClientSettingManager.h"
#include "ClientAdManager.h"

#include "HeartbeatSender.h"
#include "ClientWallpaperManager.h"

Client::~Client()
{
}

bool Client::init()
{
    bool ret = true;

    // pre-init
    {
        // data (eager initialization)
        m_threadMgr = WorkThreadManager::GetInstance();
        // the work thread init must come first
        ret = ret && m_threadMgr->initThreads();
        m_threadMgr->startThreads();
    }

    // construct those instances
    {
        m_socketMgr = cc2csSocketManager::GetInstance();
        //m_socketMgr->moveToThread(m_threadMgr->cc2csNetworkThread());

        m_networkMgr = cc2csNetworkManager::GetInstance();
        //m_networkMgr->moveToThread(m_threadMgr->cc2csNetworkThread());

        m_userAccMgr = UserAccountManager::GetInstance();
        m_sessionMgr = SessionManager::GetInstance();
    }

    m_socketMgr->init();
    m_networkMgr->init();
    //QMetaObject::invokeMethod(m_socketMgr, "init", Qt::QueuedConnection);
    //QMetaObject::invokeMethod(m_networkMgr, "init", Qt::QueuedConnection);
    m_userAccMgr->init();
    m_sessionMgr->init();
    AdminOptionManager::GetInstance()->init();
    ProductManager::GetInstance()->init();
    SystemManager::GetInstance()->init();
    TextMessageManager::GetInstance()->init();
    WebHistoryManager::GetInstance()->init();
    SecurityProfileManager::GetInstance()->init();
    AppControlManager::GetInstance()->init();
    ClientSettingManager::GetInstance()->init();
    ClientAdManager::GetInstance()->init();
    HeartBeatSender::GetInstance()->init();
    ClientWallpaperManager::GetInstance()->init();
    return ret;
}

bool Client::connectSigSlot()
{
    m_socketMgr->connectSigSlot();
    m_networkMgr->connectSigSlot();
    //QMetaObject::invokeMethod(m_socketMgr, "connectSigSlot", Qt::QueuedConnection);
    //QMetaObject::invokeMethod(m_networkMgr, "connectSigSlot", Qt::QueuedConnection);
    m_userAccMgr->connectSigSlot();
    m_sessionMgr->connectSigSlot();
    AdminOptionManager::GetInstance()->connectSigSlot();
    ProductManager::GetInstance()->connectSigSlot();
    SystemManager::GetInstance()->connectSigSlot();
    TextMessageManager::GetInstance()->connectSigSlot();
    WebHistoryManager::GetInstance()->connectSigSlot();
    SecurityProfileManager::GetInstance()->connectSigSlot();
    AppControlManager::GetInstance()->connectSigSlot();
    ClientSettingManager::GetInstance()->connectSigSlot();
    ClientAdManager::GetInstance()->connectSigSlot();
    HeartBeatSender::GetInstance()->connectSigSlot();
    ClientWallpaperManager::GetInstance()->connectSigSlot();

    return true;
}

bool Client::startUp()
{
    bool ret = true;

    cs2ccCmdProcs::GetInstance()->registerProcs();

    //QMetaObject::invokeMethod(m_socketMgr, "connectSocket", Qt::QueuedConnection);
    m_socketMgr->connectSocket();

    OSAgentWin* agent = OSAgentWinQt::GetInstance();
    if (agent != NULL)
    {
        agent->registerAsAutoRun();
        agent->initSecurityProtection();
    }
    else
    {
        LOG_ERROR << "can't get osagent intance, failed to apply security";
    }

    // apply the security profile (can use timer for delay-load?)
    SecurityProfileManager::GetInstance()->startup();

    // run vnc server
    SystemManager::GetInstance()->startup();

    return true;
}

