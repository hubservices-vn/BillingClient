#ifndef OFFLINESESSION_H
#define OFFLINESESSION_H

#include <QObject>
#include <QDateTime>
#include "stdtypes.h"

enum UserGroupType;
enum OfflineSessionStatus;

class OfflineSession
{
public:
    // used when adding a new offline session
    explicit OfflineSession(quint64 sessionId, const QDateTime& startTime, int accountId, UserGroupType accType);
    // used when load database records
    explicit OfflineSession(quint64 id, quint64 sessionId, 
        const QDateTime& startDT, const QDateTime& recordDT, const QDateTime& stopDT, 
        int minutes, int accountId, UserGroupType accType, OfflineSessionStatus status);

    bool hasEnded() const;

    // we use 2 ways to calculate the offline time:
    // 1) the current time - the start time;
    // 2) the self-increased offlineMinutes (increased every minute)
    // the real offline minutes will be the greater one of these 2 values
    int getRealOfflineMinutes() const;

public:
    quint64 Id() const { return m_id; }
    void SetId(quint64 val) { m_id = val; }

    quint64 SessionId() const { return m_sessionId; }
    void SetSessionId(quint64 val) { m_sessionId = val; }

    QDateTime OfflineStartTime() const { return m_offlineStartTime; }
    void SetOfflineStartTime(const QDateTime& val) { m_offlineStartTime = val; }

    QDateTime OfflineRecTime() const { return m_offlineRecTime; }
    void SetOfflineRecTime(const QDateTime& val) { m_offlineRecTime = val; }

    QDateTime OfflineStopTime() const { return m_offlineStopTime; }
    void SetOfflineStopTime(const QDateTime& val) { m_offlineStopTime = val; }

    int OfflineMinutes() const { return m_offlineMinutes; }
    void SetOfflineMinutes(int val) { m_offlineMinutes = val; }

    int AccountId() const { return m_accountId; }
    void SetAccountId(int val) { m_accountId = val; }

    UserGroupType accountType() const { return m_accountType; }
    void setAccountType(UserGroupType val) { m_accountType = val; }

    OfflineSessionStatus status() const { return m_status; }
    void setStatus(OfflineSessionStatus val) { m_status = val; }

    bool isDeleted() const { return m_isDeleted; }
    void setIsDeleted(bool val) { m_isDeleted = val; }
private:
    quint64 m_id;
    
    quint64 m_sessionId;

    QDateTime m_offlineStartTime; // when the offline begins
    QDateTime m_offlineRecTime; // last time we recorded the offline (minutes elapsed = m_offlineRecTime - m_offlineStartTime);
    QDateTime m_offlineStopTime;

    int m_offlineMinutes;
    
    int m_accountId;
    UserGroupType m_accountType;
    OfflineSessionStatus m_status;
    
    // only in memory
    // helper
    // this value will be true if it's removed from DB but kept in memory
    // scenario: when offlineSessionReply command comes, we'll delete the offline session from DB
    // but won't delete it from memory immediately (will wait until all finished and the beginConnectionReplyCommand comes,
    // because only after beginConnectionReplyCommand comes, we can say that all the offlineSessionReply commands have already come
    // (root reason: don't want to delete items from a list when iterating it)
    bool m_isDeleted; 
};


#endif // OFFLINESESSION_H
