#ifndef WEBCONTROLMANAGER2_H
#define WEBCONTROLMANAGER2_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include <QThread>
#include "SmartPtrs.h"



class WebControlTask;
class WebControlNetworkPackItem;
class WebControl_ReplyItem;
DECLARE_SHARED_PTR(WebControlDB);

class WebControlMgrThread : public QThread
{
    Q_OBJECT
public:
    WebControlMgrThread(const WebControlDBPtrS& db);
    ~WebControlMgrThread();
    bool init();

    void addTodoList(qint64 latestUpdateDT, const QList<WebControl_ReplyItem>& itemList);

private:
    void run();

private:
    void __handleTask(qint64 latestUpdateDT, const QList<WebControl_ReplyItem>& itemList);

private:
    WebControlDBPtrS m_db;
    uint64_t m_latestUpdateDT;
    QList< QList<WebControl_ReplyItem> > m_itemListList;
};





class WebControlReplyQtModel;
class WebControlManager2 : public QObject, public Singleton_T<WebControlManager2>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

public: // cmd handlers
    void procWebControlReply( const WebControlReplyQtModel &qtModel );

signals:

public slots:
    void serverConnectedSlot();

private:
    QScopedPointer<WebControlMgrThread> m_mgrThread;
    WebControlDBPtrS m_db;
};

#endif // WEBCONTROLMANAGER2_H
