#include "SessionManager.h"
#include "SessionStatusQtModel.h"
#include "Session.h"
#include "Client.h"
#include "SessionEndQtModel.h"
#include "SessionBeginQtModel.h"


bool SessionManager::init()
{
    m_session = NULL;
    return true;
}

bool SessionManager::connectSigSlot()
{
    return true;
}

void SessionManager::procSessionStatus( const SessionStatusQtModel &qtModel )
{
    // this is a new session
    if (m_session == NULL)
    {
        m_session = new Session(qtModel.SessionId, qtModel.MinutesUsed, 
            qtModel.MinutesLeft, qtModel.TimeFee, qtModel.ServiceFee);
    }
    else
    {
        if (m_session->id() != qtModel.SessionId)
        {
            delete m_session;
            m_session = new Session(qtModel.SessionId, qtModel.MinutesUsed, 
                qtModel.MinutesLeft, qtModel.TimeFee, qtModel.ServiceFee);
        }
        else
        {
            m_session->updateValue(qtModel.MinutesUsed, 
                qtModel.MinutesLeft, qtModel.TimeFee, qtModel.ServiceFee);
        }
    }

    emit sessionUpdated();
}

void SessionManager::procSessionEnd(const SessionEndQtModel& qtModel)
{
    endSession();
}

void SessionManager::procSessionBegin( const SessionBeginQtModel& qtModel )
{
    emit sessionStarted();
}

void SessionManager::endSession()
{
    if (m_session != NULL)
    {
        delete m_session;
        m_session = NULL;
    }
    emit sessionEnded();
}

quint64 SessionManager::currentSessionId() const
{
    if (m_session != NULL)
    {
        return m_session->id();
    }

    // don't return -1, as in protocalBuffer, this is defined as uint64
    return 0;
}

int SessionManager::totalMinutes() const
{
    if (m_session != NULL)
    {
        return (
            m_session->minutesLeft() + m_session->minutesUsed()
            );
    }

    return 0;
}

int SessionManager::minutesUsed() const
{
    if (m_session != NULL)
    {
        return m_session->minutesUsed();
    }

    return 0;
}

int SessionManager::minutesLeft() const
{
    if (m_session != NULL)
    {
        return m_session->minutesLeft();
    }

    return 0;
}

double SessionManager::timeFee() const
{
    if (m_session != NULL)
    {
        return m_session->timeFee();
    }

    return 0;
}

double SessionManager::serviceFee() const
{
    if (m_session != NULL)
    {
        return m_session->serviceFee();
    }

    return 0;
}
