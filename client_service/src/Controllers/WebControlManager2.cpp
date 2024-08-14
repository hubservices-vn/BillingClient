#include "WebControlManager2.h"
#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QRegExp>
#include <QStringList>
#include <QProcessEnvironment>
#include <QTextStream>
#include "log.h"
#include "WebControl.h"
#include "ConfigManager.h"
#include "WebControlDB.h"
#include "HostFileHelper.h"
#include <QMutex>
#include "WebControlQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "cs2ssNetworkManager.h"

static QMutex s_itemListList_mutex;

WebControlMgrThread::WebControlMgrThread(const WebControlDBPtrS& db)
: m_db(db), m_latestUpdateDT(0)
{
}

WebControlMgrThread::~WebControlMgrThread()
{
}

bool WebControlMgrThread::init()
{
    return true;
}

void WebControlMgrThread::addTodoList( qint64 latestUpdateDT, const QList<WebControl_ReplyItem>& itemList )
{
    QMutexLocker locker(&s_itemListList_mutex);
    {
        m_latestUpdateDT = max(m_latestUpdateDT, latestUpdateDT);
        m_itemListList.append(itemList);

        int kk = 3;
    }
}

void WebControlMgrThread::run()
{
    while(!m_itemListList.isEmpty())
    {
        QList<WebControl_ReplyItem> itemList;
        {
            // only lock when fecthing it from list
            QMutexLocker locker(&s_itemListList_mutex);
            {
                itemList = m_itemListList.takeAt(0);
            }
        }

        __handleTask(m_latestUpdateDT, itemList);
    }
}

void WebControlMgrThread::__handleTask( qint64 latestUpdateDT, const QList<WebControl_ReplyItem>& itemList )
{
    LOG_DEBUG4 << "Begin to update hosts file";
    // update the "last update web control dateTime" value in Config DB table
    CConfigManager::setLastUpdateWebCtrlDT(latestUpdateDT);

    // update DB
    foreach(const WebControl_ReplyItem& netItem, itemList)
    {
        int ctrlId = netItem.WebControlId_;
        bool active = (netItem.Active_ && netItem.Enabled_) == true;
        // either update or delete
        if (m_db->m_webMap.contains(ctrlId))
        {
            WebControl* webControl = m_db->m_webMap[ctrlId];
            if (webControl == NULL)
            {
                m_db->addControl(ctrlId, netItem.Url_, netItem.Timestamp_);
            }
            else
            {
                QString oldUrl = webControl->url();
                if (active)
                {
                    // if the url doesn't change, no need to update
                    if (QString::compare(oldUrl, netItem.Url_, Qt::CaseInsensitive) == 0)
                    {
                        continue;
                    }
                    m_db->modifyControl(ctrlId, netItem.Url_, netItem.Timestamp_);
                }
                else
                {
                    m_db->removeControl(ctrlId);
                }
            }
        }
        // add active ones
        else
        {
            if (active)
            {
                m_db->addControl(ctrlId, netItem.Url_, netItem.Timestamp_);
            }
        }
    }

    // update hosts file
    if (false == CConfigManager::isDeveloperEnv())
    {
        HostFileHelper::GetInstance()->updateHostsFile(m_db->m_webMap);
    }

    LOG_DEBUG4 << "Update hosts file done";
}

bool WebControlManager2::init()
{
    // init & load DB
    bool succeeded = true;
    m_db = WebControlDBPtrS(new WebControlDB);
    succeeded = succeeded && m_db->loadDB();

    // init thread
    m_mgrThread.reset(new WebControlMgrThread(m_db));
    succeeded = succeeded && m_mgrThread->init();

    return succeeded;
}

bool WebControlManager2::connectSigSlot()
{
    // after server online, send command to get latest web control data from server
    {
        cs2ssNetworkManager* networkMgr = cs2ssNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    return true;
}

void WebControlManager2::serverConnectedSlot()
{
    qint64 lastUpdateDTVal = CConfigManager::lastUpdateWebCtrlDTVal();
    WebControlRequestQtModel qtModel(CConfigManager::clientId(), lastUpdateDTVal);
    cs2ssSender::send(cs2ss::WebControlRequestCommand(qtModel));
}

void WebControlManager2::procWebControlReply( const WebControlReplyQtModel &qtModel )
{
    m_mgrThread->addTodoList(qtModel.UpdateUtcTimestamp_, qtModel.Items_);

    if (!m_mgrThread->isRunning())
    {
        m_mgrThread->start();
    }
}

