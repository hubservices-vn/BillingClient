#include "OrderDialog.h"
#include <QtGui>
#include <QStringList>
#include "Client.h"
#include <math.h>
#include "OrderHistoryDialog.h"
#include "Utils.h"
#include "ui_OrderDialog.h"
#include "ProductManager.h"
#include "Product.h"
#include "ClientAppSettings.h"
#include "log.h"
#include "OrderQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "ConfigManager.h"

bool productLessThan(Product* p1, Product* p2)
{
    return p1->name().toUpper() < p2->name().toUpper();
}

OrderDialog::OrderDialog(QWidget* parent)
: QDialog(parent), ui(new Ui::OrderDialog)
{
    ui->setupUi(this);
    tableWidget = ui->tableWidget;

    initTableWidget();

    {
        // the server sends product data update
        ProductManager* prodMgr = ProductManager::GetInstance();
        connect(prodMgr, SIGNAL(productChanged(Product*,RecordChangeType)),
            this, SLOT(productChangedSlot(Product*,RecordChangeType)));
    }

    //this->setModal(true);
    //this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //window()->layout()->setSizeConstraint(QLayout::SetFixedSize);
    //this->setWindowFlags(this->windowFlags() & ~Qt::WindowSystemMenuHint);
}

void OrderDialog::changeEvent( QEvent *e )
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        retranslateUiEx();
        break;
    default:
        break;
    }
}

void OrderDialog::retranslateUiEx()
{
    QStringList list;
    list << tr("Product") << tr("Unit") << tr("Price") << tr("Qty") << tr("Total price");
    tableWidget->setHorizontalHeaderLabels(list);
}

void OrderDialog::initTableWidget()
{
    // set headers
    tableWidget->setColumnCount(5);
    QStringList list;
    list << tr("Product") << tr("Unit") << tr("Price") << tr("Qty") << tr("Total price");
    tableWidget->setHorizontalHeaderLabels(list);

    
    tableWidget->horizontalHeader()->setDefaultSectionSize(80);

    tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    tableWidget->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
    
    tableWidget->verticalHeader()->setVisible(false);

    //// hide the id column, it's just used internally to identify a product
    //tableWidget->setColumnHidden(0, true);
   
    QList<Product*> productList = ProductManager::GetInstance()->products();
    int size = productList.size();
    qStableSort(productList.begin(), productList.end(),productLessThan);
    for (int i = 0; i < size;  ++i)
    {
        Product* prod = productList.at(i);
        addProductUI(prod);
    }
}

void OrderDialog::addProductUI(Product* prod)
{
    if (prod == NULL || prod->active() == false || prod->enabled() == false)
    {
        return;
    }

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    m_rowProdMap.insert(row, prod);

    updateProductUI(prod, row);
}

void OrderDialog::updateProductUI( Product* prod, int row )
{
    QString name = prod->name();
    QString unit = prod->unit();
    double unitPrice = prod->price();
    QString unitPriceStr = Utils::formatDisplayMoney(unitPrice);

    tableWidget->setItem(row, 0, new QTableWidgetItem(name));
    tableWidget->setItem(row, 1, new QTableWidgetItem(unit));
    tableWidget->setItem(row, 2, new QTableWidgetItem(unitPriceStr));

    QSpinBox* spinBox = static_cast<QSpinBox*>(tableWidget->cellWidget(row, 3));
    if (spinBox == NULL)
    {
        spinBox = new QSpinBox(this);
        spinBox->setRange(0, 100);
        tableWidget->setCellWidget(row, 3, spinBox);
        connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(updatePrice(int)));
    }
    
    tableWidget->setItem(row, 4, new QTableWidgetItem(QString("")));
}

void OrderDialog::removeProductFromUI( Product* product )
{
    int row = m_rowProdMap.key(product);
    m_rowProdMap.remove(row);

    // after tableWidget->removeRow(), the currentRow and rowCount changes
    int originalChangeRow = row;
    int originalRowCount = tableWidget->rowCount();

    if (row >= 0 &&
        row <= tableWidget->rowCount())
    {
        tableWidget->removeRow(row);
    }

    refreshRowDataCache(originalRowCount, originalChangeRow, RecordChangeType_Del);
}

void OrderDialog::updatePrice(int /*qty*/)
{
    int rowCount = tableWidget->rowCount();
    double totalOrderPrice = 0;
    for(int i = 0; i < rowCount; ++i)
    {
        Product* prod = m_rowProdMap[i];
        QSpinBox* spinBox = static_cast<QSpinBox*>(tableWidget->cellWidget(i, 3));
        int qty = spinBox->value();
        updateProduct(prod->id(), qty);
        double totalItemPrice = prod->price() * qty;
        if (Utils::equals(totalItemPrice, 0))
        {
            tableWidget->setItem(i, 4, new QTableWidgetItem(""));
        }
        else
        {
            tableWidget->setItem(i, 4, new QTableWidgetItem(Utils::formatDisplayMoney(totalItemPrice)));
        }
        totalOrderPrice += prod->price() * qty;
    }
    
    m_totalPrice = totalOrderPrice;
    ui->totalPriceTextLabel->setText(Utils::formatDisplayMoney(totalOrderPrice));
}

void OrderDialog::updateProduct( int id, int qty )
{
    if (m_orderedProducts.contains(id))
    {
        m_orderedProducts.remove(id);
    }
    if (qty > 0)
    {
        m_orderedProducts.insert(id, qty);
    }
}

void OrderDialog::on_placeOrderButton_clicked()
{
    OrderRequestQtModel qtModel(CConfigManager::clientId());
    QMapIterator<int/*prodId*/, int/*qty*/> iter(m_orderedProducts);
    while(iter.hasNext())
    {
        iter.next();
        qtModel.Items_.append(Order_RequestItem(
            iter.key(), iter.value()));
    }
    cc2csSender::send(cc2cs::OrderRequestCommandC(qtModel));

    this->done(1);
}

void OrderDialog::on_cancelButton_clicked()
{
    this->done(0);
}

//void OrderDialog::on_orderHistoryButton_clicked()
//{
    //OrderHistoryDialog* dlg = new OrderHistoryDialog(this);
    //dlg->show();
//}

void OrderDialog::productChangedSlot( Product* prod, RecordChangeType type )
{
    switch(type)
    {
    case RecordChangeType_Add:
        addProductUI(prod);
        break;
    case RecordChangeType_Edit:
        if (m_rowProdMap.values().contains(prod))
        {
            int row = m_rowProdMap.key(prod);
            updateProductUI(prod, row);
        }
        else
        {
            addProductUI(prod);
        }
        break;
    case RecordChangeType_Del:
        removeProductFromUI(prod);
        break;
    }
}

void OrderDialog::refreshRowDataCache(int rowCount, int modifiedRow, RecordChangeType changeType)
{
    // only edit a row, the <row, data> map won't be affected
    if (changeType == RecordChangeType_Edit)
    {
        return;
    }

    // the original operation is added to the last row, so no need to update the other rows
    if (modifiedRow == rowCount - 1)
    {
        return;
    }

    QMap<int, Product*> mid;

    QMapIterator<int, Product*> iter(m_rowProdMap);
    while(iter.hasNext())
    {
        iter.next();
        int row = iter.key();
        Product* data = iter.value();

        if (row <= modifiedRow)
        {
            mid.insert(row, data);
            continue;
        }

        int newRow  = row;
        if (changeType == RecordChangeType_Add)
        {
            newRow += 1;
        }
        else if (changeType == RecordChangeType_Del)
        {
            newRow -= 1;
        }

        mid.insert(newRow, data);
    }

    m_rowProdMap = mid;
}

