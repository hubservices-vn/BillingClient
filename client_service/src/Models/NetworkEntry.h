#ifndef NETWORKENTRY_H
#define NETWORKENTRY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QIPv6Address>

class Server;

class NetworkAddress
{
public:
    NetworkAddress();

    NetworkAddress(const QString& hostName, const QString& domainName, const QString& ipv4, const QString& ipv6, 
        const QString& mac, const QString& ipv4Ext, const QString& networkName, bool isValid);

    ~NetworkAddress() {}

public:
    QString LocalHostName() const { return m_localHostName; }
    void setLocalHostName(const QString& val) { m_localHostName = val; }

    QString LocalDomainName() const { return m_localDomainName; }
    void setLocalDomainName(const QString& val) { m_localDomainName = val; }

    QString ipv4() const { return m_ipv4; }
    void setIpv4(const QString& val) { m_ipv4 = val; }
    
    QString ipv6() const { return m_ipv6; }
    void setIpv6(const QString& val) { m_ipv6 = val; }
    
    QString mac() const { return m_mac; }
    void setMac(const QString& val) { m_mac = val; }
    
    QString ipv4External() const { return m_ipv4External; }
    void setIpv4External(const QString& val) { m_ipv4External = val; }
    
    QString networkName() const { return m_networkName; }
    void setNetworkName(const QString& val) { m_networkName = val; }

    bool isValid() const { return m_isValid; }

private:
    QString m_localHostName;
    QString m_localDomainName;
    QString m_ipv4;
    QString m_ipv6;
    
    QString m_mac;
    QString m_ipv4External;
    QString m_networkName;
    
    bool m_isValid;
    
};

class NetworkAddressManager : public QObject
{
    Q_OBJECT

public:
    //static QList<NetworkAddress> getAllEntries();
    static NetworkAddress getNetworkAddress(const quint32 ipv4Address);
    static NetworkAddress getNetworkAddress(const Q_IPV6ADDR& ipv6Address);

    // get macaddress of one of the network interfaces returned by QNetworkInterface::allInterfaces()
    static QString getMacAddress();
signals:

public slots:

private:
};

#endif // NETWORKENTRY_H
