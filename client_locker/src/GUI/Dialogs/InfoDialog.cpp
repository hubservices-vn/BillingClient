#include <QDesktopWidget>
#include "InfoDialog.h"
#include "ui_InfoDialog.h"
#include <QMessageBox>
#include <QtGui>
#include "log.h"
#include <QHostInfo>
#include "GuiController.h"
#include "ConfigManager.h"
#include "ThreadHookHelper.h"

InfoDialog::InfoDialog(QWidget* parent)
: QDialog(parent), ui(new Ui::InfoDialog), m_closeAction(DlgClose_NA)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //setParent(0); // Create TopLevel-Widget
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_PaintOnScreen);
    QString hostName = QHostInfo::localHostName();
    ui->clientNameLabel->setText(hostName);
    ui->clientNameLabel->installEventFilter(this);
    this->setModal(true);
}

InfoDialog::~InfoDialog()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }
}

void InfoDialog::changeEvent( QEvent *e )
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        retranslateUiEx();
        break;
    default:
        break;
    }
}

void InfoDialog::retranslateUiEx()
{
}

static QRect s_lastDlgRect;
static bool s_selfAdjusting = false;
void InfoDialog::showEvent( QShowEvent *event )
{
    LOG_DEBUG << "show fullscreen dialog";
    if (s_selfAdjusting)
    {
        return;
    }

    s_selfAdjusting = true;
    {
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

        if (CConfigManager::isNoFullScreen() == false)
        {
            this->showFullScreen();
        }
        else
        {
            this->showNormal();
        }
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


    LOG_DEBUG << "login dlg show";
    this->setUpdatesEnabled(false);
    {
        // calculate its location (pop on center of screen)
        QDesktopWidget* desktop = QApplication::desktop();
        QRect screenRect = desktop->screenGeometry(desktop->primaryScreen());
        QSize dlgSize = this->frameSize();
        int x = (screenRect.width() - dlgSize.width()) / 2;
        int y = (screenRect.height() - dlgSize.height()) / 2;;
        this->move(x, y);
    }
    this->setUpdatesEnabled(true);

    //QDialog::showEvent(event);
    ThreadHookHelper::start();
    LOG_DEBUG << "login dlg show done";
}

void InfoDialog::hideEvent( QHideEvent* event )
{
    ThreadHookHelper::stop();
}

void InfoDialog::closeEvent( QCloseEvent* event )
{
    ThreadHookHelper::stop();
}

void InfoDialog::keyPressEvent(QKeyEvent *event)
{
    GuiController::GetInstance()->popupLoginDialog();
}

void InfoDialog::mousePressEvent(QMouseEvent *event)
{
    GuiController::GetInstance()->popupLoginDialog();
}

bool InfoDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::MouseButtonPress) {
        GuiController::GetInstance()->popupLoginDialog();
        return true;
    }
    return QObject::eventFilter(object, event);
}