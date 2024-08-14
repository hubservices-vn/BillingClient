#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "CcCore/CcSingleton.h"

#include <QObject>
#include <QScopedPointer>
#include <QTimer>
#include <QDateTime>

class LogManager : public QObject, public Singleton_T<LogManager>
{
    Q_OBJECT
public:
    bool init();

signals:
    
public slots:

private:
    bool _clearOldLogFiles();
    bool _initLogFile();

private:
    bool _needChangeFile() const;
    QString _findProperFileName();
private:
    QScopedPointer<QTimer> m_changeLogFileTimer;
    QString m_currentFilePath;
    QDate m_currentDate;
};

#endif // LOGMANAGER_H
