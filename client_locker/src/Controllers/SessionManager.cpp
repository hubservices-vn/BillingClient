#include "SessionManager.h"
#include "Client.h"
#include "SessionEndQtModel.h"
#include "SessionBeginQtModel.h"

bool SessionManager::init()
{
    m_currentSessionId = 0;
    return true;
}

bool SessionManager::connectSigSlot()
{
    return true;
}

void SessionManager::procSessionEnd(const SessionEndQtModel& /*qtModel*/)
{
    emit sessionEnded();
}

void SessionManager::procSessionBegin( const SessionBeginQtModel& qtModel)
{
    emit sessionStarted();
}
