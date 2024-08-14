#include "ordermanager.h"
#include <QUuid>
#include "OrderRecord.h"
#include "ConfigManager.h"
#include "OrderQtModel.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"


bool OrderManager::init()
{
    return true;
}

bool OrderManager::connectSigSlot()
{
    return true;
}


void OrderManager::procOrderRequestC( OrderRequestQtModel &qtModel )
{
    // store to memory
    OrderRecord* record = new OrderRecord(qtModel.Items_);
    m_orderList.append(record);

    // forward command to server
    qtModel.ClientId_ = CConfigManager::clientId();
    cs2ssSender::send(cs2ss::OrderRequestCommand(qtModel));
}

//OrderRecord* OrderManager::acceptOrder( const QMap<int, int>& orderProducts, double totalPrice )
//{
//    OrderRecord* record = new OrderRecord(orderProducts, totalPrice);
//    record->setReplied(true);
//    record->setAccepted(true);
//    record->setIsFromServer(true);
//    m_orderList.append(record);
//    return record;
//}
//
//bool OrderManager::orderReply( const QString& uid, bool accepted )
//{
//    OrderRecord* record = findOrderByUID(uid);
//    if (record == NULL)
//    {
//        return false;
//    }
//    record->setReplied(true);
//    record->setAccepted(accepted);
//    record->setIsFromServer(false);
//
//    return true;
//}
//
//OrderRecord* OrderManager::findOrderByUID( const QString& uid )
//{
//    int size = m_orderList.size();
//    for (int i = 0; i < size; ++i)
//    {
//        if (QString::compare(m_orderList.at(i)->uid(), uid, Qt::CaseInsensitive) == 0)
//        {
//            return m_orderList.at(i);
//        }
//    }
//
//    return NULL;
//}
