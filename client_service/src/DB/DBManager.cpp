#include "DBManager.h"
#include "ConfigManager.h"
#include "log.h"
#include <QtSql>
#include <QFile>
#include <QStringList>
#include <QDir>
#include "PathManager.h"

const QString INIT_CONFIG_TABLE = 
"CREATE TABLE IF NOT EXISTS %1 ( \
name TEXT primary key, \
value TEXT, \
enabled INTEGER); ";

/* columns:
1 -- sessionId INTEGER, 
2 -- startDt INTEGER, 
3 -- recordDt INTEGER, 
4 -- stopDt INTEGER, 
5 -- minutes INTEGER, 
6 -- accountId INTEGER, 
7 -- accountType INTEGER,
8 -- status INTEGER
*/
const QString INIT_OFFLINE_SESSION_TABLE = 
"CREATE TABLE IF NOT EXISTS %1 ( \
id INTEGER primary key AUTOINCREMENT, \
SessionId INTEGER, \
StartDT INTEGER, \
RecordDT INTEGER, \
StopDT INTEGER, \
MinutesUsed INTEGER, \
AccountId INTEGER, \
AccountType INTEGER, \
Status INTEGER); ";

const QString INIT_WEB_CONTROL_TABLE = 
"CREATE TABLE IF NOT EXISTS %1 ( \
id INTEGER, \
url TEXT, \
updateDT INTEGER); ";

const QString INIT_WEB_LOG_TABLE = 
"CREATE TABLE IF NOT EXISTS %1 ( \
id INTEGER primary key AUTOINCREMENT, \
url TEXT, \
recordDt TEXT, \
userId INTEGER); ";


// static helper methods
QSqlDatabase DBManager::mainDB()
{
    return QSqlDatabase::database(MAIN_DB_FILE_NAME, true);
}

QSqlDatabase DBManager::webControlDB()
{
    return QSqlDatabase::database(WEB_CONTROL_DB_FILE_NAME, true);
}

QSqlDatabase DBManager::webHistoryDB()
{
    return QSqlDatabase::database(WEB_HISTORY_DB_FILE_NAME, true);
}

bool DBManager::init()
{
    QStringList drivers = QSqlDatabase::drivers();

    bool isOK = QSqlDatabase::isDriverAvailable("SQLITECIPHER");
    if (isOK == false)
    {
        //QMessageBox::critical(NULL, tr("error"), tr("Can't load sqLite driver"));
        return false;
    }

    QString dbFileFolder = PathManager::getDBFolder();
    if (dbFileFolder.isEmpty())
    {
        LOG_ERROR << "Can't find database folder: " << qPrintable(dbFileFolder);
        return false;
    }

    bool result = true;

    result = result && initDBImp(dbFileFolder, MAIN_DB_FILE_NAME);

    result = result && initDBImp(dbFileFolder, WEB_CONTROL_DB_FILE_NAME);

    result = result && initDBImp(dbFileFolder, WEB_HISTORY_DB_FILE_NAME);

    result = result && createTableImp(mainDB(), CONFIG_TABLE, 
        INIT_CONFIG_TABLE.arg(CONFIG_TABLE));

    result = result && createTableImp(mainDB(), OFFLINE_SESSION_TABLE, 
        INIT_OFFLINE_SESSION_TABLE.arg(OFFLINE_SESSION_TABLE));

    result = result && createTableImp(webControlDB(), WEB_CONTROL_TABLE, 
        INIT_WEB_CONTROL_TABLE.arg(WEB_CONTROL_TABLE));

    result = result && createTableImp(webHistoryDB(), WEB_LOG_TABLE, 
        INIT_WEB_LOG_TABLE.arg(WEB_LOG_TABLE));
    return result;
}

bool DBManager::initDBImp(const QString& dbFileFolder, const QString& dbFileName )
{
    QString dbFilePath = dbFileFolder + "\\" + dbFileName;

    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITECIPHER", dbFileName);
    db.setDatabaseName(dbFilePath);
    if (!db.open())
    {
        LOG_ERROR << "Can't open the database: " << qPrintable(dbFilePath);
        return false;
    }

    //m_mainDBFile = new QFile(dbFilePath);
    //if (m_mainDBFile == NULL)
    //{
    //    return false;
    //}
    //m_mainDBFile->setPermissions(
    //    QFile::ReadOwner || QFile::WriteOwner || QFile::ExeOwner);
    //m_mainDBFile->open(QIODevice::ReadWrite);

    return true;
}

bool DBManager::createTableImp(QSqlDatabase db, const QString& tableName, 
                               const QString& sqlStatement, const QStringList& paramList)
{
    QStringList tables = db.tables();
    // the table has already been created
    if (tables.contains(tableName, Qt::CaseInsensitive))
    {
        return true;
    }

    QString queryStr = sqlStatement;
    foreach(QString param, paramList)
    {
        queryStr = queryStr.arg(param);
    }

    // the table doesn't exist yet, create a new one
    bool sqlOk = true;
    db.transaction();
    {
        QSqlQuery query(db);
        sqlOk = query.exec(queryStr);
    }

    if (sqlOk == true)
    {
        sqlOk = db.commit();
    }

    if (sqlOk == false)
    {
        db.rollback();
        LOG_ERROR << qPrintable(db.lastError().text());
    }

    return sqlOk;
}
