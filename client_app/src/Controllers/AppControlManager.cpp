#include "AppControlManager.h"

#include <QStringList>
#include <QTimer>

#include "cc2csNetworkManager.h"
#include "ApplicationProhibition.h"
#include "CommonSettings.h"
#include "OSAgentWinQt.h"
#include "log.h"
#include "UserAccountManager.h"
#include "SystemManager.h"
#include "ConfigManager.h"
#include "AppControlQtModel.h"
#include "cs2ssCmds.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"

bool AppControlManager::init()
{
    return true;
}

bool AppControlManager::connectSigSlot()
{
    // after server online, send command to product data from server
    {
        cc2csNetworkManager* networkMgr = cc2csNetworkManager::GetInstance();
        connect(networkMgr, SIGNAL(serverConnected()),
            this, SLOT(serverConnectedSlot()));
    }

    return true;
}

ApplicationProhibition* AppControlManager::getAppProhibitionById( int id )
{
    foreach(ApplicationProhibition* appProhi, m_appProhibitList)
    {
        if (appProhi->id() == id)
        {
            return appProhi;
        }
    }

    return NULL;
}

void AppControlManager::serverConnectedSlot()
{
    // step 1: remove all obsolete app prohibitions
    // reason: suppose when this client is down, the server removed several app prohibitions
    // but client didn't receive them. after client is ON, and query from server
    // client will never remove those removed app prohibitions
    if (m_appProhibitList.isEmpty() == false)
    {
        foreach(ApplicationProhibition* appControl, m_appProhibitList)
        {
            _enableApplication(appControl);
        }
    }

    // step 2: ask for latest app prohibitions
    {
        AppControlRequestQtModel qtModel(CConfigManager::clientId());
        cc2csSender::send(cc2cs::AppControlRequestCommandC(qtModel));
    }
}

bool AppControlManager::_applyApplicationProhibition( ApplicationProhibition* app )
{
    if (CConfigManager::isDisableSecurityRestrictions() == true)
    {
        return true;
    }

    OSAgentWin* agent = OSAgentWinQt::GetInstance();

    if (CConfigManager::isDeveloperEnv() == false)
    {
        switch(app->prohibitType())
        {
        case AppControl_FilePath:
            if (app->enabled() == true)
            {
                agent->disableAppRunByProcName(app->fileName().toStdWString());
            }
            else
            {
                agent->enableAppRunByProcName(app->fileName().toStdWString());
            }
            break;
        case AppControl_FileHashMD5:
            {
                QStringList paramContent = app->params().split(":", QString::SkipEmptyParts);
                if (paramContent.size() != 3)
                {
                    return false;
                }

                if (app->enabled() == true)
                {
                    if (!app->oldParams().isEmpty())
                    {
                        QStringList oldParamContent = app->params().split(":", QString::SkipEmptyParts);
                        if (oldParamContent.size() == 3)
                        {
                            agent->enableAppRunByHash(oldParamContent.at(0));
                        }
                    }

                    agent->disableAppRunByHash(app->fileName(), paramContent.at(0), paramContent.at(1), paramContent.at(2));
                }
                else
                {
                    agent->enableAppRunByHash(paramContent.at(0));
                }
            }
            break;
        case AppControl_FileSig:
            {
                if (app->enabled() == true)
                {
                    if (!app->oldParams().isEmpty())
                    {
                        agent->enableAppRunBySig(app->oldParams());
                    }

                    agent->disableAppRunBySig(app->fileName(), app->params());
                }
                else
                {
                    agent->enableAppRunBySig(app->params());
                }
            }
            break;
        default:
            break;
        }
    }

    // if the server removed an app prohibit rule, we should also do it from client side (after removing the rule from the registry)
    if (app->enabled() == false)
    {
        m_appProhibitList.removeOne(app);
    }

    return true;
}

//
//bool AppControlManager::enableAllApplications()
//{
//    OSAgentWin* agent = OSAgentWinQt::GetInstance();
//
//    if (CConfigManager::isDeveloperEnv() == false)
//    {
//        agent->enableAllAppsRunByProcName();
//        //agent->enableAllAppsRunByHash();
//    }
//    return true;
//}

void AppControlManager::applyAllApplicationControls()
{
    foreach (ApplicationProhibition* appProhibit, m_appProhibitList)
    {
        _applyApplicationProhibition(appProhibit);
    }
}

bool AppControlManager::_enableApplication( ApplicationProhibition* app )
{
    if (app == NULL ||
        CConfigManager::isDisableSecurityRestrictions() == true)
    {
        return true;
    }

    OSAgentWin* agent = OSAgentWinQt::GetInstance();
    if (CConfigManager::isDeveloperEnv() == false)
    {
        switch(app->prohibitType())
        {
        case AppControl_FilePath:
            agent->enableAppRunByProcName(app->fileName().toStdWString());
            break;
        case AppControl_FileHashMD5:
            {
                QStringList paramContent = app->params().split(":", QString::SkipEmptyParts);
                if (paramContent.size() != 3)
                {
                    return false;
                }

                agent->enableAppRunByHash(paramContent.at(0));
            }
            break;
        case AppControl_FileSig:
            {
                agent->enableAppRunBySig(app->params());
            }
        default:
            break;
        }
    }

    // if the server removed an app prohibit rule, we should also do it from client side 
    // (after removing the rule from the registry)
    if (m_appProhibitList.contains(app))
    {
        m_appProhibitList.removeOne(app);
        delete app;
    }

    return true;
}


void AppControlManager::procAppControlReply( const AppControlReplyQtModel &qtModel )
{
    if (qtModel.Items_.isEmpty())
    {
        return;
    }

    foreach(const AppControl_ReplyAppItem &qtItem, qtModel.Items_)
    {
        int id = qtItem.AppProhibitId_;
        ApplicationProhibition* app = getAppProhibitionById(id);

        // there's existing record on client side
        if (app != NULL)
        {
            // enable the old restriction (and release the memory)
            _enableApplication(app);
            app = NULL; // already deleted inside _enableApplication() method
        }

        // if the application is removed from server side, since we've already enabled the old record
        // so it's safe to directly return
        if (qtItem.Enabled_ == false)
        {
            continue;
        }

        app = new ApplicationProhibition(qtItem.AppProhibitId_, qtItem.ProhibitType_,
        qtItem.FileName_, qtItem.Params_, qtItem.OldParams_, qtItem.Enabled_);
        m_appProhibitList.append(app);
        _applyApplicationProhibition(app);
    }
}


void AppControlManager::procAppControlWhiteReply(const AppControlReplyQtModel &qtModel)
{
    if (qtModel.Items_.isEmpty())
    {
        return;
    }

    foreach(const AppControl_ReplyAppItem &qtItem, qtModel.Items_)
    {
        if(qtItem.Enabled_ == true)
        {
            SystemManager::GetInstance()->addExcludeApp(qtItem.FileName_);
        }else
        {
            SystemManager::GetInstance()->removeExcludeApp(qtItem.FileName_);
        }
    }
}

