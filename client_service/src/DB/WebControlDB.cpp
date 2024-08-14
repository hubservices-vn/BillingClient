#include "WebControlDB.h"
#include "log.h"
#include <QSqlDatabase>
#include "DBManager.h"
#include "WebControl.h"
#include "DateTimeUtils.h"

WebControlDB::WebControlDB(QObject *parent) : QObject(parent)
{
}

WebControlDB::~WebControlDB()
{
    qDeleteAll(m_webMap.values());
    m_webMap.clear();
}

bool WebControlDB::loadDB()
{
    QSqlDatabase db = DBManager::webControlDB();
    QSqlQuery query(db);

    QString queryStr = QString("select id, url, updateDT \
                               FROM %1 ").arg(WEB_CONTROL_TABLE);

    if (!query.prepare(queryStr) ||
        !query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    while (query.next()) 
    {
        quint64 id = query.value(0).toULongLong();
        QString url =query.value(1).toString();
        QDateTime updateDT = LegacyDateTimeUtils::uint64InSecToDateTime(query.value(2).toLongLong());
        WebControl* control = new WebControl(id, url, updateDT);
        m_webMap[id] = control;
    }

    return true;
}

WebControl* WebControlDB::addControl(quint64 controlId, const QString& url, const QDateTime& updateDT )
{
    QSqlDatabase db = DBManager::webControlDB();
    QSqlQuery query(db);

    QString queryStr = QString("insert into %1 (id, url, updateDT) \
                       values(?, ?, ?)").arg(WEB_CONTROL_TABLE);
    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return NULL;
    }

    query.addBindValue(controlId);
    query.addBindValue(url);
    query.addBindValue(LegacyDateTimeUtils::dateTimeToUInt64InSec(updateDT));

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return NULL;
    }

    WebControl* control = new WebControl(controlId, url, updateDT);
    m_webMap[controlId] = control;
    
    return control;
}

bool WebControlDB::modifyControl( quint64 controlId, const QString& url, const QDateTime& updateDT )
{
    WebControl* control = getControlById(controlId);
    if (control == NULL)
    {
        return false;
    }

    return _modifyControlImp(control, url, updateDT);
}

bool WebControlDB::removeControl( quint64 controlId )
{
    WebControl* control = getControlById(controlId);
    if (control == NULL)
    {
        return false;
    }

    return _removeControlImp(control);
}

WebControl* WebControlDB::getControlById( quint64 controlId )
{
    if (m_webMap.contains(controlId))
    {
        return m_webMap[controlId];
    }

    return NULL;
}

bool WebControlDB::_modifyControlImp( WebControl* control, const QString& url, const QDateTime& updateDT )
{
    QSqlDatabase db = DBManager::webControlDB();
    QSqlQuery query(db);

    QString queryStr = QString("UPDATE %1 \
                               SET url=?, updateDT=? \
                               WHERE id=?")
                               .arg(WEB_CONTROL_TABLE);
    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(url);
    query.addBindValue(LegacyDateTimeUtils::dateTimeToUInt64InSec(updateDT));
    query.addBindValue(control->controlId());

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    control->setUrl(url);

    return true;
}

bool WebControlDB::_removeControlImp( WebControl* control )
{
    QSqlDatabase db = DBManager::webControlDB();
    QSqlQuery query(db);

    QString queryStr = QString("delete from %1 where id=?").arg(WEB_CONTROL_TABLE);
    if (!query.prepare(queryStr))
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    query.addBindValue(control->controlId());

    if(!query.exec())
    {
        LOG_ERROR << qPrintable(query.lastError().text());
        return false;
    }

    delete m_webMap.take(control->controlId());

    return true;
}
