#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QDateTime>

#include "Common/Singleton_T.h"
#include "Common/ConstructorMagic.h"
using SDK::Common::Singleton_T;

class OrderRecord;
class OrderRequestQtModel;
class OrderManager : public QObject, public Singleton_T<OrderManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    const QList<OrderRecord*>& orderList() const
    {
        return m_orderList;
    }

    //OrderRecord* placeOrderRequest( const QMap<int/*prodId*/, int/*amount*/> & orderProducts);

public:
    void procOrderRequestC( OrderRequestQtModel &qtModel );

//    bool orderReply( const QString& uid, bool accepted );
//    OrderRecord* acceptOrder( const QMap<int, int>& order, double totalPrice );
//
//signals:
//public slots:
//
//private:
//    OrderRecord* findOrderByUID(const QString& uid);
//    
private:
    QList<OrderRecord*> m_orderList;
};

#endif // ORDERMANAGER_H
