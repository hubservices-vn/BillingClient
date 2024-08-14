#ifndef WEBHISTORYMANAGER_H
#define WEBHISTORYMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>
#include "IEHistoryHelper.h"
#include "ChromeHistoryHelper.h"
#include "FirefoxHistoryHelper.h"

class WebHistory_Item;
class WebHistoryManager : public QObject, public Singleton_T<WebHistoryManager>
{
    Q_OBJECT
public:

public:
    bool init();
    bool connectSigSlot();

signals:

public slots:
    void getWebHistoryDoneSlot(const QList<WebHistory_Item>& historyList);

    void accLoggedInSlot();
    void accLoggedOutSlot(int userId, int accTypeVal);
private:
    QScopedPointer<IEHistoryHelper>			m_ieHelper;
	QScopedPointer<ChromeHistoryHelper>		m_chromeHelper;
	QScopedPointer<FirefoxHistoryHelper>	m_firefoxHelper;
};

#endif // WEBHISTORYMANAGER_H
