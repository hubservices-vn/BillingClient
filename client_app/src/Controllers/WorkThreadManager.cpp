#include "WorkThreadManager.h"

bool WorkThreadManager::initThreads()
{
    m_cc2csNetworkThread = new QThread();

    return true;
}

bool WorkThreadManager::connectSigSlot()
{
    return true;
}

bool WorkThreadManager::startThreads()
{
    m_cc2csNetworkThread->start();

    return true;
}
