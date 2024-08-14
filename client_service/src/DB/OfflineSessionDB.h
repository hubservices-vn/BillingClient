#ifndef OFFLINESESSIONDB_H
#define OFFLINESESSIONDB_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include "stdtypes.h"

class OfflineSession;
enum UserGroupType;
enum OfflineSessionStatus;
class OfflineSessionDB : public QObject
{
    Q_OBJECT
public:
    explicit OfflineSessionDB(QObject *parent = 0);

    // read all the data from DB
    bool loadDB();

    OfflineSession* addOfflineSession(quint64 currentSessionId, const QDateTime& dateTime, int currentAccId, UserGroupType accType);

    bool updateOfflineSession(OfflineSession* session, const QDateTime& recordDT, int offlineMinutes);

    bool endOfflineSession(OfflineSession* session, const QDateTime& endDT, OfflineSessionStatus status);

    bool deleteOfflineSessionFromDB();

private:
    // there will be AT MOST only one offline session in DB
    OfflineSession* m_userOfflineSession;

    friend class OfflineSessionManager;
};

#endif // OFFLINESESSIONDB_H
