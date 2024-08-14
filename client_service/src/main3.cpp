#include <QCoreApplication>
#include <QString>

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QLocale>
#include <QResource>
#include <QRegExp>
#include <windows.h>
#include "CcCore/CcCoreAppSingleton.h"
#include "LogManager.h"
#include "log.h"
#include "ConfigManager.h"
#include "Utils.h"
#include "Client.h"
#include <exception>
#include "ClientSvcSettings.h"
#include "DBManager.h"
#include "CommonSettings.h"
#include "DateTimeUtils.h"
#include "OSAgentWinQt.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "crashrpt.h"
#include "StringUtils.h"
#include "Settings/ClientCommonSettings.h"
#include <winsvc.h>

#include "Helper/MSFireWall.h"
#include "Helper/MSFireWall2.h"
#include <boost/functional/hash/hash.hpp>
#include <boost/lexical_cast.hpp>
#include "SysInfoHelper.h"
#include "ClientEventDllUpdateManager.h"
#include "ClientEventManager.h"
#include "WorkThreadManager.h"

#pragma comment(lib, "Advapi32.lib ")
#ifdef NDEBUG
// to hide the console window in release environment
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// Service Name
#define SVCGROUP TEXT("GarenaGroup")
#define Q_SVCGROUP "GarenaGroup"
#define SVCNAME  TEXT("gbClientService")

SERVICE_STATUS          gSvcStatus;
SERVICE_STATUS_HANDLE   gSvcStatusHandle;
VOID SvcInstall(bool isAlsoRun);
VOID SvcUnInstall(void);
VOID SvcStart();
VOID WINAPI SvcCtrlHandler(DWORD);
VOID WINAPI SvcMain(int, char**);
VOID ReportSvcStatus(int, int, int);
VOID SvcInit(int, char**);
VOID SvcAuthen(int, char**);
void _addFireWallException();

extern const char* UIVERSION;
extern const char* VERSIONSTRING;
extern const char* BRANCH;
const QString APP_SINGLETON_NAME = "Global\\{673CC6CA-D71F-4298-841E-45EE958068C0}";
const QString CLIENT_UPDATER = "clientupdater";

void _addFireWallException()
{  
    //1) get the clientService program path
    TCHAR szTemp[MAX_PATH+1]={0};
    GetModuleFileName(NULL,szTemp,MAX_PATH);
    std::wstring clientServicePath = szTemp;

    //2) get program folder
    TCHAR* psz=_tcsrchr(szTemp,'\\');
    if (psz)
    {
        *psz=0;
        psz++;
    }
    std::wstring folderPath = szTemp;

    //3. get client service name & add to firewall exception list
    if (psz && *psz)
    {
        MSFirewallSetting2 firewall2;
        TCHAR* psz2=_tcsrchr(psz,'.');
        if (psz2) *psz2=0;

        std::wstring clientServiceName = psz;
        // new rules are name based
        size_t path_hash=boost::hash<std::wstring>()(clientServicePath);
        HRESULT hr = firewall2.AddApp((clientServiceName + boost::lexical_cast<std::wstring>(path_hash)).c_str(),clientServicePath.c_str());

        if (FAILED(hr))
        {
            // old rules are file based
            MSFirewallSetting firewall;
            hr = firewall.AddApp(clientServiceName.c_str(),clientServicePath.c_str());
        }
    }

    //4.add auto update program to firewall exception list
    {
        QString programName = CLIENT_UPDATER;
        QString programExe = "\\" + programName + ".exe";

        std::wstring clientUpdaterName = programName.toStdWString();
        std::wstring clientUpdaterPath = folderPath.append(programExe.toStdWString());

        MSFirewallSetting2 firewall2;
        // new rules are name based
        size_t path_hash=boost::hash<std::wstring>()(clientUpdaterPath);
        HRESULT hr = firewall2.AddApp((clientUpdaterName + boost::lexical_cast<std::wstring>(path_hash)).c_str(),clientUpdaterPath.c_str());

        if (FAILED(hr))
        {// old rules are file based
            MSFirewallSetting firewall;
            hr = firewall.AddApp(clientUpdaterName.c_str(),clientUpdaterPath.c_str());
        }
    }
}

bool iniBeforeAuthen()
{
    //1. add firewall setting
    _addFireWallException();

    //2. init config manager and load settings
    bool configInited = CConfigManager::init();
    if (configInited == false)
    {
        return false;
    }
    // load settings stored in registry
    CConfigManager::loadRegData();
    return true;
}

int _tryRun()
{
    //////////////////////////////////////////////////////////////////////////
    // add firewall setting
    _addFireWallException();

    // init & start the worker threads
    WorkThreadManager::GetInstance()->initThreads();
    WorkThreadManager::GetInstance()->startThreads();

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // init config manager and load settings
    bool configInited = CConfigManager::init();
    if (configInited == false)
    {
        return 1;
    }
    // load settings stored in registry
    CConfigManager::loadRegData();

    // load database
    bool result = DBManager::init();
    if (result == false)
    {
        LOG_ERROR << "Failed to init database";
    }

    // load settings stored in DB
    CConfigManager::loadDBData();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    result = Client::GetInstance()->init();
    if (result == false)
    {
        LOG_ERROR << "Failed to init client";
        return 1;
    }
    Client::GetInstance()->connectSigSlot();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // begin the connection now
    result = Client::GetInstance()->startUp();
    if (!result)
    {
        LOG_ERROR << "failed to startup";
        return 1;
    }

    LOG_DEBUG << "Trying to start client code dll update" ;
    ClientEventDllUpdateManager* dllUpdateManager = ClientEventDllUpdateManager::GetInstance();
    dllUpdateManager->init();
    dllUpdateManager->connectSigSlot();
    // run client code event: startup now
    ClientEventManager* clientEventManager = ClientEventManager::GetInstance();
    clientEventManager->init();
    LOG_DEBUG << "Connect sig slot" ;
    clientEventManager->connectSigSlot();

    return 0;
}

int debugStart(int argc, char *argv[])
{
    QCoreApplication coreApp(argc, argv);
    coreApp.setOrganizationName(G_COMPANY);
    coreApp.setApplicationVersion(UIVERSION);

    _tryRun();

    // Perform work until service stops.
    return coreApp.exec();
}

int svcLaunch(int argc, char *argv[])
{
    if(argc >= 2)
    {
        if((strcmp(argv[1], "--install") == 0)||(strcmp(argv[1], "-i") == 0))
        {
            LOG_DEBUG4 << "begin to install service";
            SvcInstall(/*alsoRun*/false);
            return 0;
        }
        else if((strcmp(argv[1], "--uninstall") == 0)||(strcmp(argv[1], "-u") == 0))
        {
            LOG_DEBUG4 << "begin to uninstall service";
            SvcUnInstall();
            return 0;
        }
        else if((strcmp(argv[1], "--installstartup") == 0)||(strcmp(argv[1], "-is") == 0))
        {
            LOG_DEBUG4 << "begin to install and run service";
            SvcInstall(/*alsoRun*/true);
            return 0;
        }
        else if((strcmp(argv[1], "--startup") == 0)||(strcmp(argv[1], "-s") == 0))
        {
            LOG_DEBUG4 << "begin to start service";
            SvcStart();
            return 0;
        }
        else if((strcmp(argv[1], "--authenticate") == 0)||(strcmp(argv[1],"-au") == 0))
        {
            LOG_DEBUG4 << "Authen before uninstall";
            SvcAuthen(argc, argv);
            return 0;
        }
    }

    LOG_DEBUG4 << "start gbclient service";
    SERVICE_TABLE_ENTRY DispatchTable[] =
    {
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION)SvcMain },
        { NULL, NULL }
    };

    if (!StartServiceCtrlDispatcher(DispatchTable))
    {
        DWORD errorCode = GetLastError();
        LOG_ERROR << "StartServiceCtrlDispatcher failed. Error code: " << errorCode;
        // God Error.
    }

    LOG_DEBUG << "quit the service program";
    return 0;
}

int main(int argc, char *argv[])
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // initialize the log file
    {
        LogManager::GetInstance()->init();
        LOG_DEBUG4 << "client service " 
            << VERSIONSTRING << "-" << BRANCH 
            << " is launching. "
            << qPrintable(DateTimeUtils::dateTimeToString(QDateTime::currentDateTime()));
    }

    //////////////////////////////////////////////////////////////////////////
    // init crash handler
    CrashHandlerT::GetInstance()->Init(const_cast<wchar_t*>(QString(VERSIONSTRING + QString("-cs")).toStdWString().c_str()), 
        L"http://crashreport.garena.com:8080/gcafebilling/upload/");

#ifdef NDEBUG
    try
    {
        return svcLaunch(argc, argv);
    }
    catch (std::exception & e)
    {
        std::cerr << e.what();
        LOG_ERROR << e.what();
    }
    catch(...)
    {
        std::cerr << "unknown error";
        LOG_ERROR << "unknown error";
    }
#else
    return debugStart(argc, argv);
#endif
}

VOID SvcInstall(bool isAlsoRun = false)
{
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if(!GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        printf("Cannot install service (%d)\n", (int)GetLastError());
        return;
    }

    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database
        SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", (int)GetLastError());
        return;
    }

    schService = CreateService(
        schSCManager,              // SCM database
        SVCNAME,                   // name of service
        SVCNAME,                   // service name to display
        SERVICE_ALL_ACCESS,        // desired access
        SERVICE_WIN32_OWN_PROCESS, // service type
        SERVICE_AUTO_START,        // start type
        SERVICE_ERROR_NORMAL,      // error control type
        szPath,                    // path to service's binary
        SVCGROUP,                  // no load ordering group
        NULL,                      // no tag identifier
        NULL,                      // no dependencies
        NULL,                      // LocalSystem account
        NULL);                     // no password

    if (schService == NULL)
    {
        printf("CreateService failed (%d)\n", (int)GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }
    else printf("Service installed successfully\n");

    //////////////////////////////////////////////////////////////////////////
    // config windows service failure action (restart service after failure)
    SERVICE_FAILURE_ACTIONS failureAction = {0};
    failureAction.dwResetPeriod = 0;
    failureAction.lpRebootMsg = NULL;

    SC_ACTION actions[3];
    actions[0].Type = SC_ACTION_RESTART;
    actions[0].Delay = Win_Service_Restart_Interval_MS;
    actions[1].Type = SC_ACTION_RESTART;
    actions[1].Delay = Win_Service_Restart_Interval_MS;
    actions[2].Type = SC_ACTION_RESTART;
    actions[2].Delay = Win_Service_Restart_Interval_MS;

    failureAction.cActions = 3;
    failureAction.lpsaActions = actions;
    failureAction.lpCommand = NULL;

    ::ChangeServiceConfig2(schService, SERVICE_CONFIG_FAILURE_ACTIONS, &failureAction);

    //////////////////////////////////////////////////////////////////////////
    // modify the scm's load ordering group settings,
    // put our group ahead of "ShellSvcGroup" group, so that the service
    // will run before client UI App
    {
        const QString SHELL_GROUP = "ShellSvcGroup"; // insert our group before this
        QString keyHive = "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\ServiceGroupOrder";
        QSettings settings( keyHive, QSettings::NativeFormat ); 
        QStringList groups = settings.value("list", QStringList()).toStringList();
        if (groups.contains(Q_SVCGROUP, Qt::CaseInsensitive) == false)
        {
            LOG_INFO << "the expected service group doesn't exist, insert our group";
            QStringList newGroups;
            foreach(const QString &orderStr, groups)
            {
                if (QString::compare(orderStr, SHELL_GROUP, Qt::CaseInsensitive) == 0)
                {
                    newGroups << Q_SVCGROUP << SHELL_GROUP;
                }
                else
                {
                    newGroups << orderStr;
                }
            }

            settings.setValue("list", newGroups);
        }
        else
        {
            LOG_INFO << "the expected service group already exists";
        }
    }
    //////////////////////////////////////////////////////////////////////////


    // launch service, if the service is expected to launch after installation
    if (isAlsoRun)
    {
        ::StartService(schService, 0, 0);
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

VOID SvcStart()
{
    SC_HANDLE schSCManager = ::OpenSCManager(
        NULL,                    // machine (NULL == local)
        NULL,                    // database (NULL == default)
        SC_MANAGER_ALL_ACCESS);  // full access rights
    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", (int)GetLastError());
        return;
    }

    SC_HANDLE schService = ::OpenService(
        schSCManager,              // SCM database
        SVCNAME,                   // name of service
        SERVICE_ALL_ACCESS         // desired access
        );
    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", (int)GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    // try to start the service
    _tprintf(TEXT("Starting up %s."), SVCNAME);
    if( ::StartService(schService, 0, 0) ) 
    {
        Sleep(500);

        int count = 0;
        SERVICE_STATUS ssStatus;
        while( ::QueryServiceStatus(schService, &ssStatus) ) 
        {
            // if longer than 300s (5min), force quit
            if (count > 600)
            {
                break;
            }

            if( ssStatus.dwCurrentState == SERVICE_START_PENDING ) 
            {
                _tprintf(TEXT("."));
                ::Sleep( 500 );
                ++count;
            } 
            else
            {
                break;
            }
        }

        if( ssStatus.dwCurrentState == SERVICE_RUNNING )
        {
            _tprintf(TEXT("\n%s started.\n"), SVCNAME);
        }
        else
        {
            _tprintf(TEXT("\n%s failed to start.\n"), SVCNAME);
        }
    } 
    else 
    {
        // StartService failed
        _tprintf(TEXT("\n%s failed to start: %d\n"), SVCNAME, GetLastError());
    }

    ::CloseServiceHandle(schService);
    ::CloseServiceHandle(schSCManager);
}

VOID SvcUnInstall()
{
    LOG_DEBUG4 << "Begin to uninstall gbClientService";
    SC_HANDLE schSCManager;
    SC_HANDLE schService;

    //////////////////////////////////////////////////////////////////////////
    // when uninstalling the client service, also remove client app from "UserInit" method
    {
        LOG_DEBUG << "Recover system configurations";
        QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";
        QString userInitRegKeyName = "Userinit";
        QSettings settings( keyHive, QSettings::NativeFormat ); 

        QString sys32FolderPath = SysInfoHelper::system32Folder();
        QString newUserInitStr = sys32FolderPath + "\\userinit.exe,";
        settings.setValue(userInitRegKeyName, newUserInitStr);
    }
    //////////////////////////////////////////////////////////////////////////


    schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database
        SC_MANAGER_ALL_ACCESS);  // full access rights

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", (int)GetLastError());
        return;
    }

    schService = OpenService(schSCManager, SVCNAME, SC_MANAGER_ALL_ACCESS);
    if(schService == NULL)
    {
        printf("OpenService failed (%d)\n", (int)GetLastError());
        CloseServiceHandle(schSCManager);
        return;
    }

    if(!DeleteService(schService))
    {
        printf("DeleteService failed (%d)\n", (int)GetLastError());
        return;
    }
    else
        printf("Service uninstall successfully\n");

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
}

VOID WINAPI SvcMain(int dwArgc, char** lpszArgv)
{
    gSvcStatusHandle = RegisterServiceCtrlHandler(
        SVCNAME,
        SvcCtrlHandler);

    if(!gSvcStatusHandle)
    {
        DWORD errorCode = GetLastError();
        LOG_DEBUG << "RegisterServiceCtrlHandler failed. Error code: " << errorCode;
        return;
    }

    gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    gSvcStatus.dwServiceSpecificExitCode = 0;

    ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    SvcInit(dwArgc, lpszArgv);
}

VOID SvcInit(int argc, char** argv)
{
    // this is IMPORTANT, as we have a "CcCoreAppSingleton" inst here, 
    // an at the end of thie method, we set the service status as SERVICE_STOPPED ("from MSDN, 
    // after service status is SERVICE_STOPPED, the execution may be stopped at any time, so 
    // any operation after it may NOT be invoked)
    // since CcCoreAppSingleton inst is on stack, and its dtor will only be invoked when out of scope, 
    // which means, if without this pair of curly braces, its dtor will be called when the method runs out 
    // of scope, and definitely AFTER "ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0)" is invoked
    // SO: put a pair of curly braces here, make "CcCoreAppSingleton" be dtored before set service as stopped
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // app sineleton
#ifdef NDEBUG
        LOG_DEBUG << "svcinit";
        CcCoreAppSingleton appSingleton(APP_SINGLETON_NAME);
        if (appSingleton.isAlreadyRunning())
        {
            LOG_WARN << "There's already an Service instance running. Exiting...";
            exit(1);
            return;
        }
#endif

        QCoreApplication coreApp(argc, argv);
        coreApp.setOrganizationName(G_COMPANY);
        coreApp.setApplicationVersion(UIVERSION);

        int retCode = _tryRun();

        if (retCode == 0)
        {
            ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
            // Perform work until service stops.
            LOG_DEBUG << "Enter QCoreApplication event loop";
            coreApp.exec();

            LOG_DEBUG << "svc about to quit, close network";
            Client::GetInstance()->stop();
        }
        else
        {
            LOG_ERROR << "failed to init & run svc";
        }
    }

    LOG_DEBUG << "quit QCoreApplication event loop";
    ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

VOID ReportSvcStatus(int dwCurrentState,
                     int dwWin32ExitCode,
                     int dwWaitHint)
{
    static int dwCheckPoint = 1;

    gSvcStatus.dwCurrentState = dwCurrentState;
    gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    gSvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
    else gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ((dwCurrentState == SERVICE_RUNNING) ||
        (dwCurrentState == SERVICE_STOPPED))
        gSvcStatus.dwCheckPoint = 0;
    else gSvcStatus.dwCheckPoint = dwCheckPoint++;

    SetServiceStatus(gSvcStatusHandle, &gSvcStatus);
}

VOID WINAPI SvcCtrlHandler(DWORD dwCtrl)
{
    switch(dwCtrl)
    {
    case SERVICE_CONTROL_STOP:   // SCM STOP SIGNAL
        {
            ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
            QCoreApplication::quit();
            return;
        }
    case SERVICE_CONTROL_INTERROGATE:
        break;
    default:
        break;
    }
    ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
}

VOID SvcAuthen(int argc, char *argv[])
{
    if(argc >= 4)
    {
        QString userName = QString::fromAscii(argv[2]).toUpper();
        QString passWord = QString::fromAscii(argv[3]);
        LOG_DEBUG << "Authen before uninstall";
        bool ret = iniBeforeAuthen();
        if(ret == false)
        {
            LOG_DEBUG << "Authen init failed";
            exit(1);
        }
        QString pwdHash = Utils::hashUserData(userName, passWord, HashType_GCB_MD5);
        ret = (QString::compare(pwdHash,CConfigManager::adminCodeHash(), Qt::CaseInsensitive) == 0);
        if(ret == true)
        {
            LOG_DEBUG << "Admin Authen success";
            exit(0);
        }
    }
    LOG_DEBUG << "Admin Authen failed";
    exit(1);
}