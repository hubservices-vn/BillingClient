#include "GuiController.h"

#include "FullScreenDialog2.h"
#include "LoginDialog.h"
#include "InfoDialog.h"
#include "TempLockDialog.h"
#include "log.h"
#include "ConfigManager.h"
#include "CssLoader.h"
#include "SystemManager.h"
#include "SessionManager.h"



bool GuiController::init()
{
    m_fullScreenDialog = new FullScreenDialog2();
    m_loginDialog = NULL;
    m_infoDialog = NULL;
    m_tempLockDialog = NULL;

    return true;
}

bool GuiController::connectSigSlot()
{
    {
        SessionManager* mgr = SessionManager::GetInstance();
        connect(mgr, SIGNAL(sessionStarted()),
            this, SLOT(unlockClient()));
        connect(mgr, SIGNAL(sessionEnded()),
            this, SLOT(lockClient()));
    }

    {
        SystemManager* mgr = SystemManager::GetInstance();
        connect(mgr, SIGNAL(lockScreenRequested()),
            this, SLOT(tempLockScreen()));
        connect(mgr, SIGNAL(unlockScreenRequested()),
            this, SLOT(unlockScreen()));
    }

    // listen to other signals raised from other dialog
    {
        // event connection
        connect(m_fullScreenDialog, SIGNAL(popupLoginDialogSignal()), 
            this, SLOT(popupLoginDialog()));
    }

    return true;
}

bool GuiController::delayInit()
{
    m_loginDialog = new LoginDialog(m_fullScreenDialog);
    m_tempLockDialog = new TempLockDialog(m_fullScreenDialog);
    m_infoDialog = new InfoDialog(m_fullScreenDialog);
    // load css and set to whole app
    // this must be done after the server's rcc loaded, as the css is fetched from the server's rcc
    QString mainCss = CssLoader::loadAllCssFiles();
    QApplication* app = static_cast<QApplication*>(QApplication::instance());
    if (app != NULL)
    {
        app->setStyleSheet(mainCss);
    }
    m_infoDialog->show();

    return true;
}

bool GuiController::delayConnectSigSlot()
{
    return true;
}

void GuiController::popupLoginDialog()
{
    if (!m_fullScreenDialog->isVisible())
    {
        LOG_DEBUG4 << "force show fullscreen dialog";
        m_fullScreenDialog->showDialog();
    }

    if (!m_infoDialog->isVisible())
    {
        LOG_DEBUG4 << "force show info dialog";
        m_infoDialog->show();
    }

    if (!m_loginDialog->isVisible())
    {
        LOG_DEBUG4 << "login dialog poped up";
        m_loginDialog->show();
    }
}

bool GuiController::lockClient()
{
    LOG_DEBUG4 << "lockClient";
    // hide OS taskbar
    SystemManager::GetInstance()->restriceOSAccess();
    // hide other dialogs
    if (m_loginDialog != NULL)
    {
        m_loginDialog->hide();
    }

    if (m_tempLockDialog != NULL)
    {
        m_tempLockDialog->hide();
    }

    // show the full dialog
    m_fullScreenDialog->showDialog();

    if (m_infoDialog != NULL)
    {
        LOG_DEBUG4 << "force show info dialog";
        m_infoDialog->show();
    }

    return true;
}

bool GuiController::unlockClient()
{
    LOG_DEBUG4 << "unlockClient";

    // restore OS taskbar
    SystemManager::GetInstance()->resumeOSAccess();

    // hide all dialogs
    m_loginDialog->closeDialog();
    m_infoDialog->hide();
    m_tempLockDialog->hide();
    m_fullScreenDialog->closeDialog();

    return true;
}

void GuiController::tempLockScreen()
{
    SystemManager::GetInstance()->restriceOSAccess();

    m_loginDialog->hide();
    m_fullScreenDialog->showDialog();
    m_infoDialog->show();
    m_tempLockDialog->show();
}

void GuiController::unlockScreen()
{
    SystemManager::GetInstance()->resumeOSAccess();
    m_fullScreenDialog->closeDialog();
    m_infoDialog->hide();
    m_loginDialog->hide();
    m_tempLockDialog->hide();
}


