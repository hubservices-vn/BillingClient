#include "GbProtectManager.h"
#include "gbkernelapi.h"
#include "WorkThreadManager.h"
#include "OSAgentWinQt.h"

GbProtectManager_p::GbProtectManager_p(QObject *parent) 
: QObject(parent), m_gbKernal(NULL)
{
}

GbProtectManager_p::~GbProtectManager_p()
{
#ifdef NDEBUG
    if ( ! m_sysHBTimer.isNull())
    {
        m_sysHBTimer->stop();
    }

    if (m_gbKernal != NULL)
    {
        GBKernel_Destory(m_gbKernal);
        m_gbKernal = NULL;
    }
#endif
}

const int DEF_SYS_HB_INTERVAL = 5000; // 5,000 ms
void GbProtectManager_p::init()
{
    for (int i = 0; i < 60; ++i)
    {
        if (OSAgentWinQt::GetInstance()->getExplorerPid() != 0)
        {
            break;
        }

        LOG_DEBUG4 << "gbpm waiting...";
        ::Sleep(1000);
    }

#ifdef NDEBUG
    LOG_DEBUG4 << "create gbk";
    m_gbKernal = GBKernel_Create();
    m_gbKernal->Install();
    LOG_DEBUG4 << "done";

    m_sysHBTimer.reset(new QTimer());
    m_sysHBTimer->setInterval(DEF_SYS_HB_INTERVAL);
    connect(m_sysHBTimer.data(), SIGNAL(timeout()),
        this, SLOT(sysHeartbeatSlot()));
    m_sysHBTimer->start();

    // do it once first
    m_gbKernal->HeartBeat(GetCurrentProcessId());
#endif
}

void GbProtectManager_p::sysHeartbeatSlot()
{
    quint32 pid = GetCurrentProcessId();
    //LOG_DEBUG4 << "shbing " << pid;

#ifdef NDEBUG
    m_gbKernal->HeartBeat(pid);
#endif
}

void GbProtectManager_p::onAppEndConnection()
{
    //LOG_DEBUG4 << "shbing " << -1;

#ifdef NDEBUG
    m_gbKernal->HeartBeat(-1);
#endif
}



bool GbProtectManager::init()
{
    m_pimp.reset(new GbProtectManager_p());
    m_pimp->moveToThread(WorkThreadManager::GetInstance()->gbProtectThread());

    QTimer::singleShot(0, this, SLOT(init_imp()));

    return true;
}

void GbProtectManager::init_imp()
{
    LOG_DEBUG4 << "init gbpm";
    bool retCode = QMetaObject::invokeMethod(
        m_pimp.data(), "init", Qt::QueuedConnection);
}

bool GbProtectManager::connectSigSlot()
{
    return true;
}

void GbProtectManager::onAppEndConnection()
{
    bool retCode = QMetaObject::invokeMethod(
        m_pimp.data(), "onAppEndConnection", Qt::QueuedConnection);
}
