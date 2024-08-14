//#include "OrderHistoryDialog.h"
//#include "Client.h"
//#include "OrderManager.h"
//#include <QMap>
//
//
//OrderHistoryDialog::OrderHistoryDialog( Client* client, QWidget *parent /* = 0*/ )
//: QDialog(parent), m_client(client)
//{
//    setupUi(this);
//
//    initTreeWidget();
//}
//
//void OrderHistoryDialog::initTreeWidget()
//{
//    // init headers
//    QStringList headers;
//    headers << tr("Name") << tr("Amount");
//    treeWidget->setHeaderLabels(headers);
//    treeWidget->setHeaderHidden(true);
//    treeWidget->header()->setVisible(false);
//    treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
//
//    treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//
//    //const QList<OrderRecord*>& orderList = m_client->orderManager()->orderList();
//    //int size = orderList.size();
//    //for (int i = 0; i < size; ++i)
//    //{
//    //    OrderRecord* record = orderList.at(i);
//    //    addOrderToTree(record);
//    //}
//}
//
//void OrderHistoryDialog::addOrderToTree(OrderRecord* record)
//{
//    QString timeStr = record->time().toString("dd/MM hh:mm");
//    QString status;
//    if (!record->replied())
//    {
//        status = tr("Pending");
//    }
//    else
//    {
//        if (record->isFromServer())
//        {
//            status = tr("From Server");
//        }
//        else
//        {
//            if (record->accepted())
//            {
//                status = tr("Delivered");
//            }
//            else
//            {
//                status = tr("Rejected");
//            }
//        }
//    }
//
//    QString description = QString("%1 (%2)").arg(timeStr).arg(status);
//    QString totalPriceStr = tr("$") + QString::number(record->totalPrice(), 'f', 2);
//
//    QTreeWidgetItem* topItem = addTopLevelItem(description, totalPriceStr);
//
//    const QMap<int, int>& prodMap = record->productMap();
//    QMapIterator<int, int> iter(prodMap);
//    while(iter.hasNext())
//    {
//        iter.next();
//        int prodId = iter.key();
//        int quantity = iter.value();
//        //Product* prod = m_client->orderManager()->getProductById(prodId);
//        //if (prod == NULL)
//        //{
//        //    continue;
//        //}
//
//        //QString prodName = prod->name();
//        //addSubItem(topItem, prodName, quantity);
//    }
//    topItem->setExpanded(true);
//}
//
//QTreeWidgetItem* OrderHistoryDialog::addTopLevelItem( const QString& description, const QString& priceStr )
//{
//    QStringList list;
//    list << description << priceStr;
//    QTreeWidgetItem* item = new QTreeWidgetItem(list);
//    treeWidget->addTopLevelItem(item);
//    return item;
//}
//
//void OrderHistoryDialog::addSubItem( QTreeWidgetItem* parent, QString name, int amount )
//{
//    QStringList list;
//    list << name << QString("%1").arg(amount);
//    QTreeWidgetItem* item = new QTreeWidgetItem(list);
//    parent->addChild(item);
//}
//
//void OrderHistoryDialog::on_closeButton_clicked()
//{
//    this->done(1);
//}
