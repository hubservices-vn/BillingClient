#include "OfflineSession.h"
#include "CommonSettings.h"
#include "SessionStatus.h"

OfflineSession::OfflineSession(quint64 sessionId, const QDateTime& startTime, int accountId, UserGroupType accType)
: m_id(0), m_sessionId(sessionId), m_offlineStartTime(startTime), 
m_offlineRecTime(QDateTime()), m_offlineStopTime(QDateTime()),
m_offlineMinutes(0), m_accountId(accountId), m_accountType(accType), m_status(STATUS_ACTIVE), m_isDeleted(false)
{
}

OfflineSession::OfflineSession(quint64 id, quint64 sessionId, const QDateTime& startDT, 
                               const QDateTime& recordDT, const QDateTime& stopDT, 
                               int minutes, int accountId, UserGroupType accType, OfflineSessionStatus status)
                               : m_id(id), m_sessionId(sessionId), m_offlineStartTime(startDT),
                               m_offlineRecTime(recordDT), m_offlineStopTime(stopDT),
                               m_offlineMinutes(minutes), m_accountId(accountId), m_accountType(accType), 
                               m_status(status), m_isDeleted(false)
{
}

int OfflineSession::getRealOfflineMinutes() const
{
    int offMinResult = m_offlineStartTime.secsTo(m_offlineRecTime) / 60;
    if (m_offlineMinutes > offMinResult)
    {
        offMinResult = m_offlineMinutes;
    }

    return offMinResult;
}

bool OfflineSession::hasEnded() const
{
    return m_offlineStopTime.isValid();
}
