#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>

class Product : public QObject
{
    Q_OBJECT
public:
    explicit Product(int id, const QString& name, double price, int stock, const QString& unit, bool active, bool enabled) : 
    m_id(id), m_name(name), m_price(price), m_stock(stock), m_unit(unit), m_active(active), m_enabled(enabled)
    {
    }

    // accessors
    int id() const { return m_id; }
    void setId(int val) { m_id = val; }

    QString name() const { return m_name; }
    void setName(const QString& val) { m_name = val; }

    double price() const { return m_price; }
    void setPrice(double val) { m_price = val; }

    QString unit() const { return m_unit; }
    void setUnit(const QString& val) { m_unit = val; }

    int stock() const { return m_stock; }
    void setStock(int val) { m_stock = val; }

    bool active() const { return m_active; }
    void setActive(bool val) { m_active = val; }

    bool enabled() const { return m_enabled; }
    void setEnabled(bool val) { m_enabled = val; }

private:
    int     m_id;
    QString m_name;
    double  m_price;
    QString m_unit; // for Sprite, the unit is "bottle", for coffee, the unit is normally "cup", etc
    int     m_stock; // not used in this version
    bool m_active;
    bool m_enabled;
};

#endif // PRODUCT_H
