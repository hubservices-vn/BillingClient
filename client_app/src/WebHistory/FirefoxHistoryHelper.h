#ifndef FIREFOXHISTORYHELPER_H
#define FIREFOXHISTORYHELPER_H

#include <QObject>
#include <QThread>
#include <Qlist>
#include <QScopedPointer>
#include <QDateTime>
#include <QtSql>

class WebHistory_Item;

class FirefoxHistoryHelperThread : public QThread
{
    Q_OBJECT

public:
    explicit FirefoxHistoryHelperThread(QObject *parent = 0);

signals:
    void firefoxHistoryGot(const QList<WebHistory_Item>& historyList);

private: 
    bool __getLatestWebHistory(QList<WebHistory_Item>& historyList);

private:
    void run();

private:
	QString			m_tempHistoryDB;
	QDateTime		m_lastTimeStamp;
	QSqlDatabase	m_database;
};


class FirefoxHistoryHelper : public QObject
{
    Q_OBJECT

public:
    FirefoxHistoryHelper();

public: // methods
    bool beginFetchHistory();
    bool stopFetchHistory();
	void join();

signals:
    void getFirefoxHistoryDone(const QList<WebHistory_Item>& historyList);

public slots:
    void firefoxHistoryGotSlot(const QList<WebHistory_Item>& historyList);

private:
    QScopedPointer<FirefoxHistoryHelperThread> m_thread;
};

#endif // FIREFOXHISTORYHELPER_H