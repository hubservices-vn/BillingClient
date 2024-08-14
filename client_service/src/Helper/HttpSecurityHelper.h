#ifndef HTTPSECURITYHELPER_H
#define HTTPSECURITYHELPER_H

#include <QObject>
#include <QMap>
#include <QString>

class HttpSecurityHelper
{
public:
    static QString GetTimeStamp();

    static QString GetNetworkName();

    static QString GetMacAddress();

    static QString GetSysVolSn(const QString& drive = "C:\\");
};

#endif // HTTPSECURITYHELPER_H
