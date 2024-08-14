#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QResource>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>

#include "CcCore/CcMultiLangHelper.h"
#include "CcCore/CcCoreAppSingleton.h"
#include "log.h"
#include "SysInfoHelper.h"
#include "ConfigManager.h"
#include "Utils.h"
#include "Client.h"
#include <exception>
#include "ClientAppSettings.h"
#include "CommonSettings.h"
#include "DateTimeUtils.h"
#include "LogManager.h"
#include "GuiController.h"
#include "crashrpt.h"
#include "OSAgentWinQt.h"
#include <iostream>
#include "TimerTaskManager.h"
#include "ClientEventManager.h"
#include "RegionConsts.h"
#include "StringUtils.h"
#include "RegionalSettings.h"

extern const char* UIVERSION;
extern const char* VERSIONSTRING;
extern const char* BRANCH;
const QString APP_SINGLETON_NAME = "Global\\{6C020E63-13B2-47fe-A1EA-BA64C2752591}";

// in case client crashes and couldn't launch system's userinit.exe,
// we have another tool for back up
// it will delay 5 seconds to launch, and if it can't find "explorer.exe",
// it will try to run userinit.exe from system32 folder
const QString USERINIT_HELPER_TOOL_NAME = "gInitTool.exe";

bool launchClientLocker()
{
    bool isLockerRunning = OSAgentWinQt::GetInstance()->isProcessRunning(LOCKER_EXE_NAMEWITHEXT.toStdWString());
    if ( ! isLockerRunning)
    {
        LOG_DEBUG << "locker is NOT running";

        QString clientFolderPath = QDir::toNativeSeparators(qApp->applicationDirPath());
        QString lockerExePath = clientFolderPath + "\\" + LOCKER_EXE_NAMEWITHEXT;
        LOG_DEBUG << "Locker path: " << qPrintable(lockerExePath);

        QStringList args;
        isLockerRunning = QProcess::startDetached(lockerExePath, args);
    }

    return isLockerRunning;
}

bool launchClientService()
{
    bool isRunning = OSAgentWinQt::GetInstance()->isProcessRunning(SVC_EXE_NAMEWITHEXT.toStdWString());
    if ( ! isRunning)
    {
        LOG_DEBUG << "service is NOT running";

        QString clientFolderPath = QDir::toNativeSeparators(qApp->applicationDirPath());
        QString exePath = clientFolderPath + "\\" + SVC_EXE_NAMEWITHEXT;
        LOG_DEBUG << "service path: " << qPrintable(exePath);

        QStringList args;
        args << "-is"; // to install and run the windows service
        isRunning = QProcess::startDetached(exePath, args);
    }

    return isRunning;
}

// make sure client app is in userInit registry
// put client app into "UserInit" method
// so when OS starts, it launches the programs in "UserInit" which stored in registry:
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon
// put our program after userinit.exe (which will run explorer in turn), so that: 
// after OS runs userinit.exe, OS will immediately run our client UI program, and during this
// time, the userinit.exe will launch explorer asynchronously, so this change will
// make client UI app pop up very quickly
bool preConfig()
{
    // get the client App exe path
    QString clientFolderPath = QDir::toNativeSeparators(qApp->applicationDirPath());

    QString clientAppExePath = QDir::toNativeSeparators(qApp->applicationFilePath());
    QString lockerExePath = clientFolderPath + "\\" + LOCKER_EXE_NAMEWITHEXT;
    QString helperToolExePath = clientFolderPath + "\\" + USERINIT_HELPER_TOOL_NAME;

    QString keyHive = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon";
    QSettings settings( keyHive, QSettings::NativeFormat ); 
    QString userInitRegKeyName = "Userinit";
    QString userInitVal = settings.value(userInitRegKeyName, QString()).toString();

    QByteArray newUserInitBuff; // the new "UserInit" registry values
    // add gbClientLocker.exe 
    newUserInitBuff.append(lockerExePath).append(",");
    // add gbClientApp.exe 
    newUserInitBuff.append(clientAppExePath).append(",");
    // add user init helper tool "initTool.exe"
    newUserInitBuff.append(helperToolExePath).append(",");

    settings.setValue(userInitRegKeyName, QString(newUserInitBuff));
    //////////////////////////////////////////////////////////////////////////

    return true;
}

int tryRun( int argc, char** argv)
{
    int retCode = 0;
    QApplication app(argc, argv);
    app.setOrganizationName(G_COMPANY);
    app.setApplicationVersion(UIVERSION);

    app.setQuitOnLastWindowClosed(false);

    //////////////////////////////////////////////////////////////////////////
    // initialize the log file
    {
        LogManager::GetInstance()->init();
        LOG_INFO << "client app " 
            << VERSIONSTRING << "-" << BRANCH 
            << " is launching. "
            << qPrintable(DateTimeUtils::dateTimeToString(QDateTime::currentDateTime()));
    }
    //////////////////////////////////////////////////////////////////////////
    // init crash handler
    CrashHandlerT::GetInstance()->Init(const_cast<wchar_t*>(QString(VERSIONSTRING + QString("-cc")).toStdWString().c_str()), 
        L"http://crashreport.garena.com:8080/gcafebilling/upload/");

    //////////////////////////////////////////////////////////////////////////
    // app sineleton
    CcCoreAppSingleton appSingleton(APP_SINGLETON_NAME);
    if (appSingleton.isAlreadyRunning())
    {
        LOG_WARN << "There's already an client instance running. Exiting...";
        QCoreApplication::exit(1);
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    // launch the "clientLocker.exe", if it's not running
    bool succeeded = launchClientLocker();
    if (!succeeded)
    {
        LOG_ERROR << "failed to launch client locker";
    }

    // launch the "client service", if it's not running
    succeeded = launchClientService();
    if (!succeeded)
    {
        LOG_ERROR << "failed to launch client service";
    }

    //////////////////////////////////////////////////////////////////////////
    // init config manager and load settings
    CConfigManager::init();
    // load settings stored in registry
    CConfigManager::loadRegData();

    //////////////////////////////////////////////////////////////////////////
    // make sure client app and tool is in user init registry
    if ( ! CConfigManager::isDeveloperEnv())
    {
        bool preConfigSucceeded = preConfig();
        if (!preConfigSucceeded)
        {
            QString msg = QCoreApplication::tr("Pre-Configuration failed. Please reinstall the program to solve this problem.");
            QMessageBox::critical(NULL, QCoreApplication::tr("Error"), msg);
            return 1;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // load the resource file
    QString rccFile = QApplication::applicationDirPath() + "/clientApp.rcc";
    bool rccLoaded = false;
    if (true == QFile::exists(rccFile))
    {
        rccLoaded = QResource::registerResource(rccFile);
    }

    if (rccLoaded == false)
    {
        QString msg = QCoreApplication::tr("Some critical files are missing or broken, please reinstall the program to solve this problem.");
        QMessageBox::critical(NULL, QCoreApplication::tr("Error"), msg);
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    // install the translator
    LOG_DEBUG4 << "Begin to load translater";
    CcMultiLangHelper::loadTranslations(QApplication::applicationDirPath() + "/tr/clientApp");

    QString lang = RegionConsts::getRegionLanguage();
    QString region = RegionConsts::getRegion();

    if (lang.length() >= 2)
    {
        QString langPart = lang.left(2);
        CrashHandlerT::GetInstance()->UpdateLanguage(QStringToTCHAR(langPart));
    }

    bool result = CcMultiLangHelper::setLanguage(lang);
    if ( ! result)
    {
        LOG_ERROR << "can't find language file: " << qPrintable(lang);
    }

    result = RegionalSettings::init();

    //////////////////////////////////////////////////////////////////////////
    Client::GetInstance()->init();
    Client::GetInstance()->connectSigSlot();

    // start the full screen dialog here
    GuiController::GetInstance()->init();
    GuiController::GetInstance()->connectSigSlot();

    // hide it first (wait for user login to display)
    GuiController::GetInstance()->hideMiniWindow();

    //////////////////////////////////////////////////////////////////////////
    // launch system userinit.exe to start explorer.exe
    //if ( ! CConfigManager::isDeveloperEnv())
    {
        LOG_DEBUG4 << "launch system userinit";
        TimerTaskManager::launchSystemUserInit();
    }

    //////////////////////////////////////////////////////////////////////////
    // begin the network connection to client service first
    Client::GetInstance()->startUp();

    //////////////////////////////////////////////////////////////////////////
    // if the cafe admin doesn't configure this client yet (server address), pop up a config dialog
    QString serverAddr = CConfigManager::serverAddress();

    // run client event: startup now
    LOG_DEBUG << "Trying to start client event" ;
    ClientEventManager* clientEventManager = ClientEventManager::GetInstance();
    clientEventManager->init();
    LOG_DEBUG << "Connect sig slot" ;
    clientEventManager->connectSigSlot();

    return app.exec();
}

int main( int argc, char** argv)
{

#ifdef NDEBUG
    try
    {
        tryRun(argc, argv);
    }
    catch (std::exception const& e)
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
    tryRun(argc, argv);
#endif

}
