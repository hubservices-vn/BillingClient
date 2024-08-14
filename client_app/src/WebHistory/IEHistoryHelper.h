#ifndef IEHISTORYHELPER_H
#define IEHISTORYHELPER_H


#include <QObject>
#include <QThread>
#include <Qlist>
#include <QScopedPointer>

class WebHistory_Item;
class IEHistoryHelperThread : public QThread
{
    Q_OBJECT
public:
    explicit IEHistoryHelperThread(QObject *parent = 0);

signals:
    void ieHistoryGot(const QList<WebHistory_Item>& historyList);

private: 
    bool __getLatestWebHistory(QList<WebHistory_Item>& historyList);

private:
    void run();
};


class IEHistoryHelper : public QObject
{
    Q_OBJECT

public:
    IEHistoryHelper();

public: // methods
    bool beginFetchHistory();
    bool stopFetchHistory();

signals:
    void getIEHistoryDone(const QList<WebHistory_Item>& historyList);

public slots:
    void ieHistoryGotSlot(const QList<WebHistory_Item>& historyList);

private:
    QScopedPointer<IEHistoryHelperThread> m_thread;
};

#endif // IEHISTORYHELPER_H
