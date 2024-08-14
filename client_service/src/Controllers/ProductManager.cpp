#include "ProductManager.h"
#include "Product.h"
#include "ClientSvcSettings.h"
#include "cs2ssNetworkManager.h"
#include "cs2ssSender.h"
#include "cs2ssCmds.h"
#include "ConfigManager.h"
#include "ProductQtModel.h"
#include "cs2ccSender.h"
#include "cs2ccCmds.h"

bool ProductManager::init()
{
    m_dataFetched = false;
    return true;
}

bool ProductManager::connectSigSlot()
{
    // after server online, send command to product data from server
    {
        cs2ssNetworkManager* networkMgr = cs2ssNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    // the server replies with product data
    //{
    //    ProductReplyNotifier* prodNoti = ProductReplyNotifier::GetInstance();
    //    connect(prodNoti, SIGNAL(cmdRecved(int,QString,double,int,QString,bool,bool)),
    //        this, SLOT(addOrUpdateProduct(int,QString,double,int,QString,bool,bool)));
    //}

    return true;
}

bool ProductManager::addOrUpdateProduct(
    int id, const QString& name, double price, int stock, 
    const QString& unit, bool active, bool enabled )
{
    m_dataFetched = true;

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
    cs2ssSender::send(cs2ss::ProductRequestCommand(qtModel));
}

void ProductManager::procProdReply( const ProductReplyQtModel &qtModel )
{
    // store in service
    foreach(const Product_ReplyItem &qtItem, qtModel.ProdList_)
    {
        addOrUpdateProduct(qtItem.ProductId_, qtItem.Name_, qtItem.Price_, 
            qtItem.Stock_, qtItem.Unit_, qtItem.Active_, qtItem.Enabled_);
    }

    // forward to client UI APP
    cs2ccSender::send(cs2cc::ProductReplyCommandC(qtModel));
}

void ProductManager::procProdRequestC( const ProductRequestQtModel &qtModel )
{
    ProductReplyQtModel replyModel;
    foreach(Product* prod, m_products)
    {
        if (prod->enabled() == false ||
            prod->active() == false)
        {
            continue;
        }

        replyModel.ProdList_.append(
            Product_ReplyItem(
            prod->id(), prod->name(), prod->price(), 
            prod->unit(), prod->stock(), prod->active(), prod->enabled())
            );
    }

    cs2ccSender::send(cs2cc::ProductReplyCommandC(replyModel));
}
