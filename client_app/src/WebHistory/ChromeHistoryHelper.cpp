#include "ChromeHistoryHelper.h"

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Knownfolders.h>
#include <Shlobj.h>
#include "WebHistoryQtModel.h"

const int QUERY_INTERVAL = 30000; // ms // 1 min, 1 * 60 * 1000

// =======================================
// = ChromeHistoryHelperThread
// =======================================
ChromeHistoryHelperThread::ChromeHistoryHelperThread( QObject *parent /* = 0*/ )
	: m_tempHistoryDB("ChromeHistory.data")
{
}

bool ChromeHistoryHelperThread::__getLatestWebHistory(QList<WebHistory_Item>& historyList)
{
	// Chrome browser history is stored in the following places
	// XP    -- C:\Documents and Settings\USER\Local Settings\Application Data\Google\Chrome\User Data\Default
	// VISTA -- C:\users\USER\Local Settings\Application Data\Google\Chrome\User Data\Default
	// http://www.forensicswiki.org/wiki/Google_Chrome
	wchar_t path[MAX_PATH];

	if (FAILED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, 0, NULL, path)))
	{
		return false;
	}

	const QString historyDataFile = QString::fromWCharArray(path) + "\\Google\\Chrome\\User Data\\Default\\History";

	// Store the current time before we copy the DB. Copying can take time.
	// If we store the time only after copying or reading the DB, there will be a time gap and lost of record
	QDateTime currentTime = QDateTime::currentDateTimeUtc();

	// Copy the DB
	if (!CopyFile(historyDataFile.utf16(), m_tempHistoryDB.utf16(), false))
	{
		return false;
	}
	
	// Open DB
	m_database.setDatabaseName(m_tempHistoryDB);

	if (!m_database.open())
	{
		qDebug() << m_database.lastError().text();

		return false;
	}

	// Query the URL and timestamp
	// Timestamp is stored in number of microseconds since midnight UTC of 1 January 1601
	QSqlQuery query("SELECT url, last_visit_time\
					FROM urls", m_database);

	while (query.next())
	{
		WebHistory_Item data;

		data.Url_ = query.value(0).toString();

		data.Timestamp_ = QDateTime(
			QDate(1601, 1, 1), QTime(0, 0, 0), Qt::UTC).addMSecs(query.value(1).toULongLong() / 1000);

		if (data.Timestamp_ >= m_lastTimeStamp)
		{
			historyList.append(data);
		}
	}

	m_database.close();

	m_lastTimeStamp = currentTime;

	return (!!DeleteFile(m_tempHistoryDB.utf16()));
}

void ChromeHistoryHelperThread::run()
{
	m_lastTimeStamp = QDateTime::currentDateTimeUtc();

	const QString connectionName = "connection.chrome.history";

	m_database = QSqlDatabase::addDatabase("QSQLITE", connectionName);

	while(true)
    {
        //LOG_DEBUG << "begin reading history data: ";

        QList<WebHistory_Item> historyList;

        bool succeeded = __getLatestWebHistory(historyList);

        if (succeeded)
        {
            //LOG_DEBUG << "read web history succeeded. web history count: " << historyList.size();
            emit chromeHistoryGot(historyList);
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
// = ChromeHistoryHelper
// =======================================
ChromeHistoryHelper::ChromeHistoryHelper()
{
	m_thread.reset(new ChromeHistoryHelperThread);

    connect(m_thread.data(), SIGNAL(chromeHistoryGot(QList<WebHistory_Item>)),
        this, SLOT(chromeHistoryGotSlot(QList<WebHistory_Item>)));
}

bool ChromeHistoryHelper::beginFetchHistory()
{
	if (m_thread->isRunning())
    {
        return true;
    }

    m_thread->start();

    return true;
}

bool ChromeHistoryHelper::stopFetchHistory()
{
	if (m_thread->isRunning())
    {
        m_thread->quit();
    }

    return true;
}

void ChromeHistoryHelper::join()
{
	m_thread->wait();
}

void ChromeHistoryHelper::chromeHistoryGotSlot(const QList<WebHistory_Item>& historyList)
{
	emit getChromeHistoryDone(historyList);
}