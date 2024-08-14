#include "GuiController.h"

#include <QMessageBox>

#include "MiniDialog.h"
#include "Client.h"
#include "ClientAppSettings.h"
#include "log.h"
#include "ConfigManager.h"
#include "DateTimeUtils.h"
#include "CssLoader.h"
#include "SessionManager.h"
#include "UserAccountManager.h"
#include "SystemManager.h"
#include "ScreenLockQtModel.h"

bool GuiController::init()
{
    m_miniDialog = new MiniDialog();

    // load css and set to whole app
    // this must be done after the server's rcc loaded, as the css is fetched from the server's rcc
    QString mainCss = CssLoader::loadAllCssFiles();
    QApplication* app = static_cast<QApplication*>(QApplication::instance());
    if (app != NULL)
    {
        app->setStyleSheet(mainCss);
    }

    return true;
}

bool GuiController::connectSigSlot()
{
    // session begin/end, unlock/relock the UI
    {
        // event connection
        connect(SessionManager::GetInstance(), SIGNAL(sessionStarted()), 
            this, SLOT(sessionStartedSlot()));
        connect(SessionManager::GetInstance(), SIGNAL(sessionEnded()), 
            this, SLOT(sessionEndedSlot()));
    }

    // screen unlocked/locked, show/hide the mini window
    {
        // event connection
        connect(SystemManager::GetInstance(), SIGNAL(screenUnlockReplyRecved(int,bool)), 
            this, SLOT(screenUnlockReplyRecvedSlot()));
        connect(SystemManager::GetInstance(), SIGNAL(screenLockReplyRecved(int,bool)), 
            this, SLOT(screenLockReplyRecvedSlot()));
    }

    return true;
}

static bool s_miniWindowVisible = false;
void GuiController::sessionStartedSlot()
{
    if (s_miniWindowVisible == false)
    {
        showMiniWindow();
        s_miniWindowVisible = true;
    }
}

void GuiController::sessionEndedSlot()
{
    hideMiniWindow();
    s_miniWindowVisible = false;
}

void GuiController::showMiniWindow() const
{
    m_miniDialog->show();
}

void GuiController::hideMiniWindow() const
{
    m_miniDialog->hide();
}

void GuiController::showSystemMessage( int type, MessageLevel level, const QString& msg )
{
    m_miniDialog->showMessage(msg, level);
}

void GuiController::restoreMiniDialog()
{
    if (UserAccountManager::GetInstance()->CurrentUser() == NULL)
    {
        LOG_ERROR << "User wants to restore the mini dialog, but the logged in user is NULL";
        return;
    }

    m_miniDialog->show();
}

void GuiController::screenUnlockReplyRecvedSlot(int mode, bool result)
{
    if ((mode == LockMode_UnlockByTmpPwd || mode == LockMode_UnlockByAcc) &&
        result == true)
    {
        showMiniWindow();
    }
}

void GuiController::screenLockReplyRecvedSlot(int mode, bool result)
{
    if (mode == LockMode_Lock &&
        result == true)
    {
        hideMiniWindow();
    }
}

