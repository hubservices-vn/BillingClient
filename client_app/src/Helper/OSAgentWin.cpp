#include "stdafx.h"
#include "OSAgentWin.h"
#include <Windows.h>
#include <PowrProf.h>
// for windows 7 sdk compatibility
#define PSAPI_VERSION 1
#include <Psapi.h>
#include <Shlwapi.h>
#include <Tlhelp32.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>
#include <LMCONS.H>
#include <winbase.h>
#include <Strsafe.h>

#include <QCoreApplication>
#include <QMutexLocker>
#include <QDate>
#include <QTime>
#include <QSysInfo>
#include <QMapIterator>
#include <qprocess>
#include <qfile>

#include "Utils.h"
#include "log.h"
#include <shlobj.h>
#include "ConfigManager.h"
#include "SafeTermProc.h"
#include "DateTimeUtils.h"
#include "CommonSettings.h"
#include "QueryTermAppQtModel.h"
#include <boost/algorithm/string.hpp>

using namespace std;

typedef void  (*PVOIDHOOKFN)(void);
typedef bool  (*PBOOLHOOKFN)(void);


#pragma comment(lib, "Psapi.lib ")
#pragma comment(lib, "Shlwapi.lib ")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "PowrProf.lib")

#define     PROGRAM_MANAGER L"Program Manager"  // Program manager window name
#define     TASKBAR         L"Shell_TrayWnd"        // Taskbar class name
#define     BUTTON_HND      L"BUTTON"       // Taskbar class name
#define     ID_STARTBUTTON  0x130               // Start button ID
#define     ID_TRAY         0x12F               // System tray ID
#define     ID_CLOCK        0x12F               // System clock ID




namespace
{
	struct FILEDATA_LANGCODEPAGE
	{
		WORD language;
		WORD codePage;
	};

    bool ShutdownComputerAction(BOOL rebootAfterShutdown = FALSE)
    {
        HANDLE hToken; 
        TOKEN_PRIVILEGES tkp; 

        // Get a token for this process. 
        if (!OpenProcessToken(GetCurrentProcess(), 
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
        {
            LOG_ERROR << "Open process token failed. code: " << ::GetLastError();
            return false; 
        }

        // Get the LUID for the shutdown privilege. 
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
            &tkp.Privileges[0].Luid); 

        tkp.PrivilegeCount = 1;  // one privilege to set    
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

        // Get the shutdown privilege for this process. 
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
        CloseHandle(hToken);

        if (GetLastError() != ERROR_SUCCESS) 
        {
            LOG_ERROR << "AdjustTokenPrivileges failed. code: " << ::GetLastError();
            return false; 
        }

        // Shut down /restart the system and force all applications to close.
        if (!InitiateSystemShutdownEx(
            NULL, // pc name, local
            NULL, // msg, null
            0, // timeout
            TRUE, // force apps to close
            rebootAfterShutdown, 
            SHTDN_REASON_MAJOR_APPLICATION | SHTDN_REASON_MINOR_MAINTENANCE | SHTDN_REASON_FLAG_PLANNED
            ))
        {
            LOG_ERROR << "InitiateSystemShutdownEx(shutdown) failed. code: " << ::GetLastError();
            return false; 
        }

        LOG_DEBUG << "Shutdown succeeded";
        return true;
    }

    bool ComputerAction(UINT flag)
    {
        HANDLE hToken; 
        TOKEN_PRIVILEGES tkp; 

        // Get a token for this process. 
        if (!OpenProcessToken(GetCurrentProcess(), 
            TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
        {
            LOG_ERROR << "Open process token failed. code: " << ::GetLastError();
            return false; 
        }

        // Get the LUID for the shutdown privilege. 
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
            &tkp.Privileges[0].Luid); 

        tkp.PrivilegeCount = 1;  // one privilege to set    
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

        // Get the shutdown privilege for this process. 
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
        CloseHandle(hToken);

        if (GetLastError() != ERROR_SUCCESS) 
        {
            LOG_ERROR << "AdjustTokenPrivileges failed. code: " << ::GetLastError();
            return false; 
        }

        // Shut down /restart the system and force all applications to close. 
        if (!ExitWindowsEx(flag, 
            SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
            SHTDN_REASON_MINOR_UPGRADE |
            SHTDN_REASON_FLAG_PLANNED)) 
        {
            LOG_ERROR << "ExitWindowsEx(shutdown) failed. code: " << ::GetLastError();
            return false; 
        }

        LOG_DEBUG << "Shutdown succeeded";
        return true;
    }

    void enableDebugPrivileges()
    {
        HANDLE hcurrent=GetCurrentProcess();
        HANDLE hToken;
        BOOL bret=OpenProcessToken(hcurrent,40,&hToken);
        LUID luid;
        bret=LookupPrivilegeValue(NULL,SE_DEBUG_NAME, &luid);
        TOKEN_PRIVILEGES NewState,PreviousState;
        DWORD ReturnLength;
        NewState.PrivilegeCount =1;
        NewState.Privileges[0].Luid =luid;
        NewState.Privileges[0].Attributes=2;
        AdjustTokenPrivileges(hToken,FALSE,&NewState,28,&PreviousState,&ReturnLength);
    }

    QString GetProcessUsername(HANDLE hProcess, BOOL bIncDomain) 
    {
        QString result; // the return value
        static char sname[300];
        HANDLE tok = 0;
        TOKEN_USER *ptu;
        DWORD nlen, dlen;
        char name[300], dom[300], tubuf[300];
        int iUse;

        //open the processes token
        if (!OpenProcessToken(hProcess,TOKEN_QUERY,&tok)) goto ert;

        //get the SID of the token
        ptu = (TOKEN_USER*)tubuf;
        if (!GetTokenInformation(tok,(TOKEN_INFORMATION_CLASS)1,ptu,300,&nlen)) goto ert;

        //get the account/domain name of the SID
        dlen = 300;
        nlen = 300;
        if (!LookupAccountSidA(0, ptu->User.Sid, name, &nlen, dom, &dlen, (PSID_NAME_USE)&iUse)) goto ert;

        //copy info to our static buffer
        if (dlen && bIncDomain) {
            strcpy(sname,dom);
            strcat(sname,"");
            strcat(sname,name);
        } else {
            strcpy(sname,name);
        }
        //set our return variable
        result = QString(sname);

    ert:
        if (tok) CloseHandle(tok);
        return result;
    }

    QString getProcessUserNameByPid (DWORD processID)
    {
        // Get a handle to the process.
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );

        QString result = GetProcessUsername(hProcess, FALSE);

        // Release the handle to the process.
        CloseHandle( hProcess );

        return result;
    }

	QString getProcessDescriptionByPid(DWORD processID)
	{
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

		DWORD errorCode = 0;

		do
		{
			hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);

			errorCode = GetLastError();
		}
		while ((hModuleSnap == INVALID_HANDLE_VALUE) && (errorCode == ERROR_BAD_LENGTH));

        if( hModuleSnap == INVALID_HANDLE_VALUE )
        {
            LOG_ERROR << "CreateToolhelp32Snapshot (of module) failed.";

            return QString();
        }

		MODULEENTRY32 me32;
		me32.dwSize = sizeof(MODULEENTRY32);

		if(!Module32First(hModuleSnap, &me32))
		{
			LOG_ERROR << "Failed to execute the Module32First.";

			CloseHandle(hModuleSnap);

			return QString();
		}

		DWORD fileVersionInfoSize = GetFileVersionInfoSize(me32.szExePath, NULL);

		QByteArray fileVersionInfoDataBlock(fileVersionInfoSize, '\0');

		QString result;

		if (GetFileVersionInfo(me32.szExePath, 0, fileVersionInfoSize, reinterpret_cast<void*>(fileVersionInfoDataBlock.data())))
		{
			UINT count = 0;

			FILEDATA_LANGCODEPAGE* langCodePage = NULL;

			VerQueryValue(fileVersionInfoDataBlock.data(), TEXT("\\VarFileInfo\\Translation"),
				reinterpret_cast<void**>(&langCodePage), &count);

			if (count == 0)
			{
				return QString();
			}

			TCHAR requestString[50];

			StringCchPrintf(requestString, 50,
				TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"), langCodePage[0].language, langCodePage[0].codePage);

			TCHAR* fileDesc = NULL;

			VerQueryValue(fileVersionInfoDataBlock.data(), requestString, reinterpret_cast<void**>(&fileDesc), &count);

			result = QString::fromWCharArray(fileDesc);
		}

		CloseHandle(hModuleSnap);

		return result;
	}

    // get the user name which is currently logged into OS
    QString getUserNameLoggedin()
    {
        QString userName;

    #if defined(UNICODE)
        TCHAR winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
        DWORD winUserNameSize = sizeof(winUserName);
        GetUserName( winUserName, &winUserNameSize );
        userName = QString::fromWCharArray(winUserName);
    #else
        char winUserName[UNLEN + 1]; // UNLEN is defined in LMCONS.H
        DWORD winUserNameSize = sizeof(winUserName);
        GetUserNameA( winUserName, &winUserNameSize );
        userName = QString::fromLocal8Bit( winUserName );
    #endif

        return userName;
    }

    // some programs may have multi processes mapped to same name
    QList<quint32> FindAllPidsForApp(const std::wstring& processName)
    {
        QList<quint32> resultList;

        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if ( processesSnapshot == INVALID_HANDLE_VALUE )
        {
            return resultList;
        }

        Process32First(processesSnapshot, &processInfo);
        if (boost::iequals(processName, processInfo.szExeFile))
        {
            resultList.append(processInfo.th32ProcessID);
        }

        while ( Process32Next(processesSnapshot, &processInfo) )
        {
            if (boost::iequals(processName, processInfo.szExeFile))
            {
                resultList.append(processInfo.th32ProcessID);
            }
        }

        CloseHandle(processesSnapshot);
        return resultList;
    }

    DWORD FindProcessId(const std::wstring& processName)
    {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if ( processesSnapshot == INVALID_HANDLE_VALUE )
            return 0;

        Process32First(processesSnapshot, &processInfo);
        if (boost::iequals(processName, processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }

        while ( Process32Next(processesSnapshot, &processInfo) )
        {
            if (boost::iequals(processName, processInfo.szExeFile))
            {
                CloseHandle(processesSnapshot);
                return processInfo.th32ProcessID;
            }
        }

        CloseHandle(processesSnapshot);
        return 0;
    }

}


OSAgentWin::~OSAgentWin()
{
}

bool OSAgentWin::isProcessRunning( const std::wstring & processName)
{
    DWORD pid = FindProcessId(processName);
    return (pid != 0);
}

bool OSAgentWin::isProcessRunning( const std::wstring & processName, int & pid )
{
    pid = FindProcessId(processName);
    return (pid != 0);
}

bool OSAgentWin::killProcessByName( const std::wstring & processName )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: killProcessByName";
        return true;
    }

    DWORD processID = FindProcessId(processName);

    if ( processID == 0 )
    {
        LOG_ERROR << "Failed to find procId for app: " << Utils::UnicodeToUtf8(processName);
        return false;
    }

    return killProcessByPid(processID);
}

bool OSAgentWin::killProcessByPid(int processID)
{
    if ( processID == 0 )
        return false;

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: killProcessByPid";
        return true;
    }

    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle  = FALSE;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, processID);
    if (hProcess == NULL)
    {
        LOG_ERROR << "Failed to open process by pid: " << processID;
        return false;
    }

    // step 1: try directly terminate the process
    BOOL result = ::TerminateProcess(hProcess, 0);
    if (result == TRUE)
    {
        CloseHandle(hProcess);
        return true;
    }

    // step 2: try to close the application via safe term proc method
    result = SafeTerminateProcess(hProcess, 0);
    if (result == TRUE)
    {
        CloseHandle(hProcess);
        return true;
    }

    LOG_ERROR << "Failed to kill process by pid: " << processID;
    CloseHandle(hProcess);
    return false;
}

bool OSAgentWin::killProcessByWnd( int handle )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: killProcessByWnd";
        return true;
    }

    HWND hwnd = (HWND)handle;
    ::PostMessage(hwnd, WM_CLOSE, 0, 0);

    DWORD dwProcessId=0;
    DWORD dwThreadid = GetWindowThreadProcessId(hwnd,&dwProcessId);
    killProcessByPid(dwProcessId);
    return true;
}

bool OSAgentWin::killAllRunningApps(const QStringList& excludeAppList)
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: killAllRunningApps";
        return true;
    }

    QList<QueryTermApp_AppItem> procList;
    getRunningProcessNameListEx(procList, excludeAppList);

    {
        LOG_DEBUG << "get proc to del: " << procList.size() << " processes";
    }

    foreach(const QueryTermApp_AppItem &item, procList)
    {
        ProcessType procType = (ProcessType)item.Type_;
        switch(procType)
        {
        case ProcessType_Process:
            {
                int pid = item.Pid_;
                QString procName = item.Name_;
                if (excludeAppList.contains(procName, Qt::CaseInsensitive) == true)
                {
                    continue;
                }
                LOG_DEBUG4 << "Begin to kill program " << qPrintable(procName) << " with pid: " << pid;
                killProcessByPid(pid);
            }
            break;
        case ProcessType_Window:
            {
                int handle = item.Handle_;
                QString wndTitle = item.Name_;
                LOG_DEBUG4 << "Begin to kill program. Title " << qPrintable(wndTitle) << " with handle: " << handle;
                killProcessByWnd(handle);
            }
            break;
        }
    }

    return true;
}

BOOL IsTskbarWindow(HWND hwnd)
{
    // Start at the root owner
    HWND root = GetAncestor(hwnd, GA_ROOTOWNER);

    // See if we are the last active visible popup
    HWND hwndTry;
    while ((hwndTry = GetLastActivePopup(root)) != hwndTry) 
    {
        if (IsWindowVisible(hwndTry)) 
        {
            break;
        }
        root = hwndTry;
    }
    return root == hwnd;
}

static QMap<HWND, QString> s_windowMap;
static QList<quint32> s_excludePidList;
BOOL CALLBACK GetProcListProc(HWND hWnd, LPARAM lParam) 
{
    if (!hWnd)
    {
        return TRUE;        // Not a window
    }

    if (hWnd == ::GetShellWindow())
    {
        return TRUE;
    }

    DWORD windowProcessId = 0;
    ::GetWindowThreadProcessId(hWnd, &windowProcessId);

    if (windowProcessId == QCoreApplication::applicationPid() ||
        windowProcessId == GetCurrentProcessId()) // if the window belongs to our program, skip them
    {
        return TRUE;
    }

    // belongs to exclude list
    if (s_excludePidList.contains(windowProcessId))
    {
        return TRUE;
    }

    TCHAR class_name[80];
    TCHAR title[80];
    GetClassName(hWnd,class_name, sizeof(class_name) / sizeof(TCHAR));
    GetWindowText(hWnd,title,sizeof(title) / sizeof(TCHAR));

#ifdef UNICODE
    QString classNameStr = QString::fromWCharArray(class_name);
    QString titleStr = QString::fromWCharArray(title);
#else
    QString classNameStr = QString::fromAscii(class_name);
    QString titleStr = QString::fromWCharArray(title);
#endif

    if (titleStr.isEmpty())
    {
        return TRUE;
    }

    if (QString::compare(classNameStr, QString("Shell_TrayWnd")) == 0 ||        // windows taskbar
        QString::compare(classNameStr, QString("DV2ControlHost")) == 0 ||       // windows startmenu
        QString::compare(classNameStr, QString("Button")) == 0 ||               // windows startmenu button
        QString::compare(classNameStr, QString("MsgrIMEWindowClass")) == 0 ||   // Live messenger's notifybox
        QString::compare(classNameStr, QString("SysShadow")) == 0 ||            // Live messenger's shadow-hack
        QString::compare(classNameStr, QString("Progman")) == 0)                // program manager
    {
        return TRUE;
    }

    if(!IsTskbarWindow(hWnd)) // not in alt-tab list (most likely it doesn't appear in taskbar)
    {
        return TRUE;
    }

    s_windowMap[hWnd] = titleStr;

    return TRUE;
}

bool OSAgentWin::getRunningProcessNameListEx(
    /*out*/QList<QueryTermApp_AppItem>& procList, 
    /*in*/ const QStringList& excludeAppList)
{
    QMap<int/*pid*/, int/*useless*/> pidMap;

    qint64 selfPid = QCoreApplication::applicationPid();
    QString currentOSUserName = getUserNameLoggedin();
    //LOG_DEBUG4 << "current os user name: " << qPrintable(currentOSUserName);

    //{
    //    LOG_DEBUG4 << "selfpid: " << selfPid << " ,current os username: " << qPrintable(currentOSUserName);
    //    LOG_DEBUG4 << "begin to enable debug privilege";
    //}

    // enable the privileges so that we can look up to the process's user name information
    enableDebugPrivileges();

    //{
    //    LOG_DEBUG4 << "enable debug privilege done";
    //    LOG_DEBUG4 << "begin enumerate process list";
    //}

    QStringList excludeProcOSUserNameList;
    excludeProcOSUserNameList 
        << "SYSTEM" 
        << "LOCAL SERVICE"
        << "NETWORK SERVICE";

    // get by enumerating process list
    {
        // Take a snapshot of all processes in the system.
        HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
        if( hProcessSnap == INVALID_HANDLE_VALUE )
        {
            LOG_ERROR << "CreateToolhelp32Snapshot (of processes) failed.";
            return false;
        }

        PROCESSENTRY32 pe32;
        // Set the size of the structure before using it.
        pe32.dwSize = sizeof( PROCESSENTRY32 );

        // Retrieve information about the first process,
        // and exit if unsuccessful
        if( !Process32First( hProcessSnap, &pe32 ) )
        {
            LOG_ERROR << "Failed to execute the Process32First.";
            CloseHandle( hProcessSnap );          // clean the snapshot object
            return false;
        }

        // Now walk the snapshot of processes, and
        // display information about each process in turn
        do
        {
            int pid = pe32.th32ProcessID;
            //LOG_DEBUG4 << "check process with pid: " << pid;
            if (pid == selfPid)
            {
                //LOG_DEBUG4 << "is self, skip";
                continue;
            }

            QString procUserName = getProcessUserNameByPid(pid);
            //LOG_DEBUG4 << "process os user name: " << qPrintable(procUserName);
            if (procUserName.isEmpty())
            {
                continue;
            }
            // since this is run in client service, so most likely this process is running
            // under "SYSTEM" account, so if we kill all processes with same account as client service
            // we'll kill the system processes by mistakenly
            if (excludeProcOSUserNameList.contains(procUserName, Qt::CaseInsensitive))
            {
                continue;
            }

            QString procName = QString::fromWCharArray(pe32.szExeFile);
            //LOG_DEBUG4 << "proc name to kill: " << qPrintable(procName);
            if (!procName.isEmpty())
            {
                if (excludeAppList.contains(procName, Qt::CaseInsensitive))
                {
                    continue;
                }
                if (pid > 0)
                {
                    pidMap[pid] = procList.size();
                }

				// Get the process description
				QString procDesc = getProcessDescriptionByPid(pid);

                procList.append(QueryTermApp_AppItem(ProcessType_Process, procName, -1, pid, procDesc));
            }
        } while( Process32Next( hProcessSnap, &pe32 ) );

        CloseHandle( hProcessSnap );
    }

    //{
    //    LOG_DEBUG4 << "enumerate process list done";
    //    LOG_DEBUG4 << "begin enumerate windows list";
    //}

    // get by enumerating window list
    s_excludePidList.clear();
    foreach(const QString &excludeApp, excludeAppList)
    {
        QList<quint32> pidList = FindAllPidsForApp(excludeApp.toStdWString());
        s_excludePidList.append(pidList);
    }

    s_windowMap.clear();
    {
        ::EnumWindows(GetProcListProc, NULL);

        QMapIterator<HWND, QString> iter(s_windowMap);
        while(iter.hasNext())
        {
            iter.next();
            HWND hWnd = iter.key();
            QString title = iter.value();
            //LOG_DEBUG4 << "process by wnd: HWND: " << hWnd << " ,title: " << qPrintable(title);

            DWORD wndPid = 0;
            ::GetWindowThreadProcessId(hWnd, &wndPid);
            //LOG_DEBUG4 << "pid of the window: " << wndPid;
			QMap<int, int>::const_iterator iter = pidMap.find(wndPid);
            if (iter != pidMap.end())
            {
				procList[pidMap[wndPid]].Title_ = title;
                //LOG_DEBUG4 << "this pid has already been included when scanning processes, skip";
                continue;
            }

            QString procUserName = getProcessUserNameByPid(wndPid);
            //LOG_DEBUG4 << "process os user name: " << qPrintable(procUserName);
            //if (procUserName.isEmpty())
            //{
            //    continue;
            //}

            // since this is run in client service, so most likely this process is running
            // under "SYSTEM" account, so if we kill all processes with same account as client service
            // we'll kill the system processes by mistakenly
            if (excludeProcOSUserNameList.contains(procUserName, Qt::CaseInsensitive))
            {
                //LOG_DEBUG4 << "user name is in exclude list, skip";
                continue;
            }

            if (wndPid > 0)
            {
                pidMap[wndPid] = 1;
            }
            procList.append(QueryTermApp_AppItem(ProcessType_Window, title, (int)hWnd, -1, title));
        }
    }

    {
        //LOG_DEBUG4 << "enumerate windows list done";
    }

    return true;
}

// it doesn't work well with 64bit windows 7 (should we compile is at x64?)
QString OSAgentWin::getProcessNameByPid (DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("");

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID );

    // Get the process name.
    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }
    else
    {
        LOG_DEBUG4 << "Failed to get name of process " << processID << ", error code: " << GetLastError();
    }

    QString result;
#ifdef UNICODE
    result = QString::fromUtf16((ushort*)szProcessName);
#else
    result = QString::fromLocal8Bit(szProcessName);
#endif

    // Release the handle to the process.
    CloseHandle( hProcess );

    return result;
}

bool OSAgentWin::shutdownComputer()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: shutdownComputer";
        return true;
    }

    LOG_DEBUG4 << "Begin to shutdown computer";
    return ShutdownComputerAction(FALSE/*restart*/);
}

bool OSAgentWin::restartComputer()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: restartComputer";
        return true;
    }

    LOG_DEBUG4 << "Begin to shutdown computer";
    return ShutdownComputerAction(TRUE/*restart*/);
}

bool OSAgentWin::standbyComputer()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: standbyComputer";
        return true;
    }

    BOOL result = SetSuspendState(TRUE, FALSE, FALSE);
    if (result == FALSE)
    {
        LOG_WARN << "standby pc failed. error code: " << GetLastError();
    }

    return (result == TRUE);
}

bool OSAgentWin::logoffComputer()
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: logoffComputer";
        return true;
    }

    return ComputerAction(EWX_LOGOFF | EWX_FORCE);
}

