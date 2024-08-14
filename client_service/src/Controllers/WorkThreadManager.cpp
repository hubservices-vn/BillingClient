#include "WorkThreadManager.h"

WorkThreadManager::~WorkThreadManager()
{
    if (m_gbProtectThread != NULL)
    {
        m_gbProtectThread->exit();
        m_gbProtectThread->deleteLater();
        m_gbProtectThread = NULL;
    }
}

bool WorkThreadManager::initThreads()
{
    m_gbProtectThread = new QThread();
    return true;
}

bool WorkThreadManager::startThreads()
{
    m_gbProtectThread->start(QThread::LowPriority);
    return true;
}

QThread* WorkThreadManager::gbProtectThread() const
{
    return m_gbProtectThread;
}
