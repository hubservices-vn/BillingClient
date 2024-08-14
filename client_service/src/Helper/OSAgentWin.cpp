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
#include <QCoreApplication>
#include <LMCONS.H>
#include <QMutexLocker>
#include <QDate>
#include <QTime>
#include <QSysInfo>
#include <QMapIterator>

#include "Utils.h"
#include "log.h"
#include <shlobj.h>
#include "ConfigManager.h"
#include "SafeTermProc.h"
#include <winbase.h>
#include <Strsafe.h>
#include "RestartExplorerHelper.h"
#include "DateTimeUtils.h"
#include <qprocess>
#include <qfile>
#include "CommonSettings.h"
#include "QueryTermAppQtModel.h"

using namespace std;

typedef void  (*PVOIDHOOKFN)(void);
typedef bool  (*PBOOLHOOKFN)(void);


#pragma comment(lib, "Psapi.lib ")
#pragma comment(lib, "Shlwapi.lib ")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "PowrProf.lib")

#define     TASKBAR         L"Shell_TrayWnd"		// Taskbar class name

namespace
{
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

    DWORD FindProcessId(const std::wstring& processName)
    {
        PROCESSENTRY32 processInfo;
        processInfo.dwSize = sizeof(processInfo);

        HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
        if ( processesSnapshot == INVALID_HANDLE_VALUE )
            return 0;

        Process32First(processesSnapshot, &processInfo);
        if ( !processName.compare(processInfo.szExeFile) )
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }

        while ( Process32Next(processesSnapshot, &processInfo) )
        {
            if ( !processName.compare(processInfo.szExeFile) )
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

bool OSAgentWin::killProcessByPid(int pid)
{
    if ( pid == 0 )
        return false;

    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        LOG_DEBUG4 << "Simulated action: killProcessByPid";
        return true;
    }

    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL  bInheritHandle  = FALSE;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, pid);
    if (hProcess == NULL)
    {
        LOG_DEBUG << "Failed to open process by pid: " << pid;
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

    LOG_ERROR << "Failed to kill process by pid: " << pid;
    CloseHandle(hProcess);
    return false;
}

bool OSAgentWin::setLocalTime( const QDate &date, const QTime &time,
                              int timezoneBias, const std::string& timezoneName )
{
    return true;

    //if (CConfigManager::isDisableSecurityRestrictions() == true)
    //{
    //    LOG_DEBUG4 << "Simulated action: setLocalTime";
    //    return true;
    //}

    //BOOL tzResult = TRUE;
    //LOG_DEBUG4 << "Set local time zone. bias: " << timezoneBias << ", timezone name: " << timezoneName;
    //if (date.isValid() == false ||
    //    time.isValid() == false)
    //{
    //    return false;
    //}

    //// set the local time
    //SYSTEMTIME localTime;
    //localTime.wYear = date.year();
    //localTime.wMonth = date.month();
    //localTime.wDay = date.day();
    //localTime.wDayOfWeek = date.dayOfWeek();
    //localTime.wHour = time.hour();
    //localTime.wMinute = time.minute();
    //localTime.wSecond = time.second();
    //localTime.wMilliseconds = time.msec();
    //BOOL timeResult = SetLocalTime(&localTime);
    //LOG_DEBUG4 << "result value SetLocalTime: " << timeResult;

    //return (tzResult == TRUE && timeResult == TRUE);
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

quint32 OSAgentWin::getExplorerPid()
{
    quint32 pid = 0;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hsnap == INVALID_HANDLE_VALUE)
    {
        return pid;
    }

    PROCESSENTRY32 pe32={0};
    pe32.dwSize = sizeof(pe32);
    if (Process32First(hsnap, &pe32))
    {
        do
        {
            if (!_tcsicmp(pe32.szExeFile, L"EXPLORER.EXE"))
            {
                LOG_DEBUG4 << "EXPLORER.EXE pid:" << pe32.th32ProcessID;
                pid = pe32.th32ProcessID;
                break;
            }
        } 
        while (Process32Next(hsnap,&pe32));
    }
    CloseHandle(hsnap);
    return pid;
}
