#include "stdafx.h"
#include "Client.h"
#include "cs2ssSocketManager.h"
#include "cs2ssNetworkManager.h"
#include "OSAgentWinQt.h"
#include "SecurityProfileManager.h"
#include "cs2ccSocketManager.h"
#include "cs2ccNetworkManager.h"
#include "cc2csCmdProcs.h"
#include "ss2csCmdProcs.h"
#include "UserAccountManager.h"
#include "SessionManager.h"
#include "OfflineSessionManager.h"
#include "AppControlManager.h"
#include "ProductManager.h"
#include "ClientSettingManager.h"
#include "SystemManager.h"
#include "WebControlManager2.h"
#include "WebHistoryManager.h"
#include "HeartBeatManager.h"
#include "OrderManager.h"
#include "TextMessageManager.h"
#include "gcaInfoManager.h"
#include "UIAppManager.h"
#include "cs2clSocketManager.h"
#include "cs2clNetworkManager.h"
#include "cl2csCmdProcs.h"
#include "GbProtectManager.h"
#include "TickCountHelper.h"

bool Client::init()
{
    bool ret = true;
    ret = ret && AppControlManager::GetInstance()->init();
    ret = ret && SecurityProfileManager::GetInstance()->init();

    ret = ret && cs2ssSocketManager::GetInstance()->init();
    ret = ret && cs2ssNetworkManager::GetInstance()->init();

    ret = ret && cs2ccSocketManager::GetInstance()->init();
    ret = ret && cs2ccNetworkManager::GetInstance()->init();

    ret = ret && cs2clSocketManager::GetInstance()->init();
    ret = ret && cs2clNetworkManager::GetInstance()->init();

    ret = ret && OrderManager::GetInstance()->init();
    ret = ret && UserAccountManager::GetInstance()->init();
    ret = ret && SessionManager::GetInstance()->init();
    ret = ret && SystemManager::GetInstance()->init();
    ret = ret && ProductManager::GetInstance()->init();
    ret = ret && ClientSettingManager::GetInstance()->init();
    ret = ret && OfflineSessionManager::GetInstance()->init();
    ret = ret && WebControlManager2::GetInstance()->init();
    ret = ret && WebHistoryManager::GetInstance()->init();

    ret = ret && SvrHeartBeatSender::GetInstance()->init();
    ret = ret && ClientHeartBeatChecker::GetInstance()->init();

    ret = ret && TextMessageManager::GetInstance()->init();
    ret = ret && GcaInfoManager::GetInstance()->init();
    ret = ret && UIAppManager::GetInstance()->init();
    ret = ret && GbProtectManager::GetInstance()->init();

    ret = ret && TickCountHelper::GetInstance()->init();
    return ret;
}

bool Client::connectSigSlot()
{
    cs2ssSocketManager::GetInstance()->connectSigSlot();
    cs2ssNetworkManager::GetInstance()->connectSigSlot();

    cs2ccSocketManager::GetInstance()->connectSigSlot();
    cs2ccNetworkManager::GetInstance()->connectSigSlot();

    cs2clSocketManager::GetInstance()->connectSigSlot();
    cs2clNetworkManager::GetInstance()->connectSigSlot();

    AppControlManager::GetInstance()->connectSigSlot();
    SecurityProfileManager::GetInstance()->connectSigSlot();

    OrderManager::GetInstance()->connectSigSlot();
    UserAccountManager::GetInstance()->connectSigSlot();
    SessionManager::GetInstance()->connectSigSlot();
    SystemManager::GetInstance()->connectSigSlot();
    ProductManager::GetInstance()->connectSigSlot();
    ClientSettingManager::GetInstance()->connectSigSlot();
    OfflineSessionManager::GetInstance()->connectSigSlot();
    WebControlManager2::GetInstance()->connectSigSlot();
    WebHistoryManager::GetInstance()->connectSigSlot();
    GcaInfoManager::GetInstance()->connectSigSlot();

    SvrHeartBeatSender::GetInstance()->connectSigSlot();
    ClientHeartBeatChecker::GetInstance()->connectSigSlot();
    TextMessageManager::GetInstance()->connectSigSlot();
    UIAppManager::GetInstance()->connectSigSlot();
    GbProtectManager::GetInstance()->connectSigSlot();

    return true;
}

bool Client::startUp()
{
    bool succeeded = true;

    // register the command handler
    ss2csCmdProcs::GetInstance()->registerProcs();

    cc2csCmdProcs::GetInstance()->registerProcs();

    cl2csCmdProcs::GetInstance()->registerProcs();

    // start the timer to shutdown or clear application on AVAILABLE computers
    succeeded = succeeded && SystemManager::GetInstance()->startup();

    cs2ssSocketManager::GetInstance()->connectSocket();

    succeeded = succeeded && cs2ccSocketManager::GetInstance()->startListen();
    succeeded = succeeded && cs2clSocketManager::GetInstance()->startListen();

    return succeeded;
}

bool Client::stop()
{
    LOG_DEBUG << "stop client service";
    cs2ssSocketManager::GetInstance()->disconnectSocket();
    cs2ccSocketManager::GetInstance()->stopListen();
    cs2clSocketManager::GetInstance()->stopListen();

    return true;
}
