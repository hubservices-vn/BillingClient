#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

#include <QObject>

class Product;
enum RecordChangeType;
class ProductReplyQtModel;
class ProductRequestQtModel;
class ProductManager : public QObject, public Singleton_T<ProductManager>
{
    Q_OBJECT
public:
    bool init();
    bool connectSigSlot();

    const QList<Product*>& products() const { return m_products; }

    Product* getProductById(int id);

    bool contains(int id);

public: // server cmd handler
    void procProdReply( const ProductReplyQtModel &qtModel );

public: // client cmd handler
    void procProdRequestC(const ProductRequestQtModel &qtModel);

signals:
    bool productChanged(Product* prod, RecordChangeType type);

public slots:
    void serverConnectedSlot();
    bool addOrUpdateProduct(int prodId, const QString& name, double price, 
        int stock, const QString& unit, bool active, bool enabled);
    
private:
    QList<Product*> m_products;

    bool m_dataFetched;
};

#endif // PRODUCTMANAGER_H
