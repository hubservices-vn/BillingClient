#include "HostFileHelper.h"
#include "WebControl.h"
#include "log.h"
#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QRegExp>
#include <QStringList>
#include <QProcessEnvironment>
#include <QTextStream>
#include "DateTimeUtils.h"

const QString GbBegin_Section = "####GB_BEGIN_1BF274AB90E5##";
const QString GbEnd_Section = "####GB_END_1BF274AB90E5##";

const QString GbBegin_Pattern = "^\\s*####GB_BEGIN_1BF274AB90E5##\\s*$";
const QString GbEnd_Pattern = "^\\s*####GB_END_1BF274AB90E5##\\s*$";

bool HostFileHelper::updateHostsFile( const QMap<quint64, WebControl*> &contentMap )
{
    QString systemFolder = QProcessEnvironment::systemEnvironment().value("SystemRoot");
    QString hostFilePath = systemFolder + "\\System32\\Drivers\\etc\\hosts";
    if (!QFile::exists(hostFilePath))
    {
        QFile newFile(hostFilePath);
        if (!newFile.open(QFile::WriteOnly))
        {
            return false;
        }
    }

    QString tempHostFilePath;
    bool succeeded = _createTempHostFile(tempHostFilePath);
    if (succeeded == false)
    {
        return false;
    }

    QFile hostFile(hostFilePath);
    if (!hostFile.open(QFile::ReadOnly))
    {
        return false;
    }

    QFile tempHostFile(tempHostFilePath);
    if (!tempHostFile.open(QFile::WriteOnly))
    {
        return false;
    }

    QTextStream readStream(&hostFile);
    QTextStream writeStream(&tempHostFile);
    // copy the original hosts settings from src hosts file
    bool reachGbBeginSection = false;
    bool reachGbEndSection = false;
    while(!readStream.atEnd())
    {
        QString line = readStream.readLine().trimmed();
        if (line.isEmpty())
        {
            continue;
        }

        if (! reachGbBeginSection)
        {
            reachGbBeginSection = _isGbBeginSection(line);
        }
        else
        {
            reachGbEndSection = _isGbEndSection(line);
            if (reachGbEndSection)
            {
                reachGbEndSection = false;
                reachGbBeginSection = false;
                continue;
            }
        }

        if (reachGbBeginSection)
        {
            continue;
        }

        //copy it to the temp host file
        writeStream << line << "\r\n";
    }

    // append ours
    // step 1: write section begin
    writeStream << GbBegin_Section << "\r\n";
    writeStream << "####GB_DT: " << DateTimeUtils::dateTimeToString(QDateTime::currentDateTime()) << "##" << "\r\n";
    // step 2: write the blocked websites
    QMapIterator<quint64, WebControl*> iter(contentMap);
    while(iter.hasNext())
    {
        iter.next();
        WebControl* webControl = iter.value();
        if (NULL == webControl)
        {
            continue;
        }
        QString url1 = webControl->url();
        QString url2("https://");
        QString url3("http://");

        if(url1.startsWith("https://", Qt::CaseInsensitive))
        {
            url1 = url1.right(url1.length()-8);
        }
        else if(url1.startsWith("http://", Qt::CaseInsensitive))
        {
            url1 = url1.right(url1.length()-7);
        }

        url2.append(url1);
        url3.append(url1);

        writeStream << "127.0.0.1 " << url1 << "\r\n";
        writeStream << "127.0.0.1 " << url2 << "\r\n";
        writeStream << "127.0.0.1 " << url3 << "\r\n";
    }
    // step 3: write section begin
    writeStream << GbEnd_Section << "\r\n";

    writeStream.flush();
    tempHostFile.close();
    hostFile.close();

    QFile::remove(hostFilePath);
    QFile::copy(tempHostFilePath, hostFilePath);
    QFile::remove(tempHostFilePath);

    return true;
}

bool HostFileHelper::_isGbBeginSection( const QString& line )
{
    QRegExp rx(GbBegin_Pattern);
    return (rx.exactMatch(line));
}

bool HostFileHelper::_isGbEndSection( const QString& line )
{
    QRegExp rx(GbEnd_Pattern);
    return (rx.exactMatch(line));
}

bool HostFileHelper::_createTempHostFile( QString& resultFilePath )
{
    QString tempFolder = QProcessEnvironment::systemEnvironment().value("TEMP");
    QString clientTempFolder = tempFolder + "\\gcbClient";
    QDir tempDir;
    bool succeeded = tempDir.mkpath(clientTempFolder);
    if (!succeeded)
    {
        return false;
    }

    for (int i = 0; i < 200; ++i)
    {
        QString uuidStr = QUuid::createUuid().toString();
        resultFilePath = clientTempFolder + "\\" + uuidStr;
        if (QFile::exists(resultFilePath))
        {
            QFile::remove(resultFilePath);
            continue;
        }
        else
        {
            succeeded = true;
            break;
        }
    }

    if (succeeded == false)
    {
        return false;
    }

    QFile tempFile(resultFilePath);
    if (!tempFile.open(QIODevice::WriteOnly))
    {
        return false;
    }

    return true;
}
