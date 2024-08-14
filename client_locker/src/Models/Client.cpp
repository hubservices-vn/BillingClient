#include "Client.h"
#include "log.h"

#include "ConfigManager.h"
#include "cl2csSocketManager.h"
#include "cl2csNetworkManager.h"
#include "FullScreenDialog2.h"
#include "cs2clCmdProcs.h"
#include "FlashManager.h"
#include "SessionManager.h"
#include "SystemManager.h"
#include "UserAccountManager.h"
#include "HeartbeatSender.h"

Client::~Client()
{
}

bool Client::init()
{
    bool ret = true;

    cl2csSocketManager::GetInstance()->init();
    cl2csNetworkManager::GetInstance()->init();

    UserAccountManager::GetInstance()->init();
    SessionManager::GetInstance()->init();
    FlashManager::GetInstance()->init();
    SystemManager::GetInstance()->init();
    HeartBeatSender::GetInstance()->init();
    if (!ret)
    {
        LOG_ERROR << "Client failed to init";
    }
    return ret;
}

bool Client::connectSigSlot()
{
    cl2csSocketManager::GetInstance()->connectSigSlot();
    cl2csNetworkManager::GetInstance()->connectSigSlot();

    UserAccountManager::GetInstance()->connectSigSlot();
    SessionManager::GetInstance()->connectSigSlot();
    FlashManager::GetInstance()->connectSigSlot();
    SystemManager::GetInstance()->connectSigSlot();
    HeartBeatSender::GetInstance()->connectSigSlot();
    return true;
}

bool Client::startUp()
{
    bool ret = true;

    cs2clCmdProcs::GetInstance()->registerProcs();

    cl2csSocketManager::GetInstance()->connectSocket();

    return true;
}

