#include "SystemManager.h"
#include "ScreenLockQtModel.h"
#include "log.h"
#include "OSAgentWinQt.h"
#include <QApplication>

bool SystemManager::s_osAppearanceRestricted = false;

SystemManager::~SystemManager()
{
    resumeOSAccess();
}

bool SystemManager::init()
{
    return true;
}

bool SystemManager::connectSigSlot()
{
    return true;
}

void SystemManager::procScreenLockReply( const ScreenLockReplyQtModel &qtModel )
{
    if (qtModel.Mode_ == LockMode_Lock &&
        qtModel.Result_ == true)
    {
        emit lockScreenRequested();
    }
}

void SystemManager::procScreenUnlockReply( const ScreenUnlockReplyQtModel& qtModel )
{
    emit screenUnlockReplyRecved(qtModel.Mode_, qtModel.Result_);

    if ((qtModel.Mode_ == LockMode_UnlockByTmpPwd || qtModel.Mode_ == LockMode_UnlockByAcc) &&
        qtModel.Result_ == true)
    {
        emit unlockScreenRequested();
    }
}

bool SystemManager::restriceOSAccess()
{
    //LOG_DEBUG << "hide OS bars";

    if (s_osAppearanceRestricted == true)
    {
        //LOG_DEBUG << "already hidden";
        return true; 
    }

    // hide taskbar & startmenu
    OSAgentWinQt::GetInstance()->disableCAD();
    OSAgentWinQt::GetInstance()->disableSystemHotKey();
    OSAgentWinQt::GetInstance()->hideTaskBar();
    OSAgentWinQt::GetInstance()->hideStartMenu();

    s_osAppearanceRestricted = true;
    //LOG_DEBUG << "OS bars are hidden now";

    return true;
}

bool SystemManager::resumeOSAccess()
{
    //LOG_DEBUG << "resume OS bars";
    if (s_osAppearanceRestricted == false)
    {
        //LOG_DEBUG << " already visible";
        //return true; 
    }

    // show taskbar & startmenu
    OSAgentWinQt::GetInstance()->enableCAD();
    OSAgentWinQt::GetInstance()->enableSystemHotKey();
    OSAgentWinQt::GetInstance()->showTaskBar();
    OSAgentWinQt::GetInstance()->showStartMenu();

    //LOG_DEBUG << "OS bars are visible now";
    s_osAppearanceRestricted = false;

    return true;
}

void SystemManager::procEndConnectionReply( const EndConnectionReplyQtModel &qtModel )
{
    LOG_INFO << "end connection recved, quit";
    QApplication::exit(0);
}


