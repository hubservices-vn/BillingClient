#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QtSql>
#include <QFile>

// database config

const QString MAIN_DB_FILE_NAME = "Main.gbsdat";
const QString WEB_CONTROL_DB_FILE_NAME = "WebControl.gbsdat";
const QString WEB_HISTORY_DB_FILE_NAME = "WebHistory.gbsdat";

const QString DB_FOLDER_NAME = "/Database/";

// tables' names
const QString CONFIG_TABLE = "Configs";
const QString OFFLINE_SESSION_TABLE = "OfflineSessions"; // "offlinesession" table
const QString WEB_CONTROL_TABLE = "WebControls";
const QString WEB_LOG_TABLE = "WebLogs";



class DBManager : public QObject
{
    Q_OBJECT
public:

    // helpers
    static QSqlDatabase mainDB(); // the main DB which stores all the important data
    static QSqlDatabase webControlDB(); // the web control DB (syncs with server)
    static QSqlDatabase webHistoryDB();

    // methods
    static bool init();

private:
    static bool initDBImp(const QString& dbFileFolder, const QString& dbFileName);

    static bool createTableImp(QSqlDatabase db, const QString& tableName, 
        const QString& sqlStatement, const QStringList& paramList = QStringList());

private:
    //QFile* m_mainDBFile;
};

#endif // DBMANAGER_H
