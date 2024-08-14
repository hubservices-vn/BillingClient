#include "PathManager.h"
#include <QDir>
#include "Utils.h"
#include "SysInfoHelper.h"
#include <string>
#include "log.h"

#include <Aclapi.h>
#pragma comment(lib, "Advapi32.lib")
#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif


const QString Log_Folder_Name = "Logs";
const QString LOCKER_DATA_FOLDER_NAME = "gbClientLocker";


namespace
{
    PSID MyCreateWellKnownSid(WELL_KNOWN_SID_TYPE WellKnownSidType)
    {
        DWORD SidSize = SECURITY_MAX_SID_SIZE;
        PSID psid = LocalAlloc(LMEM_FIXED, SidSize);
        if(psid)
        {
            if(CreateWellKnownSid(WellKnownSidType, NULL, psid, &SidSize)) return psid;
            LocalFree(psid);
        }
        return NULL;
    }

    // only do this for newly-created folders (for performance, as this method will go through all subdirs/files inside the path recursively)
    bool EnsureFileSecurity(LPCTSTR pszFilePath)
    {
        return true;

//        if (!pszFilePath || !*pszFilePath)
//        {
//            return false;
//        }
//
//        BOOL bResult=FALSE;
//        PACL pOldDACL = NULL, pNewDACL = NULL;
//        PSECURITY_DESCRIPTOR pSD = NULL;
//        EXPLICIT_ACCESS ea[3]={0};
//
//#ifdef UNICODE
//        Log_Debug << "Ensure file security: " << Utils::UnicodeToUtf8(pszFilePath);
//#else
//        Log_Debug << "Ensure file security: " << pszFilePath;
//#endif
//
//        LONG dwLastError;
//        if (NT_SUCCESS(dwLastError=GetNamedSecurityInfo((LPTSTR)pszFilePath, SE_FILE_OBJECT, 
//            DACL_SECURITY_INFORMATION,NULL, NULL, &pOldDACL, NULL, &pSD)))
//        {
//            ea[0].grfAccessPermissions = FILE_ALL_ACCESS|FILE_GENERIC_WRITE;
//            ea[0].grfAccessMode = GRANT_ACCESS;
//            ea[0].grfInheritance= SUB_CONTAINERS_AND_OBJECTS_INHERIT;
//            ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
//            memcpy(&ea[1],&ea[0],sizeof(EXPLICIT_ACCESS));
//            memcpy(&ea[2],&ea[0],sizeof(EXPLICIT_ACCESS));
//
//            int sid_count=0;
//            ea[sid_count].Trustee.ptstrName = (PWSTR)MyCreateWellKnownSid(WinWorldSid);//_T("EVERYONE");
//            if (ea[sid_count].Trustee.ptstrName) sid_count++;
//            ea[sid_count].Trustee.ptstrName = (PWSTR)MyCreateWellKnownSid(WinBuiltinAdministratorsSid);//_T("ADMINISTRATORS");
//            if (ea[sid_count].Trustee.ptstrName) sid_count++;
//            ea[sid_count].Trustee.ptstrName = (PWSTR)MyCreateWellKnownSid(WinBuiltinUsersSid);//_T("USERS");
//            if (ea[sid_count].Trustee.ptstrName) sid_count++;
//
//            if (sid_count && NT_SUCCESS(dwLastError=SetEntriesInAcl(sid_count, ea, pOldDACL, &pNewDACL)))
//            {
//                if (NT_SUCCESS(dwLastError=SetNamedSecurityInfo((LPTSTR)pszFilePath, SE_FILE_OBJECT, 
//                    DACL_SECURITY_INFORMATION,NULL, NULL, pNewDACL, NULL)))
//                {
//                    bResult = TRUE;
//                }
//                else
//                {
//                    Log_Debug << "SetNamedSecurityInfo error: " << dwLastError;
//                }
//            }
//            else
//            {
//                Log_Debug << "SetEntriesInAcl error: " << dwLastError;
//            }
//
//            for (int i=0;i<sid_count;i++)
//            {
//                LocalFree(ea[i].Trustee.ptstrName);
//            }
//        } 
//        else
//        {
//            Log_Debug << "GetNamedSecurityInfo error: " << dwLastError;
//        }
//
//        if(pSD != NULL) 
//            LocalFree((HLOCAL) pSD); 
//        if(pNewDACL != NULL) 
//            LocalFree((HLOCAL) pNewDACL); 
//
//        Log_Debug << "Ensure file security done";
//        return (bResult == TRUE);
    }

    // only do this for newly-created folders (for performance, as this method will go through all subdirs/files inside the path recursively)
    bool EnsureFileSecurityEx(const std::wstring &strPath)
    {
        return EnsureFileSecurity(strPath.c_str());
    }

    bool _ensureFolderExists(QString &folderPath)
    {
        QDir dir(folderPath);
        if (dir.exists() == false)
        {
            bool ret = dir.mkpath(folderPath);
            if (ret == true)
            {
                ret = EnsureFileSecurityEx(folderPath.toStdWString());
            }

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

QString PathManager::getAppFolder()
{
    static QString s_appFolder;
    if (s_appFolder.isEmpty() == false)
    {
        return s_appFolder;
    }

    s_appFolder = SysInfoHelper::allUserAppDataFolder() + "/" + LOCKER_DATA_FOLDER_NAME;
    _ensureFolderExists(s_appFolder);

    return s_appFolder;
}

QString PathManager::getLogFolder()
{
    static QString s_logFolder;
    if (s_logFolder.isEmpty() == false)
    {
        return s_logFolder;
    }

    s_logFolder = getAppFolder() + "\\" + Log_Folder_Name;
    _ensureFolderExists(s_logFolder);

    return s_logFolder;
}

