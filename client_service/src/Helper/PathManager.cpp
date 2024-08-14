#include "PathManager.h"
#include <QDir>
#include "Utils.h"
#include <Aclapi.h>
#pragma comment(lib, "Advapi32.lib")
#include "SysInfoHelper.h"
#include <string>
#include "ClientSvcSettings.h"

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

const QString Log_Folder_Name = "Logs";
const QString DB_Folder_Name = "Database";

namespace
{
    bool _ensureFolderExists(QString &folderPath)
    {
        QDir dir(folderPath);
        if (dir.exists() == false)
        {
            bool ret = dir.mkpath(folderPath);
            //if (ret == true)
            //{
            //    ret = EnsureFileSecurityEx(folderPath.toStdWString());
            //}

            // create the folder failed
            if (ret == false)
            {
                folderPath = "";
                return false;
            }
        }
        folderPath = QDir::toNativeSeparators(folderPath);
        return true;
    }
}

QString PathManager::getAppDataFolder()
{
    static QString s_appDataFolder;
    if (s_appDataFolder.isEmpty() == false)
    {
        return s_appDataFolder;
    }

    s_appDataFolder = SysInfoHelper::allUserAppDataFolder() + "/" + SVC_DATA_FOLDER_NAME;
    _ensureFolderExists(s_appDataFolder);

    return s_appDataFolder;
}

QString PathManager::getLogFolder()
{
    static QString s_logFolder;
    if (s_logFolder.isEmpty() == false)
    {
        return s_logFolder;
    }

    s_logFolder = getAppDataFolder() + "\\" + Log_Folder_Name;
    _ensureFolderExists(s_logFolder);

    return s_logFolder;
}

QString PathManager::getDBFolder()
{
    static QString s_dbFolder;
    if (s_dbFolder.isEmpty() == false)
    {
        return s_dbFolder;
    }

    s_dbFolder = getAppDataFolder() + "\\" + DB_Folder_Name;
    _ensureFolderExists(s_dbFolder);

    return s_dbFolder;
}

QString SysPathHelper::getTmpFolder()
{
    return QProcessEnvironment::systemEnvironment().value("TEMP");
}

QString SysPathHelper::getAllUserProfileFolder()
{
    return QProcessEnvironment::systemEnvironment().value("ALLUSERSPROFILE");
}

QString SysPathHelper::getSystemRootFolder()
{
    return QProcessEnvironment::systemEnvironment().value("SystemRoot");
}
