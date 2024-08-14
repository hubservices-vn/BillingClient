#include "GcaInfoManager.h"
#define GCACLIENTDLL
#include "IGCAClient.h"
#include "cs2ssNetworkManager.h"
#include "CafeInfoQtModel.h"
#include "ConfigManager.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"

#ifdef NDEBUG
// release
const int DELAY_REPORT_INTERVAL = 30000; // 30,000ms = 30s
#else
// debug
const int DELAY_REPORT_INTERVAL = 1000; // 1,000ms = 1s
#endif

bool GcaInfoManager::init()
{
    return true;
}

bool GcaInfoManager::connectSigSlot()
{
    // server connected, report the gcaID to server
    {
        cs2ssNetworkManager* mgr = cs2ssNetworkManager::GetInstance();
        bool ret = connect(mgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
        LOG_DEBUG << "GcaInfoManager::connectSigSlot result: " << ret;
    }

    return true;
}

void GcaInfoManager::serverConnectedSlot()
{
    LOG_DEBUG << "Server connected. Report GCA id in " << DELAY_REPORT_INTERVAL << " ms";

    QTimer::singleShot(DELAY_REPORT_INTERVAL, 
        this, SLOT(_delayGetGcaIdAndReport()));
}

void GcaInfoManager::_delayGetGcaIdAndReport()
{
    LOG_DEBUG << "Begin get gcaId";
    static bool hasReported = false;

#ifdef NDEBUG
    // only report once
    if (hasReported)
    {
        LOG_DEBUG << "Already reported, skip";
        return;
    }
#endif

    int gcaId = _getGcaId();
    LOG_DEBUG << "gcaId on client side is: " << gcaId;

    if (gcaId != -1)
    {
        // write this to registry
        CConfigManager::setCafeId(gcaId);
    }

    CafeInfoReportQtModel qtModel(
        CConfigManager::clientId(), 
        gcaId);
    cs2ssSender::send(cs2ss::CafeInfoReportCommand(qtModel));

    LOG_DEBUG << "gcaId reported to server";
    hasReported = true;
}

int GcaInfoManager::_getGcaId()
{
    int gcaId;
    BOOL result = GCA_GetCafeID(&gcaId);
    if (result)
    {
        return gcaId;
    }

    return -1;
}
