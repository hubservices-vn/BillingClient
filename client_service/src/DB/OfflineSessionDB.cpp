#include "OfflineSessionDB.h"
#include <QtSql>
#include "OfflineSession.h"
#include "Utils.h"
#include "log.h"
#include "DBManager.h"
#include "CommonSettings.h"
#include "SessionStatus.h"
#include "DateTimeUtils.h"

OfflineSessionDB::OfflineSessionDB(QObject *parent) 
: QObject(parent), m_userOfflineSession(NULL)
{
}

bool OfflineSessionDB::loadDB()
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    QString queryStr = QString(
        "SELECT \
        id, SessionId, StartDT, RecordDT, \
        StopDT, MinutesUsed, AccountId, AccountType, Status\
        FROM %9 \
        ORDER BY id DESC \
        LIMIT 1")
        .arg(OFFLINE_SESSION_TABLE);

    if (!query.prepare(queryStr) ||
        !query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    while(query.next())
    {
        quint64 id = query.value(0).toULongLong();
        quint64 sessionId = query.value(1).toULongLong();
        QDateTime startDT = LegacyDateTimeUtils::uint64InSecToDateTime(query.value(2).toULongLong());
        QDateTime recordDT = LegacyDateTimeUtils::uint64InSecToDateTime(query.value(3).toULongLong());
        QDateTime stopDT = LegacyDateTimeUtils::uint64InSecToDateTime(query.value(4).toULongLong());
        int minutes = query.value(5).toInt();
        int accountId = query.value(6).toInt();
        UserGroupType accType = (UserGroupType)(query.value(7).toInt());
        OfflineSessionStatus status = (OfflineSessionStatus)(query.value(8).toInt());

        m_userOfflineSession = new OfflineSession(
            id, sessionId, startDT, recordDT, stopDT, minutes, accountId, accType, status);
        break;
    }

    return true;
}

OfflineSession* OfflineSessionDB::addOfflineSession(
    quint64 currentSessionId, const QDateTime& startDT, 
    int accId, UserGroupType accType)
{
    // delete historical session
    if (m_userOfflineSession != NULL &&
        m_userOfflineSession->isDeleted() == false)
    {
        deleteOfflineSessionFromDB();
    }

    if (m_userOfflineSession != NULL)
    {
        delete m_userOfflineSession;
        m_userOfflineSession = NULL;
    }

    // when the offline session begins, recordDT == startDT
    QDateTime recordDT = startDT;
    int offlineMins = 0;
    QDateTime stopDT = QDateTime();

    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    QString queryStr = QString(" \
                               INSERT INTO %1 \
                               (SessionId, StartDT, RecordDT, \
                               MinutesUsed, AccountId, AccountType, Status) \
                               VALUES(?, ?, ?, ?, ?, ?, ?)")
                               .arg(OFFLINE_SESSION_TABLE);

    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return NULL;
    }

    query.addBindValue(currentSessionId);
    query.addBindValue(LegacyDateTimeUtils::dateTimeToUInt64InSec(startDT));
    query.addBindValue(LegacyDateTimeUtils::dateTimeToUInt64InSec(recordDT));
    // when the offline session just starts, the offline minutes is 0
    query.addBindValue(offlineMins);
    query.addBindValue(accId);
    query.addBindValue((int)accType);
    OfflineSessionStatus status = STATUS_ACTIVE;
    query.addBindValue((int)status);

    if(!query.exec())
    {
        LOG_ERROR << query.lastError().text().toStdString();
        return NULL;
    }

    quint64 id = query.lastInsertId().toULongLong();
    m_userOfflineSession = new OfflineSession(
        id, currentSessionId, startDT, recordDT, stopDT, 
        offlineMins, accId, accType, status);

    return m_userOfflineSession;
}

bool OfflineSessionDB::updateOfflineSession(
    OfflineSession* session, const QDateTime& recordDT, int offlineMinutes)
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    QString queryStr = QString(" \
                               UPDATE %1 \
                               SET RecordDT=?, MinutesUsed=? \
                               WHERE id = ?")
                               .arg(OFFLINE_SESSION_TABLE);

    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(LegacyDateTimeUtils::dateTimeToUInt64InSec(recordDT));
    query.addBindValue(offlineMinutes);
    query.addBindValue(session->Id());

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    session->SetOfflineRecTime(recordDT);
    session->SetOfflineMinutes(offlineMinutes);

    return true;
}

bool OfflineSessionDB::endOfflineSession(
    OfflineSession* session, const QDateTime& endDT, OfflineSessionStatus status)
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    QString queryStr = QString(" \
                               UPDATE %1 \
                               SET StopDT=?, Status=? \
                               WHERE id = ?")
                               .arg(OFFLINE_SESSION_TABLE);

    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(LegacyDateTimeUtils::dateTimeToUInt64InSec(endDT));
    query.addBindValue((int)status);
    query.addBindValue(session->Id());

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    session->SetOfflineStopTime(endDT);
    session->setStatus(status);

    return true;
}

bool OfflineSessionDB::deleteOfflineSessionFromDB()
{
    QSqlDatabase db = DBManager::mainDB();
    QSqlQuery query(db);

    // delete records from DB
    QString queryStr = QString("DELETE FROM %1")
                               .arg(OFFLINE_SESSION_TABLE);

    if (!query.prepare(queryStr) ||
        !query.exec())
    {
        LOG_ERROR << query.lastError().text().toStdString();
        return false;
    }

    if (m_userOfflineSession != NULL)
    {
        m_userOfflineSession->setIsDeleted(true);
    }

    return true;
}


