#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QResource>
#include <QMessageBox>
#include <QProcess>

#include "CcCore/CcMultiLangHelper.h"
#include "CcCore/CcCoreAppSingleton.h"
#include "log.h"
#include "SysInfoHelper.h"
#include "ConfigManager.h"
#include "Utils.h"
#include "Client.h"
#include <exception>

#include "RegexpSettings.h"
#include "CommonSettings.h"
#include "DateTimeUtils.h"
#include "LogManager.h"
#include "GuiController.h"
#include "crashrpt.h"
#include "ClientCommonSettings.h"
#include <iostream>
#include "RegionalSettings.h"
#include "ClientEventManager.h"
#include "RegionConsts.h"
#include "StringUtils.h"

extern const char* UIVERSION;
extern const char* VERSIONSTRING;
extern const char* BRANCH;
const QString APP_SINGLETON_NAME = "Global\\{DE0BFCC5-2198-4ec8-9BC0-809963130BB9}";

int tryRun( int argc, char** argv)
{
    int retCode = 0;
    QApplication app(argc, argv);
    app.setOrganizationName(G_COMPANY);
    app.setApplicationVersion(UIVERSION);

    app.setQuitOnLastWindowClosed(false);

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
    // init config manager and load settings
    bool configInited = CConfigManager::init();
    if (configInited == false)
    {
        QString msg = QCoreApplication::tr("Can't load program configurations. Please reinstall the program to solve this problem.");
        QMessageBox::critical(NULL, QCoreApplication::tr("Error"), msg);
        return 1;
    }

    //////////////////////////////////////////////////////////////////////////
    // load the resource file
    QString rccFile = QApplication::applicationDirPath() + "/clientLocker.rcc";
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
    //LOG_DEBUG4 << "Begin to load translater";
    CcMultiLangHelper::loadTranslations(QApplication::applicationDirPath() + "/tr/clientLocker");

    QString region = RegionConsts::getRegion();
    QString lang = RegionConsts::getRegionLanguage();

    bool result = CcMultiLangHelper::setLanguage(lang);
    if (lang.length() >= 2)
    {
        QString langPart = lang.left(2);
        CrashHandlerT::GetInstance()->UpdateLanguage(QStringToTCHAR(langPart));
    }

    if ( ! result)
    {
        LOG_ERROR << "can't find language file: " << lang.toStdString();
    }

    result = RegionalSettings::init();
    if (result == false)
    {
        LOG_ERROR << "can't find regional setting file: " << region.toStdString();
    }

    //////////////////////////////////////////////////////////////////////////
    Client::GetInstance()->init();
    Client::GetInstance()->connectSigSlot();

    // start the full screen dialog here
    GuiController::GetInstance()->init();
    GuiController::GetInstance()->connectSigSlot();

    // pop up the full screen dialog
    GuiController::GetInstance()->lockClient();

    GuiController::GetInstance()->delayInit();
    GuiController::GetInstance()->delayConnectSigSlot();

    //////////////////////////////////////////////////////////////////////////
    // begin the network connection to client service first
    Client::GetInstance()->startUp();

	LOG_DEBUG << "Trying to start client event" ;
	ClientEventManager* clientEventManager = ClientEventManager::GetInstance();
	clientEventManager->init();
	LOG_DEBUG << "Connect sig slot" ;
	clientEventManager->connectSigSlot();

    retCode = app.exec();

    LOG_INFO << "locker quit, code: " << retCode;
    return retCode;
}

int main( int argc, char** argv)
{
    //////////////////////////////////////////////////////////////////////////
    // initialize the log file
    {
        LogManager::GetInstance()->init();
        LOG_INFO << "client locker " 
            << VERSIONSTRING << "-" << BRANCH 
            << " is launching. "
            << qPrintable(DateTimeUtils::dateTimeToString(QDateTime::currentDateTime()));
    }

    //////////////////////////////////////////////////////////////////////////
    // init crash handler
    CrashHandlerT::GetInstance()->Init(const_cast<wchar_t*>(QString(VERSIONSTRING + QString("-cl")).toStdWString().c_str()), 
        L"http://crashreport.garena.com:8080/gcafebilling/upload/");
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
