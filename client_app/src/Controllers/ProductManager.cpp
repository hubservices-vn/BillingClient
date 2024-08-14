#include "ProductManager.h"
#include "Product.h"
#include "ClientAppSettings.h"
#include "ConfigManager.h"
#include "ProductQtModel.h"
#include "cc2csNetworkManager.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"


bool ProductManager::init()
{
    return true;
}

bool ProductManager::connectSigSlot()
{
    // after server online, send command to product data from server
    {
        cc2csNetworkManager* networkMgr = cc2csNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    return true;
}

bool ProductManager::addOrUpdateProduct
(int id, const QString& name, double price, int stock, 
 const QString& unit, bool active, bool enabled )
{
    // the server has no active products
    if (id == -1)
    {
        foreach(Product* product, m_products)
        {
            product->setEnabled(false);
            emit productChanged(product, RecordChangeType_Del);
        }
    }

    Product* prod = getProductById(id);
    if (prod == NULL)
    {
        prod = new Product(id, name, price, stock, unit, active, enabled);
        m_products.append(prod);

        emit productChanged(prod, RecordChangeType_Add);
        return true;
    }
    else
    {
        prod->setName(name);
        prod->setPrice(price);
        prod->setStock(stock);
        prod->setUnit(unit);
        prod->setActive(active);
        prod->setEnabled(enabled);

        if (active == true && enabled == true)
        {
            emit productChanged(prod, RecordChangeType_Edit);
        }
        else
        {
            emit productChanged(prod, RecordChangeType_Del);
        }
    }
    return true;
}

Product* ProductManager::getProductById( int id )
{
    int size = m_products.size();
    for (int i = 0; i < size; ++i)
    {
        if (m_products.at(i)->id() == id)
        {
            return m_products.at(i);
        }
    }

    return NULL;
}

bool ProductManager::contains( int id )
{
    int size = m_products.size();
    for (int i = 0; i < size; ++i)
    {
        if (m_products.at(i)->id() == id)
        {
            return true;
        }
    }

    return false;
}

void ProductManager::serverConnectedSlot()
{
    ProductRequestQtModel qtModel(CConfigManager::clientId());
    cc2csSender::send(cc2cs::ProductRequestCommandC(qtModel));
}

void ProductManager::procProdReply( const ProductReplyQtModel &qtModel )
{
    foreach(const Product_ReplyItem &qtItem, qtModel.ProdList_)
    {
        addOrUpdateProduct(qtItem.ProductId_, qtItem.Name_, qtItem.Price_, 
            qtItem.Stock_, qtItem.Unit_, qtItem.Active_, qtItem.Enabled_);
    }
}
