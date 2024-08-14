#ifndef CHROMEHISTORYHELPER_H
#define CHROMEHISTORYHELPER_H

#include <QObject>
#include <QThread>
#include <Qlist>
#include <QScopedPointer>
#include <QDateTime>
#include <QtSql>

class WebHistory_Item;

class ChromeHistoryHelperThread : public QThread
{
    Q_OBJECT

public:
    explicit ChromeHistoryHelperThread(QObject *parent = 0);

signals:
    void chromeHistoryGot(const QList<WebHistory_Item>& historyList);

private: 
    bool __getLatestWebHistory(QList<WebHistory_Item>& historyList);

private:
    void run();

private:
	QString			m_tempHistoryDB;
	QDateTime		m_lastTimeStamp;
	QSqlDatabase	m_database;
};


class ChromeHistoryHelper : public QObject
{
    Q_OBJECT

public:
    ChromeHistoryHelper();

public: // methods
    bool beginFetchHistory();
    bool stopFetchHistory();
	void join();

signals:
    void getChromeHistoryDone(const QList<WebHistory_Item>& historyList);

public slots:
    void chromeHistoryGotSlot(const QList<WebHistory_Item>& historyList);

private:
    QScopedPointer<ChromeHistoryHelperThread> m_thread;
};

#endif // CHROMEHISTORYHELPER_H