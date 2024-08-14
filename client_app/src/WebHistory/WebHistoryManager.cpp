#include "WebHistoryManager.h"
#include "UserAccountManager.h"
#include "WebHistoryQtModel.h"
#include "ConfigManager.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "log.h"
#include "CommonSettings.h"


bool WebHistoryManager::init()
{
    qRegisterMetaType<WebHistory_Item >("WebHistory_Item");
    qRegisterMetaType<QList<WebHistory_Item> >("WebHistoryItemList");

    m_ieHelper.reset(new IEHistoryHelper);
	m_chromeHelper.reset(new ChromeHistoryHelper);
	m_firefoxHelper.reset(new FirefoxHistoryHelper);

    return true;
}

bool WebHistoryManager::connectSigSlot()
{
    // user login/logout event
    {
        UserAccountManager* mgr = UserAccountManager::GetInstance();
        connect(mgr, SIGNAL(accLoggedIn()),
            this, SLOT(accLoggedInSlot()));
        connect(mgr, SIGNAL(accLoggedOut(int,int)),
            this, SLOT(accLoggedOutSlot(int,int)));
    }

    // get ie web history done
    if (m_ieHelper)
    {
        connect(m_ieHelper.data(), SIGNAL(getIEHistoryDone(QList<WebHistory_Item>)),
            this, SLOT(getWebHistoryDoneSlot(QList<WebHistory_Item>)));
    }

	// get chrome web history done
	if (m_chromeHelper)
	{
		connect(m_chromeHelper.data(), SIGNAL(getChromeHistoryDone(QList<WebHistory_Item>)),
            this, SLOT(getWebHistoryDoneSlot(QList<WebHistory_Item>)));
	}

	// get firefox web history done
	if (m_firefoxHelper)
	{
		connect(m_firefoxHelper.data(), SIGNAL(getFirefoxHistoryDone(QList<WebHistory_Item>)),
            this, SLOT(getWebHistoryDoneSlot(QList<WebHistory_Item>)));
	}

    return true;
}

void WebHistoryManager::getWebHistoryDoneSlot( const QList<WebHistory_Item>& historyList )
{
    if (historyList.isEmpty())
    {
        return;
    }

    if (UserAccountManager::currentUserType() == UserGroup_LocalSuperAdmin)
    {
        //LOG_DEBUG4 << "webhistory fetch done. current user is local admin, skip";
        return;
    }

    //LOG_DEBUG4 << "webhistory records got. begin to send to server";
    WebHistoryReportQtModel qtModel(
        CConfigManager::clientId(), 
        UserAccountManager::currentUserId(), 
        UserAccountManager::currentUserType());
    qtModel.HistItems_.append(historyList);
    cc2csSender::send(cc2cs::WebHistoryReportCommandC(qtModel));
}

void WebHistoryManager::accLoggedInSlot()
{
    //LOG_DEBUG << "account logged in. begin to get history";
    if (m_ieHelper.isNull())
    {
        m_ieHelper.reset(new IEHistoryHelper);
    }

	if (m_chromeHelper.isNull())
    {
        m_chromeHelper.reset(new ChromeHistoryHelper);
    }

	if (m_firefoxHelper.isNull())
    {
        m_firefoxHelper.reset(new FirefoxHistoryHelper);
    }

    m_ieHelper->beginFetchHistory();
	m_chromeHelper->beginFetchHistory();
	m_firefoxHelper->beginFetchHistory();
}

void WebHistoryManager::accLoggedOutSlot( int userId, int accTypeVal )
{
    //LOG_DEBUG << "account logged out. stop getting history";
    if (m_ieHelper.isNull())
    {
        return;
    }

    m_ieHelper->stopFetchHistory();

	if (m_chromeHelper.isNull())
    {
        return;
    }

	m_chromeHelper->stopFetchHistory();

	if (m_firefoxHelper.isNull())
    {
        return;
    }

	m_firefoxHelper->stopFetchHistory();
}
