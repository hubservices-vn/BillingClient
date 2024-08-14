#include "FullScreenDialog2.h"
#include "uiFullScreenDialog2.h"

#include <QtGui>

#include "log.h"
#include "Client.h"
#include "ConfigManager.h"
#include "ThreadHookHelper.h"
#include "FlashManager.h"
#include "SystemManager.h"
#include "GuiController.h"


const QString LOCAL_URI_PREFIX = "file:///";

#ifdef NDEBUG
// release env
const int MakeTopMostInterval = 5000; // 5,000 ms = 5s
#else
// debug env
const int MakeTopMostInterval = 60000; // 60,000ms = 1 minute
#endif



FullScreenDialog2::FullScreenDialog2(QWidget* parent) 
: QDialog(parent), m_closeAction(DlgCloseAction_NA)
{
    ui = new uiFullScreenDialog2();
    ui->setupUi(this);

    init();

    connectSigSlot();

    //ui->m_flashWidget->installEventFilter(this);
}

FullScreenDialog2::~FullScreenDialog2()
{
    if (ui != NULL)
    {
        delete ui;
        ui = NULL;
    }
}

bool FullScreenDialog2::init()
{
    m_topMostTimer = new QTimer(this);
    m_topMostTimer->setInterval(MakeTopMostInterval);
    connect(m_topMostTimer, SIGNAL(timeout()),
        this, SLOT(makeSelfTopMostSlot()));

    return true;
}

void FullScreenDialog2::connectSigSlot()
{
    // if flash manager says the flash file changed, need to refresh the flash
    {
        FlashManager* mgr = FlashManager::GetInstance();
        connect(mgr, SIGNAL(flashFileChanged()),
            this, SLOT(flashFileChangedSlot()));
    }
}

void FullScreenDialog2::changeEvent( QEvent *e )
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi();
        retranslateUiEx();
        break;
    default:
        break;
    }
}

void FullScreenDialog2::retranslateUiEx()
{
}

static QRect s_lastDlgRect;
static bool s_selfAdjusting = false;
void FullScreenDialog2::showDialog()
{
    LOG_DEBUG << "show fullscreen dialog";
    if (s_selfAdjusting)
    {
        return;
    }

    s_selfAdjusting = true;
    {
        SystemManager::GetInstance()->restriceOSAccess();

        if (CConfigManager::isNoFullScreen() == true)
        {
            this->resize(800, 600);
        }
        else
        {
            setWindowState(
                (windowState() & ~(Qt::WindowMinimized | Qt::WindowMaximized)) 
                | Qt::WindowFullScreen);
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        }

        QTimer::singleShot(0, this, SLOT(flashFileChangedSlot()));
        //_refreshBackground();

        if (!m_topMostTimer->isActive())
        {
            m_topMostTimer->start();
        }

        if (CConfigManager::isNoFullScreen() == false)
        {
            this->showFullScreen();
        }
        else
        {
            this->showNormal();
        }

        raise();
        activateWindow();
    }
    s_selfAdjusting = false;

    // force to hook mouse/keyboard event
    // in this case, the showEvent won't be triggered:
    // 1) open a new opentime session; 2) temp lock client;
    // 3) end session on server side; 4) this time the lock screen won't respond to input events
    // reason: in step 3, end session on server side, we do a) hide temp lock dialog; b) show lock screen;
    // the "showEvent" should be triggered in step b), but did not
    ThreadHookHelper::start();

    LOG_DEBUG << "full screen showEvent done";
}

void FullScreenDialog2::closeDialog()
{
    LOG_DEBUG4 << "fullSceen's closeDialog called";

    m_closeAction = DlgCloseAction_Close;
    {
        if (m_topMostTimer->isActive())
        {
            m_topMostTimer->stop();
        }

        this->hide();
    }

    m_closeAction = DlgCloseAction_NA;
}

void FullScreenDialog2::showEvent(QShowEvent *event)
{
    ThreadHookHelper::start();
}

void FullScreenDialog2::hideEvent( QHideEvent* event )
{
    ThreadHookHelper::stop();
}

void FullScreenDialog2::closeEvent( QCloseEvent* event )
{
    ThreadHookHelper::stop();
}

void FullScreenDialog2::makeSelfTopMostSlot()
{
    if (this->isVisible() == false)
    {
        LOG_DEBUG4 << "fullscreen dialog invisible, so won't make it to top window";
        return;
    }

#ifndef NDEBUG
    LOG_DEBUG4 << "Simulated action: pop fullscreen dialog to top window";
    return;
#endif

    //Log_Debug << "make fullscreen dialog on top";
    if (CConfigManager::isDeveloperEnv() == true)
    {
        LOG_DEBUG4 << "Simulated action: pop fullscreen dialog to top window";
        return;
    }

    if (CConfigManager::isNoFullScreen() == false)
    {
        QDesktopWidget* desktop = QApplication::desktop();
        QRect screenRect = desktop->screenGeometry(desktop->primaryScreen());
        if (s_lastDlgRect != screenRect)
        {
            s_lastDlgRect = screenRect;
            this->resize(screenRect.width(), screenRect.height());
            LOG_DEBUG << "Fullscreen dialog resizing to width: " << screenRect.width()
                << ". height: " << screenRect.height();
        }
    }

    //::SetWindowPos( this->effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    WId hWnd = this->effectiveWinId();
    if (::IsIconic(hWnd))
    {
        ::ShowWindow(hWnd,SW_RESTORE);
    }

    if (CConfigManager::isNoFullScreen() == false)
    {
        showFullScreen();
    }
    else
    {
        showNormal();
    }

    raise();
    activateWindow();
}

void FullScreenDialog2::_refreshBackground()
{
    // set swf path for display
    QString localFlashFilePath = FlashManager::GetInstance()->localFlashFilePath();
    if ( !QFile::exists(localFlashFilePath) )
    {
        FlashManager::GetInstance()->restoreDefaultFlashFile();
        return;
    }

    QString fileUri = LOCAL_URI_PREFIX + localFlashFilePath;

//#ifdef NDEBUG
    ui->remakeFlashWidget();
    ui->m_flashWidget->dynamicCall("LoadMovie(long,string)", 0, fileUri);
//#endif
}

void FullScreenDialog2::flashFileChangedSlot()
{
    _refreshBackground();
}

//
//bool FullScreenDialog2::eventFilter( QObject *obj, QEvent * e)
//{
//    // only do this in debug environment
//    // in debug env, when the dialog loses focus, stop the keyboard/mouse hook
//    if (CConfigManager::isDeveloperEnv())
//    {
//        int typeVal = e->type();
//        switch(e->type())
//        {
//        case QEvent::WindowActivate:
//            break;
//        case QEvent::WindowDeactivate:
//            break;
//        case QEvent::FocusIn:
//            ThreadHookHelper::start();
//            return true;
//        case QEvent::FocusOut:
//            ThreadHookHelper::stop();
//            return true;
//        }
//    }
//
//    // standard event processing
//    return QObject::eventFilter(obj, e);
//}
