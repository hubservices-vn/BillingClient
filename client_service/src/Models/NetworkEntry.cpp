#include "NetworkEntry.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostInfo>

NetworkAddress::NetworkAddress() : m_isValid(false)
{

}

NetworkAddress::NetworkAddress(const QString& hostName, const QString& domainName, 
                               const QString& ipv4, const QString& ipv6, const QString& mac, 
                               const QString& ipv4Ext, const QString& networkName, bool isValid)
                               : m_localHostName(hostName), m_localDomainName(domainName),
                               m_ipv4(ipv4), m_ipv6(ipv6), m_mac(mac), 
                               m_ipv4External(ipv4Ext), m_networkName(networkName), m_isValid(isValid)
{
}

//
//QList<NetworkAddress> NetworkAddressManager::getAllEntries()
//{
//    QList<NetworkAddress> result;
//    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
//    {
//        if (!netInterface.flags().testFlag(QNetworkInterface::IsRunning))
//        {
//            continue;
//        }
//
//
//        foreach(QNetworkAddressEntry entry, netInterface.addressEntries())
//        {
//            if (netInterface.hardwareAddress() != "00:00:00:00:00:00" &&
//                entry.ip().toString().contains("."))
//            {
//                QString ipv4 = entry.ip().toString();
//                QString mac = netInterface.hardwareAddress();
//                NetworkAddress addr(ipv4, mac, "");
//                result.append(addr);
//            }
//        }
//    }
//
//    return result;
//}

NetworkAddress NetworkAddressManager::getNetworkAddress( const quint32 ipv4Address )
{
    QString hostName = QHostInfo::localHostName();
    QString domainName = QHostInfo::localDomainName();

    //LOG_DEBUG << "find " << QNetworkInterface::allInterfaces().size() << " network interfaces";
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        if (!netInterface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            //LOG_DEBUG << "netInterface is not running, skip";
            continue;
        }

        foreach(QNetworkAddressEntry entry, netInterface.addressEntries())
        {
            //LOG_DEBUG << "entry addr: " << qPrintable(entry.ip().toString());
            //LOG_DEBUG << "entry protocol: " << entry.ip().protocol();
            //LOG_DEBUG << "entry ipv4: " << entry.ip().toIPv4Address();
            //LOG_DEBUG << "entry mac: " << qPrintable(netInterface.hardwareAddress());
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol &&
                netInterface.hardwareAddress() != "00:00:00:00:00:00" &&
                entry.ip().toIPv4Address() == ipv4Address)
            {
                QString ipv4 = entry.ip().toString();
                QString mac = netInterface.hardwareAddress();
                return NetworkAddress(hostName, domainName, ipv4, "", mac, "", "", true);
            }
        }
    }

    // an "invalid" network address entry
    return NetworkAddress(hostName, domainName, "", "", "", "", "", false);
}

NetworkAddress NetworkAddressManager::getNetworkAddress( const Q_IPV6ADDR& ipv6Address )
{
    QString hostName = QHostInfo::localHostName();
    QString domainName = QHostInfo::localDomainName();

    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        if (!netInterface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            continue;
        }

        foreach(QNetworkAddressEntry entry, netInterface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol &&
                netInterface.hardwareAddress() != "00:00:00:00:00:00")
            {
                bool equals = true;
                Q_IPV6ADDR addr = entry.ip().toIPv6Address();
                for (int i = 0; i < 16; ++i)
                {
                    if (addr[i] != ipv6Address[i])
                    {
                        equals = false;
                        break;
                    }
                }

                if (equals == true)
                {
                    QString ipv6 = entry.ip().toString();
                    QString mac = netInterface.hardwareAddress();
                    return NetworkAddress(hostName, domainName, "", ipv6, mac, "", "", true);
                }
            }
        }
    }

    return NetworkAddress(hostName, domainName, "", "", "", "", "", false);
}

QString NetworkAddressManager::getMacAddress()
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // if the network interface is NOT running, skip
        if ( ! netInterface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            continue;
        }

        // if the ntework interface IS loopback, skip
        if (netInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            continue;
        }

        return netInterface.hardwareAddress();
    }
    return QString();
}
