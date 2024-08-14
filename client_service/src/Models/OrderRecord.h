#ifndef ORDERRECORD_H
#define ORDERRECORD_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QDateTime>
#include "stdTypes.h"
#include "OrderQtModel.h"

enum OrderType;
enum OrderStatus;
enum PaymentStatus;
//class OrderItem: public QObject
//{
//    Q_OBJECT
//public:
//    explicit OrderItem(int productId, int amount, 
//        OrderStatus orderStatus, PaymentStatus payStatus, const QDateTime& dateTime, 
//        OrderType orderType, const QString& note = QString(), bool enabled = true)
//        : m_productId(productId), m_amount(amount), 
//        m_orderStatus(orderStatus), m_payStatus(payStatus), m_dateTime(dateTime), 
//        m_orderType(orderType), m_note(note), m_enabled(enabled)
//    {
//        m_alreadySentOut = false;
//    }
//
//    int productId() const { return m_productId; }
//    void setProductId(int val) { m_productId = val; }
//
//    int amount() const { return m_amount; }
//    void setAmount(int val) { m_amount = val; }
//
//    OrderStatus orderStatus() const { return m_orderStatus; }
//    void setOrderStatus(OrderStatus val) { m_orderStatus = val; }
//
//    PaymentStatus payStatus() const { return m_payStatus; }
//    void setPayStatus(PaymentStatus val) { m_payStatus = val; }
//
//    QDateTime dateTime() const { return m_dateTime; }
//    void setDateTime(const QDateTime& val) { m_dateTime = val; }
//
//    OrderType orderType() const { return m_orderType; }
//    void setOrderType(OrderType val) { m_orderType = val; }
//
//    QString note() const { return m_note; }
//    void setNote(const QString& val) { m_note = val; }
//
//    bool enabled() const { return m_enabled; }
//    void setEnabled(bool val) { m_enabled = val; }
//
//    bool alreadySentOut() const { return m_alreadySentOut; }
//    void setAlreadySentOut(bool val) { m_alreadySentOut = val; }
//
//private:
//    int m_productId;
//    int m_amount;
//};

class Order_RequestItem;
class OrderRecord : public QObject
{
    Q_OBJECT
public:
    explicit OrderRecord(const QList<Order_RequestItem> &itemList) 
        : m_itemList(itemList)
    {}

    ~OrderRecord() {}

public: // accessors
    QList<Order_RequestItem> itemList() const { return m_itemList; }
    void setItemList(QList<Order_RequestItem> val) { m_itemList = val; }

    QString note() const { return m_note; }
    void setNote(const QString& val) { m_note = val; }

private:
    QList<Order_RequestItem> m_itemList;
    QString m_note;
};

#endif // ORDERRECORD_H
