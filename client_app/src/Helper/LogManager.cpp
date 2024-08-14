#include "stdafx.h"
#include "LogManager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "PathManager.h"
#include "log.h"
#include <stdio.h>
#include <QtDebug>

const int Change_Log_Interval = 86400000; // 86400s = 24h = 1 day



const int MAX_LOG_FILE_DAYS = 7; // keep log files for MAX_LOG_FILE_DAYS days


bool LogManager::init()
{
    bool ret = _initLogFile();
    //ret = ret && _clearOldLogFiles();

    QtConcurrent::run(this, &LogManager::_clearOldLogFiles);

    return ret;
}

bool LogManager::_initLogFile()
{
    QString logFileFolder = PathManager::getLogFolder();
    QDir dir(logFileFolder);
    if (!dir.exists(logFileFolder))
    {
        dir.mkdir(logFileFolder);
    }

    QString fileName = _findProperFileName();
    m_currentFilePath = logFileFolder + "\\" + fileName;

    FILE* pFile = fopen(m_currentFilePath.toAscii().data(), "a");
    if (pFile == NULL)
    {
        return false;
    }
    Output2FILE::SetStream(pFile);

#ifdef NDEBUG
    FILELog::SetReportingLevel(LogLevel_Debug4);
    //FILELog::SetReportingLevel(logINFO);
#else
    FILELog::SetReportingLevel(LogLevel_Debug4);
#endif

    return true;
}

bool LogManager::_needChangeFile() const
{
    return false;
}

QString LogManager::_findProperFileName()
{
    QString fileName = QString("gbClientApp-%1.log").arg(QDate::currentDate().toString("yyyyMMdd"));
    return fileName;
//    QString baseDateStr;
//    if (QDate::currentDate() != m_currentDate)
//    {
//    }
}

bool LogManager::_clearOldLogFiles()
{
    QString logFileFolder = PathManager::getLogFolder();
    QDir dir(logFileFolder);
    if (!dir.exists(logFileFolder))
    {
        dir.mkdir(logFileFolder);
        return true;
    }

    QStringList nameFilters;
    nameFilters << "*.log";
    foreach(QFileInfo info, dir.entryInfoList(nameFilters, QDir::Files))
    {
        if (info.isFile() == false)
        {
            continue;
        }

        QDateTime dt = info.lastModified();
        int days = dt.daysTo(QDateTime::currentDateTime());
        if (days > MAX_LOG_FILE_DAYS)
        {
            QFile::remove(info.absoluteFilePath());
        }
    }

    return true;
}
