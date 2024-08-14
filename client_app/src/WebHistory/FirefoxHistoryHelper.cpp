#include "FirefoxHistoryHelper.h"

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <QtSql>
#include <QDir>
#include <windows.h>
#include <Knownfolders.h>
#include <Shlobj.h>
#include "WebHistoryQtModel.h"

const int QUERY_INTERVAL = 30000; // ms // 1 min, 1 * 60 * 1000

// =======================================
// = FirefoxHistoryHelperThread
// =======================================
FirefoxHistoryHelperThread::FirefoxHistoryHelperThread( QObject *parent /* = 0*/ )
	: m_tempHistoryDB("FirefoxHistory.data")
{
}

bool FirefoxHistoryHelperThread::__getLatestWebHistory(QList<WebHistory_Item>& historyList)
{
	// Firefox browser history is stored in the following places
	// XP    -- C:\Documents and Settings\<username>\Application Data\Mozilla\Firefox\Profiles\<profile folder>\places.sqlite
	// VISTA -- C:\Users\<user>\AppData\Roaming\Mozilla\Firefox\Profiles\<profile folder>\places.sqlite
	// http://www.forensicswiki.org/wiki/Mozilla_Firefox_3_History_File_Format
	wchar_t path[MAX_PATH];

	if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, 0, NULL, path)))
	{
		return false;
	}

	QString profileDirPath = QString::fromWCharArray(path) + "\\Mozilla\\Firefox\\Profiles\\";

	// Iterate over every profile folder
	QDir profileDir(profileDirPath);

	profileDir.setFilter(QDir::Dirs);

	QStringList entries = profileDir.entryList();

	// Store the current time before we copy the DB. Copying can take time.
	// If we store the time only after copying or reading the DB, there will be a time gap and lost of record
	QDateTime currentTime = QDateTime::currentDateTimeUtc();

	for (QStringList::ConstIterator entry = entries.begin(); entry != entries.end(); ++entry)
	{
		QString profileFolderName = *entry;

		if(profileFolderName != "." && profileFolderName != "..")
		{
			// For each profile folder, history is stored in places.sqlite
			// Process the next profile folder if the current folder fails
			QString historyDataFile = profileDirPath + profileFolderName + "\\places.sqlite";

			// Copy the DB
			if (!CopyFile(historyDataFile.utf16(), m_tempHistoryDB.utf16(), false))
			{
				continue;
			}

			// Open the DB
			m_database.setDatabaseName(m_tempHistoryDB);

			if (!m_database.open())
			{
				qDebug() << m_database.lastError().text();

				continue;
			}

			// Query the URL and timestamp
			// Timestamp is stored in number of microseconds since midnight UTC of 1 January 1970 (unix epoch)
			QSqlQuery query("SELECT moz_places.url, moz_historyvisits.visit_date\
							 FROM moz_places, moz_historyvisits\
							 WHERE moz_places.id = moz_historyvisits.place_id", m_database);

			while (query.next())
			{
				WebHistory_Item data;

				data.Url_ = query.value(0).toString();

				data.Timestamp_ = QDateTime(
					QDate(1970, 1, 1), QTime(0, 0, 0), Qt::UTC).addMSecs(query.value(1).toULongLong() / 1000);

				if (data.Timestamp_ >= m_lastTimeStamp)
				{
					qDebug() << data.itemToString().c_str() << "\n";

					historyList.append(data);
				}
			}

			m_database.close();

			DeleteFile(m_tempHistoryDB.utf16());
		}
	}

	m_lastTimeStamp = currentTime;

	return true;
}

void FirefoxHistoryHelperThread::run()
{
	m_lastTimeStamp = QDateTime::currentDateTimeUtc();

	const QString connectionName = "connection.firefox.history";

	m_database = QSqlDatabase::addDatabase("QSQLITE", connectionName);

	while(true)
    {
        //LOG_DEBUG << "begin reading history data: ";

        QList<WebHistory_Item> historyList;

        bool succeeded = __getLatestWebHistory(historyList);

        if (succeeded)
        {
            //LOG_DEBUG << "read web history succeeded. web history count: " << historyList.size();
            emit firefoxHistoryGot(historyList);
        }
        else
        {
            //LOG_WARN << "get webhistory failed";
        }

		QThread::msleep(QUERY_INTERVAL);
    }

	QSqlDatabase::removeDatabase(connectionName);
}

// =======================================
// = FirefoxHistoryHelper
// =======================================
FirefoxHistoryHelper::FirefoxHistoryHelper()
{
	m_thread.reset(new FirefoxHistoryHelperThread);

    connect(m_thread.data(), SIGNAL(firefoxHistoryGot(QList<WebHistory_Item>)),
        this, SLOT(firefoxHistoryGotSlot(QList<WebHistory_Item>)));
}

bool FirefoxHistoryHelper::beginFetchHistory()
{
	if (m_thread->isRunning())
    {
        return true;
    }

    m_thread->start();

    return true;
}

bool FirefoxHistoryHelper::stopFetchHistory()
{
	if (m_thread->isRunning())
    {
        m_thread->quit();
    }

    return true;
}

void FirefoxHistoryHelper::join()
{
	m_thread->wait();
}

void FirefoxHistoryHelper::firefoxHistoryGotSlot(const QList<WebHistory_Item>& historyList)
{
	emit getFirefoxHistoryDone(historyList);
}