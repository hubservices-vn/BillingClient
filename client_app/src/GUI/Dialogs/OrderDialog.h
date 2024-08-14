#ifndef ORDERDIALOG_H
#define ORDERDIALOG_H

#include <QDialog>
#include <QMap>
#include "ClientAppSettings.h"


namespace Ui {
    class OrderDialog;
}

class Product;
class QTableWidget;
enum RecordChangeType;
class OrderDialog : public QDialog
{
    Q_OBJECT
public:
    OrderDialog(QWidget* parent = NULL);
    
    QMap<int , int > orderedProducts() const { return m_orderedProducts; }
    double totalPrice() const { return m_totalPrice; }

signals:

public slots:
    void on_placeOrderButton_clicked();
    void on_cancelButton_clicked();
    //void on_orderHistoryButton_clicked();

    void updatePrice(int qty);

    void productChangedSlot(Product* prod, RecordChangeType type);

protected:
    void changeEvent(QEvent *e);

private:
    void retranslateUiEx();
    void initTableWidget();
    void updateProduct(int id, int qty);

    void addProductUI(Product* prod);
    void updateProductUI(Product* prod, int row);
    void removeProductFromUI(Product* product);
    void refreshRowDataCache(int rowCount, int modifiedRow, RecordChangeType changeType);
private:
    Ui::OrderDialog *ui;
    QTableWidget* tableWidget;
    QMap<int /* row */, Product*> m_rowProdMap;
    QMap<int /* prodId */, int /* qty */> m_orderedProducts;
    double m_totalPrice;
};

#endif // ORDERDIALOG_H
