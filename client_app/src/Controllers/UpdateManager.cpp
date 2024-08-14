#include "UpdateManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include "log.h"
#include "UpdateChecker.h"
#include "ConfigManager.h"
#include "ClientConnectionQtModel.h"
#include "GFileUtils.h"
#include "OSAgentWinQt.h"
extern "C"{
#include "clientupdatelib.h"
}

UpdateTask::UpdateTask(QObject *parent) : QObject(parent)
{
}

void UpdateTask::clearToDelFolder()
{
    QString toDelFolderPath = QCoreApplication::applicationDirPath() + "/ToDel";
    LOG_DEBUG << "Updating. Begin to clear folder: " << qPrintable(toDelFolderPath);

    GFileUtils::removeDirRecursive(toDelFolderPath);
}

void UpdateTask::doUpdatingProcess()
{
    LOG_DEBUG << "apply the previous downloaded update files";
    _applyPreviousUpdate();

    LOG_DEBUG << "Check whether there's newer version since last update";
    _doNewUpdate();
}

void UpdateTask::_applyPreviousUpdate()
{
}

void UpdateTask::_doNewUpdate()
{
    //fullScreenDlg.setDisplayMessage(QApplication::tr("Checking updates..."));
    LOG_INFO << "Begin to check update";
    bool hasUpdate = UpdateChecker::HasUpdate();
    if (hasUpdate)
    {
        LOG_INFO << "Found update. Updating...";
        //fullScreenDlg.setDisplayMessage(QApplication::tr("Updates found. Update in progress..."));
        UpdaterState state = UpdateChecker::BeginUpdate();
        LOG_DEBUG4 << "Updater finished downloading, state: " << UpdaterStateToStdString(state);
        // close self & client App
        if (state == UpdaterState_Done)
        {
            LOG_DEBUG4 << "kill client service";
            // close client App
            bool succeeded = OSAgentWinQt::GetInstance()->killProcessByName(SVC_EXE_NAMEWITHEXT.toStdWString());
            if (!succeeded)
            {
                LOG_DEBUG4 << "kill client service failed";
            }
            //// ask service to close it self
            //EndConnectionReplyQtModel qtModel;
            //cs2ccSender::send(cs2cc::EndConnectionReplyCommandC(qtModel));

            // ask self to quit
            LOG_DEBUG4 << "quit client app";
            QCoreApplication::quit();
        }
    }
    else
    {
        LOG_INFO << "No update";
        //fullScreenDlg.setDisplayMessage(QApplication::tr("No update. It's the latest version."));
    }
}

void UpdateThread::run()
{
    if (m_task.isNull())
    {
        return;
    }

    m_task->clearToDelFolder();
    m_task->doUpdatingProcess();
}

UpdateThread::UpdateThread()
{
    m_task.reset(new UpdateTask());
}

bool UpdateManager::beginUpdate(int tarVer, const QString& hash)
{
    if (hash.isEmpty())
    {
        return true;
    }

    return ClientUpdater_Start(
        CConfigManager::serverAddress().toStdWString().c_str(),
        hash.toStdString().c_str());

    //if (m_thread.isNull())
    //{
    //    m_thread.reset(new UpdateThread());
    //}

    //if (m_thread->isRunning())
    //{
    //    LOG_DEBUG << "Update thread is already running, skip";
    //    return true;
    //}

    //LOG_DEBUG << "start update thread";
    //m_thread->start();
    //return true;
}
