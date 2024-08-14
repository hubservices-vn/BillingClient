#include "HttpSecurityHelper.h"
#include <QDateTime>
#include <QHostInfo>
#include <qt_windows.h>  
#include <wtypes.h>
#include <QtNetwork>

QString HttpSecurityHelper::GetTimeStamp()
{
    return QString::number(QDateTime::currentMSecsSinceEpoch());
}

QString HttpSecurityHelper::GetNetworkName()
{
    return QHostInfo::localDomainName();
}

// Function to retrieve volume names
QString HttpSecurityHelper::GetSysVolSn(const QString& drive)
{
    WCHAR szVolumeName[256];
    WCHAR szFileSystemName[256];
    DWORD dwSerialNumber = 0;     
    DWORD dwMaxFileNameLength=256;     
    DWORD dwFileSystemFlags=0;     
    bool ret = GetVolumeInformation((WCHAR *)drive.utf16(), szVolumeName, 256, &dwSerialNumber, &dwMaxFileNameLength, &dwFileSystemFlags, szFileSystemName, 256);     
    if(!ret)
        return QString("");

    QString volSn = QString::number(dwSerialNumber, 16).toUpper();
    //QString vName = QString::fromUtf16((const ushort *)szVolumeName).trimmed();
    return volSn;
}

QString HttpSecurityHelper::GetMacAddress()
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        if (!netInterface.flags().testFlag(QNetworkInterface::IsRunning))
        {
            continue;
        }

        QString macAddress = netInterface.hardwareAddress().toUpper();

        // VMWare address
        if (macAddress.startsWith("00-50-56"))
        {
            continue;
        }

        // Virtual-box address
        if (macAddress.startsWith("08-00-27"))
        {
            continue;
        }

        // TODO: also need to skip Wine/VirtualPC

        return macAddress;
    }

    return QString();
}

