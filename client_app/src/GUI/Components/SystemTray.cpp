#include "SystemTray.h"
#include <QIcon>
#include <QAction>
#include <QWidget>
#include <QMenu>
#include "ConfigManager.h"
#include "ClientAppSettings.h"
#include "GuiController.h"
#include "CommonSettings.h"


SystemTray::SystemTray(QWidget *parent) 
: QObject(parent), m_parent(parent), 
m_trayIcon(NULL), trayIconMenu(NULL), restoreAction(NULL), quitAction(NULL)
{
    init();
    connectSigSlot();
}

SystemTray::~SystemTray()
{
    if (m_trayIcon != NULL)
    {
        m_trayIcon->deleteLater();
        m_trayIcon = NULL;
    }
}

bool SystemTray::init()
{
    // init the tray icon
    m_trayIcon = new QSystemTrayIcon(m_parent);
    QIcon icon(":/icons/icons/Common/client.ico");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip(G_DISPLAY_SOFTWARE);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(trayIconActivatedSlot(QSystemTrayIcon::ActivationReason)));

    // init the tray icon's context menu
    trayIconMenu = new QMenu(m_parent);
    restoreAction = new QAction(tr("&Restore"), m_parent);
    // forward this signal to "restoreMinidialogSignal" signal, so that GuiController knows to pop up the mini dialog
    connect(restoreAction, SIGNAL(triggered()), 
        this, SLOT(restoreActionSlot()));
    trayIconMenu->addAction(restoreAction);
    // set the menu to the tray icon
    m_trayIcon->setContextMenu(trayIconMenu);

    m_trayIcon->show();
    return true;
}

bool SystemTray::connectSigSlot()
{
    return true;
}

// since SystemTray doesn't get "changeEvent" to re-implement, so it depends on the MiniDialog's languageChanged signal
// and GuiController will call this method (GuiController will take charge of all the UI elements who don't get "changeEvent")
void SystemTray::retranslateUiEx()
{
    if (m_trayIcon)
    {
        m_trayIcon->setToolTip(G_DISPLAY_SOFTWARE);
    }
    
    if (restoreAction)
    {
        restoreAction->setText(tr("&Restore"));
    }
}

void SystemTray::clearMessage()
{
    if (CConfigManager::showTrayMsg() == false)
    {
        return;
    }

    m_trayIcon->showMessage(QString(), QString(), QSystemTrayIcon::NoIcon, 0);
}

void SystemTray::showMessage( const QString& msg, MessageLevel level )
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    switch(level)
    {
    case MessageLevel_Info:
        icon = QSystemTrayIcon::Information;
        break;
    case MessageLevel_Warn:
        icon = QSystemTrayIcon::Warning;
        break;
    case MessageLevel_Error:
        icon = QSystemTrayIcon::Critical;
        break;
    }

    _showMsgImp(QString(), msg, icon);
}

void SystemTray::_showMsgImp( const QString& title, const QString& msg, 
                            QSystemTrayIcon::MessageIcon icon )
{
    if (CConfigManager::showTrayMsg() == false)
    {
        return;
    }

    m_trayIcon->showMessage(title, msg, icon, CConfigManager::trayMsgDuration());
}

void SystemTray::trayIconActivatedSlot( QSystemTrayIcon::ActivationReason reason )
{
    if (reason != QSystemTrayIcon::DoubleClick)
    {
        return;
    }
    _restoreMiniDialog();
}

void SystemTray::restoreActionSlot()
{
    _restoreMiniDialog();
}

void SystemTray::_restoreMiniDialog()
{
    GuiController::GetInstance()->restoreMiniDialog();
}

void SystemTray::show()
{
    m_trayIcon->show();
}

void SystemTray::hide()
{
    m_trayIcon->hide();
}


