//#ifndef ORDERHISTORYDIALOG_H
//#define ORDERHISTORYDIALOG_H
//
//#include <QDialog>
//#include "ui_OrderHistoryDialog.h"
//#include <QString>
//
//class Client;
//class OrderRecord;
//class QTreeWidgetItem;
//
//class OrderHistoryDialog : public QDialog, public Ui::OrderHistoryDialog
//{
//    Q_OBJECT
//public:
//    explicit OrderHistoryDialog(QWidget *parent = 0);
//
//signals:
//
//public slots:
//    void on_closeButton_clicked();
//
//private:
//    void initTreeWidget();
//    void addOrderToTree(OrderRecord* record);
//    QTreeWidgetItem* addTopLevelItem( const QString& description, const QString& priceStr );
//    void addSubItem( QTreeWidgetItem* parent, QString name, int amount );
//
//private:
//    Client* m_client;
//};
//
//#endif // ORDERHISTORYDIALOG_H
