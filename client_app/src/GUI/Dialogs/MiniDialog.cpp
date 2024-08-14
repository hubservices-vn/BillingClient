#include "MiniDialog.h"
#include "ui_MiniDialog.h"

#include <QtGui>

#include "OrderDialog.h"
#include "Client.h"
#include "ChangePwdDialog.h"
#include "LockOutSettingDialog.h"
#include "Utils.h"
#include "CommonSettings.h"
#include "UserAccountManager.h"
#include "AdminOptionDialog.h"
#include "ConfigManager.h"
#include "ClientAppSettings.h"
#include "SoundManager.h"
#include "CcCore/CcMultiLangHelper.h"
#include "DateTimeUtils.h"
#include "cc2csNetworkManager.h"
#include "NetworkStatus.h"
#include "UserAccount.h"
#include "SystemTray.h"
#include "SessionManager.h"
#include "ui_TextMessageDialog.h"
#include "TextMessageManager.h"
#include "ChangePwdDialog.h"
#include "AdminOptionDialog.h"
#include "LockOutSettingDialog.h"
#include "TopupByCouponDialog.h"
#include "UserLogoutQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "GuiController.h"
#include "ClientConnectionQtModel.h"
#include "log.h"
#include "SecurityProfileManager.h"
#include "ClientSettingManager.h"
#include "SysInfoHelper.h"
#include "CcCore/CcMultiLangHelper.h"
#include "ClientAdManager.h"
#include <QDesktopServices>
#include "ClientAdManager.h"
#include <QDesktopServices>
// if user has no more than 30 minutes left, then notify him
const int MINS_TO_NOTIFY = 30;
// those "must have" ones (10, 5, 3) should NOT be removed/changed, 
// as we'll play sound when they triggers
const QList<int> OutOfTimeThreshold = 
    QList<int>() << 30 << 15 << 10/*must have*/ 
    << 7 << 5/*must have*/ 
    << 3/*must have*/ << 2 << 1;

bool MiniDialog::s_isAlive = false;

MiniDialog::MiniDialog(QWidget* parent) 
: QDialog(parent), ui(new Ui::MiniDialog), m_closeAction(DlgCloseAction_NA)
{
    m_sessionMgr = SessionManager::GetInstance();
    m_networkMgr = cc2csNetworkManager::GetInstance();

    // init on demand
    m_systemTray = NULL;
    m_movie = NULL;
    ui->setupUi(this);

    this->setMinimumWidth(300);
    this->setMaximumWidth(300);
    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->window()->layout()->setSizeConstraint(QLayout::SetFixedSize);

    init();

    // init languages
    {
        ui->langComboBox->addItem(tr("Vietnamese"), "vi_VN");
        ui->langComboBox->addItem(tr("English"), "en_US");
        ui->langComboBox->addItem(tr("Bahasa"), "id_ID");
        ui->langComboBox->addItem(tr("Thai"), "th_TH");


        QString lang = CcMultiLangHelper::currentLang();
        if(lang == "vi_VN")
        {
            ui->langComboBox->setCurrentIndex(0);
        }
        else if(lang == "id_ID")
        {
            ui->langComboBox->setCurrentIndex(2);
        }
        else if(lang == "th_TH")
        {
            ui->langComboBox->setCurrentIndex(3);
        }
        else
        {
            ui->langComboBox->setCurrentIndex(1);
        }
    }

    connectSigSlot();

    s_isAlive = true;
}

MiniDialog::~MiniDialog()
{
    if (m_movie != NULL)
    {
        m_movie->deleteLater();
        m_movie = NULL;
    }
    if (ui != NULL)
    {
        delete ui;
        ui = NULL;
    }
}

bool MiniDialog::init()
{
    _initContextMenu();

    // init the ribbon buttons
    {
        ui->sendMessageButton->init(
            ":/icons/icons/MiniWindow/home-send-msg.png",
            tr("Message"));

        ui->changePwdButton->init(
            ":/icons/icons/MiniWindow/home-change-password.png",
            tr("Password"));

        ui->orderButton->init(
            ":/icons/icons/MiniWindow/home-order.png",
            tr("Order"));

        ui->lockButton->init(
            ":/icons/icons/MiniWindow/home-lock-pc.png",
            tr("LockPC"));

        ui->utilitiesButton->init(
            ":/icons/icons/MiniWindow/home-utilities.png",
            tr("Utility"),
            RibBtnMode_Context);

        ui->logoutButton->init(
            ":/icons/icons/MiniWindow/home-logout.png",
            tr("Logout"));
			
		//// TEMPORARY HIDE THIS BUTTON.
  //      ui->topupByCouponButton->setVisible(false);
  //      ui->topupByCouponButton->init(
  //          ":/icons/icons/MiniWindow/home-topup.png",
  //          tr("Topup"));
    }
    return true;
}

bool MiniDialog::connectSigSlot()
{
    {
        // the time passes, update the "time used" and "time left" fields
        connect(m_sessionMgr, SIGNAL(sessionUpdated()), 
            this, SLOT(sessionUpdatedSlot()));

        // the session ended, reset minidialog UI
        connect(m_sessionMgr, SIGNAL(sessionEnded()), 
            this, SLOT(sessionEndedSlot()));
    }

    // the connections breaks, disable the "send message", "order product", "change password" buttons
    {
        cc2csNetworkManager* mgr = cc2csNetworkManager::GetInstance();
        connect(mgr, SIGNAL(networkStateChanged(int)),
            this, SLOT(networkStateChangedSlot(int)));
    }

    // the user requests to change password and get response from server
    {
        UserAccountManager* mgr = UserAccountManager::GetInstance();
        connect(mgr, SIGNAL(changeAccPwdReplyRecved(int)),
            this, SLOT(changeAccPwdReplyRecvedSlot(int)));
        connect(mgr, SIGNAL(void accLoggedIn()),
            this, SLOT(onClientAdFileChangedSlot()));
    }

    // client service is about to shutdown computer, warn user
    {
        ClientSettingManager* mgr = ClientSettingManager::GetInstance();
        connect(mgr, SIGNAL(aboutToShutdownComputer(QTime)),
            this, SLOT(aboutToShutdownComputerSlot(QTime)));
    }

    {
        connect(ui->defaultAdLabel, SIGNAL(clicked()),
            this, SLOT(on_defaultAdLabel_clicked()));
        ClientAdManager* mgr = ClientAdManager::GetInstance();
        connect(mgr, SIGNAL(clientAdFileChanged()),
            this, SLOT(onClientAdFileChangedSlot()));
        onClientAdFileChangedSlot();
    }

    return true;
}

void MiniDialog::changeEvent( QEvent *e )
{
    QDialog::changeEvent(e);
    switch (e->type()) 
    {
    case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
            retranslateUiEx();
            // useful for system tray, which doesn't get a changeEvent for re-implementation
            emit languageChanged();
        }
        break;
    default:
        break;
    }
}

void MiniDialog::retranslateUiEx()
{
    _displayConnectionStatus();

    ui->sendMessageButton->setBtmText(tr("Message"));
    ui->changePwdButton->setBtmText(tr("Password"));
    ui->orderButton->setBtmText(tr("Order"));
    ui->lockButton->setBtmText(tr("LockPC"));
    ui->utilitiesButton->setBtmText(tr("Utility"));
    ui->logoutButton->setBtmText(tr("Logout"));

    ui->optionButton->setText(tr("Options"));
    ui->exitButton->setText(tr("Exit"));
    // TODO:
    //_updateSessionStatus();
    m_adjustSoundAction->setText(tr("Sound"));
    m_adjustMouseAction->setText(tr("Mouse"));
    m_adjustDisplayAction->setText(tr("Display"));

    ui->langComboBox->setItemText(0, tr("Vietnamese"));
    ui->langComboBox->setItemData(0, "vi_VN");
    ui->langComboBox->setItemText(1, tr("English"));
    ui->langComboBox->setItemData(1, "en_US");
    ui->langComboBox->setItemText(2, tr("Bahasa"));
    ui->langComboBox->setItemData(2, "id_ID");
    ui->langComboBox->setItemText(3, tr("Thai"));
    ui->langComboBox->setItemData(3, "th_TH");

}


void MiniDialog::keyPressEvent( QKeyEvent* event )
{
    if (event->key() == Qt::Key_Escape)
    {
        return;
    }

    if (event->key() == Qt::Key_F4)
    {
        if (event->modifiers() & Qt::AltModifier)
        {
            return;
        }
    }
}

void MiniDialog::closeEvent( QCloseEvent* event )
{
    // this close event is triggered by the "close" button 
    // on top-right corner of mini dialog
    if (m_closeAction == DlgCloseAction_Hide)
    {
        // show tray icon
        _getSystemTray()->show();

        this->hide();
        event->ignore();
        m_closeAction = DlgCloseAction_NA;
        return;
    }
    // this close event is triggered by the OS shutdown/reboot
    // so if we ignore that, the computer has no way to shutdown
    else
    {
        event->accept();
    }
}

void MiniDialog::sessionUpdatedSlot()
{
    if (m_sessionMgr->session() == NULL)
    {
        ui->sysMsgTextLabel->clear();
        return;
    }

    // refresh the "total minutes", "minutes used", "minutes left" fields
    _updateSessionStatus();

    int minutesLeft = m_sessionMgr->minutesLeft();
    if (minutesLeft > MINS_TO_NOTIFY)
    {
        ui->sysMsgTextLabel->clear();
        _getSystemTray()->clearMessage();
    }

    // check whether it's time to notify user the remaining minutes (<< 30 << 15 << 10 << 6 << 4 << 3 << 2 << 1;)
    if (OutOfTimeThreshold.contains(minutesLeft))
    {
        int displayArea = MessageDisplay_TextBox;
        MessageLevel level = MessageLevel_Info;
        if (minutesLeft > 10)
        {
            // do nothing, just use the default setting
        }
        else if(minutesLeft == 10)
        {
            SoundManager::play("sound/10.wav");
        }
        else if (minutesLeft >= 5)
        {
            level = MessageLevel_Warn;
            if(minutesLeft == 5)
            {
                SoundManager::play("sound/05.wav");
            }
        }
        else if (minutesLeft > 0)
        {
            if(minutesLeft == 3)
            {
                SoundManager::play("sound/03.wav");
            }
            level = MessageLevel_Error;
        }

        QString msg = tr("Session ends in %1 minutes.").arg(minutesLeft);
        showMessage(msg, level);
        _getSystemTray()->showMessage(msg, level);
    }
}

void MiniDialog::_updateSessionStatus()
{
    int totalMinutes = m_sessionMgr->totalMinutes();
    int minutesUsed = m_sessionMgr->minutesUsed();
    int minutesLeft = m_sessionMgr->minutesLeft();
    double timeFee = m_sessionMgr->timeFee();
    double serviceFee = m_sessionMgr->serviceFee();
    ui->totalTimeLabel->setText(DateTimeUtils::minuteToEditableTime(totalMinutes));
    ui->timeUsedTextLabel->setText(DateTimeUtils::minuteToEditableTime(minutesUsed));
    ui->timeLeftTextLabel->setText(DateTimeUtils::minuteToEditableTime(minutesLeft));
    ui->timeFeeTextLabel->setText(Utils::formatDisplayMoney(timeFee));
    ui->serviceFeeTextLabel->setText(Utils::formatDisplayMoney(serviceFee));
}

void MiniDialog::_displayConnectionStatus()
{
    NetworkResult connState = cc2csNetworkManager::GetInstance()->finalNetworkState();

    QString connStateText;
    QString connIconFileName;
    QString textColorStr;
    if (connState == NetworkResult_Connected)
    {
        connStateText = tr("Connected");
        connIconFileName = "";//"connection-green.png";
        textColorStr = "#66cc44";
    }
    else if (connState == NetworkResult_Connecting)
    {
        connStateText = tr("Connecting");
        connIconFileName = "connection-yellow.png";
        textColorStr = "#ffff00";
    }
    else
    {
        connStateText = tr("Disconnected");
        connIconFileName = "connection-red.png";
        textColorStr = "#ff6600";
    }

    // set display text
    ui->connStatusTextLabel->setText(connStateText);
    QString cssStr = QString("QLabel {color: %1;}").arg(textColorStr);
    ui->connStatusTextLabel->setStyleSheet(cssStr);
    // update icon
    QString connIconFullPath = ":/icons/icons/MiniWindow/" + connIconFileName;
    ui->connStatusIconLabel->setPixmap(QPixmap(connIconFullPath));
    _updateButtonStatus();
}

void MiniDialog::on_logoutButton_clicked()
{
    LOG_DEBUG << "User clicks Logout";

    // send to client service for log out request
    UserLogoutQtModel qtModel(
        CConfigManager::clientId(), 
        UserAccountManager::currentUserId(), 
        UserAccountManager::currentUserType());
    cc2csSender::send(cc2cs::UserLogoutCommandC(qtModel));

    //// running in offline mode, and even client service is not connected, so directly lock the client
    //// (if server is NOT connected but client service is connected, need to wait for service's response
    //// to lock the UI)
    //if (cc2csNetworkManager::GetInstance()->isServiceConnected() == false)
    //{
    //    GuiController::GetInstance()->lockClient();
    //}
}

void MiniDialog::on_sendMessageButton_clicked()
{
    TextMessageManager::GetInstance()->showDialog();
}

void MiniDialog::on_orderButton_clicked()
{
    OrderDialog dlg(this);
    int ret = dlg.exec();
}

void MiniDialog::on_changePwdButton_clicked()
{
    ChangePwdDialog dlg(this);
    int ret = dlg.exec();
}

void MiniDialog::on_lockButton_clicked()
{
    LockOutSettingDialog dlg(this);
    int ret = dlg.exec();
    Q_UNUSED(ret);
}

void MiniDialog::showEvent( QShowEvent *event )
{
    LOG_DEBUG << "show mini dialog";

    this->setUpdatesEnabled(false);
    {
        ui->userNameLabel->setText(UserAccountManager::currentUserName());

        _displayConnectionStatus();

        _updateButtonStatus();

        _relocateDialog();
    }
    this->setUpdatesEnabled(true);

    // since dialog is visible, reset the flag
    m_closeAction = DlgCloseAction_NA;
}

void MiniDialog::changeAccPwdReplyRecvedSlot( int resultVal )
{
    QString resultStr;
    QString textColor;
    ChangeAccPwdResult result = (ChangeAccPwdResult)resultVal;
    if (result == ChangeAccPwd_OK)
    {
        resultStr = tr("Change password succeeded.");
        textColor = "black";
    }
    else if (result == ChangeAccPwd_NoSuchUser)
    {
        resultStr = tr("Change password failed. Reason: No such user.");
        textColor = "red";
    }
    else if (result == ChangeAccPwd_WrongPwd)
    {
        resultStr = tr("Change password failed. Reason: Wrong password.");
        textColor = "red";
    }
    else if (result == ChangeAccPwd_Failed)
    {
        resultStr = tr("Change password failed Reason: server error.");
        textColor = "red";
    }

    addTextToMsgArea(resultStr, textColor);
}

void MiniDialog::addTextToMsgArea( const QString& msg, const QString &textColor )
{
    //// remember the old text format
    QString css = QString("QLabel {color: %1;}").arg(textColor);

    // create the new text format with specified color
    ui->sysMsgTextLabel->setStyleSheet(css);
    ui->sysMsgTextLabel->setText(msg);

    //// resume the old color
    //ui->dynamicMsgTextEdit->setTextColor(oldColor);
}


void MiniDialog::on_topupByCouponButton_clicked()
{
    TopupByCouponDialog dlg(this);
    dlg.exec();
}

void MiniDialog::on_optionButton_clicked()
{
    AdminOptionDialog dlg(this);
    dlg.exec();
}

void MiniDialog::on_exitButton_clicked()
{
    //// send to client service for log out request
    {
        UserLogoutQtModel qtModel;
        cc2csSender::send(cc2cs::UserLogoutCommandC(qtModel));
    }

    {
        EndConnectionRequestQtModel qtModel(CConfigManager::clientId());
        cc2csSender::send(cc2cs::EndConnectionRequestCommandC(qtModel));
    }

    // re-enable all security settings
    SecurityProfileManager::GetInstance()->clientEndedConnectionSlot();

    // close the application
    QApplication::closeAllWindows();
    QApplication::quit();
}

void MiniDialog::on_allowTrayMsgCheckBox_stateChanged()
{
    bool allowed = ui->allowTrayMsgCheckBox->isChecked();
    CConfigManager::setShowTrayMsg(allowed);
}

void MiniDialog::showMessage(const QString& msg, MessageLevel level)
{
    QString textColor = "black";
    switch(level)
    {
    case MessageLevel_Info:
        textColor = "black";
        break;
    case MessageLevel_Warn:
        textColor = "rgba(255, 106, 0, 255)"; // similar with "dark yellow"
        break;
    case MessageLevel_Error:
        textColor = "red";
        break;
    }
    addTextToMsgArea(msg, textColor);
}

void MiniDialog::_updateButtonStatus()
{
    bool connected = cc2csNetworkManager::GetInstance()->isServerConnected();
    bool hasUser = false;
    bool isAdmin = false;
    bool isGuest = false;
    UserAccount* user = UserAccountManager::GetInstance()->CurrentUser();

    if (user != NULL)
    {
        hasUser = true;
    }

    if (user != NULL &&
        user->accType() == UserGroup_LocalSuperAdmin)
    {
        isAdmin = true;
    }

    if (user != NULL &&
        user->accType() == UserGroup_Guest)
    {
        isGuest = true;
    }

    if (connected == false || hasUser == false)
    {
        ui->sendMessageButton->setEnabled(false);
        ui->changePwdButton->setEnabled(false);
        ui->orderButton->setEnabled(false);
    }
    // connected to server
    else
    {
        ui->sendMessageButton->setEnabled(true);
        ui->changePwdButton->setEnabled(!(isAdmin || isGuest));
        ui->orderButton->setEnabled(!isAdmin);
        ui->logoutButton->setEnabled(!isGuest);
    }

    // toggle the admin-specific buttons
    //(only available when admin)
    ui->optionButton->setEnabled(isAdmin);
    ui->optionButton->setVisible(isAdmin);
    ui->exitButton->setEnabled(isAdmin);
    ui->exitButton->setVisible(isAdmin);

    ui->timeFeeTitleLabel->setEnabled(!isAdmin);
    ui->timeFeeTitleLabel->setVisible(!isAdmin);
    ui->timeFeeTextLabel->setEnabled(!isAdmin);
    ui->timeFeeTextLabel->setVisible(!isAdmin);

    ui->serviceFeeTitleLabel->setEnabled(!isAdmin);
    ui->serviceFeeTitleLabel->setVisible(!isAdmin);
    ui->serviceFeeTextLabel->setEnabled(!isAdmin);
    ui->serviceFeeTextLabel->setVisible(!isAdmin);
}

void MiniDialog::on_langComboBox_currentIndexChanged( int index )
{
    if (index >= 0 &&
        index < ui->langComboBox->count())
    {
        QVariant var = ui->langComboBox->itemData(index);
        QString langStr = var.toString();
        CcMultiLangHelper::setLanguage(langStr);

        // the dialog size might change
        _relocateDialog();
    }
    else
    {
        QString msg = tr("Can't find the language settings.");
        QMessageBox::critical(this, tr("Error"), msg);
    }
}

void MiniDialog::on_minimizeButton_clicked()
{
    m_closeAction = DlgCloseAction_Hide;
    this->close();
}

void MiniDialog::networkStateChangedSlot( /*NetworkResult*/int result )
{
    // update the status display and enable/disable some buttons
    _displayConnectionStatus();
}

void MiniDialog::sessionEndedSlot()
{
    // account logged out, clear the historical temp unlock code
    CConfigManager::setInTempLockMode(false);
    CConfigManager::setTempLockCode("");

    // clear UI data
    ui->totalTimeLabel->clear();
    ui->timeUsedTextLabel->clear();
    ui->timeLeftTextLabel->clear();
    ui->connStatusTextLabel->clear();
    ui->sysMsgTextLabel->clear();
}

SystemTray* MiniDialog::_getSystemTray()
{
    if (m_systemTray == NULL)
    {
        LOG_DEBUG << "init system tray";
        m_systemTray = new SystemTray(this);
    }

    return m_systemTray;
}

void MiniDialog::aboutToShutdownComputerSlot( const QTime &autoShutdownTime )
{
    if ( ! autoShutdownTime.isValid())
    {
        return;
    }

    QString timeStr = tr(". Shutdown at:%1.").arg(DateTimeUtils::timeToString(autoShutdownTime));
    QString msg = ClientSettingManager::GetInstance()->autoShutdownMsg() + timeStr;
    showMessage(msg, MessageLevel_Warn);
    _getSystemTray()->showMessage(msg, MessageLevel_Warn);
}

void MiniDialog::_relocateDialog()
{
    // calculate its location (dock on the top-right corner)
    QDesktopWidget* desktop = QApplication::desktop();
    QRect screenRect = desktop->availableGeometry(desktop->primaryScreen());
    QSize miniDlgSize = this->frameSize();
    int x = screenRect.width() - miniDlgSize.width();
    int y = 4;
    this->move(x, y);
}

void MiniDialog::on_utilitiesButton_clicked()
{
    QPoint globalPos = QCursor::pos();
    m_utilitiesContextMenu->exec(globalPos);
}

void MiniDialog::_initContextMenu()
{
    m_utilitiesContextMenu = new QMenu();

    m_adjustSoundAction = m_utilitiesContextMenu->addAction(tr("Sound"),
        this, SLOT(adjustSoundSlot()));

    m_adjustMouseAction = m_utilitiesContextMenu->addAction(tr("Mouse"),
        this, SLOT(adjustMouseSlot()));

    m_adjustDisplayAction = m_utilitiesContextMenu->addAction(tr("Display"),
        this, SLOT(adjustDisplaySlot()));

    ui->utilitiesButton->setMenu(m_utilitiesContextMenu);
}

void MiniDialog::adjustSoundSlot()
{
    LOG_DEBUG << "Adjust sound";
    QString rundll32Path = SysInfoHelper::system32Folder() + "\\" + "control.exe";
    QStringList params; 
    params << "mmsys.cpl";
    QProcess::startDetached(rundll32Path, params);
}

void MiniDialog::adjustMouseSlot()
{
    LOG_DEBUG << "Adjust mouse";
    QString rundll32Path = SysInfoHelper::system32Folder() + "\\" + "control.exe";
    QStringList params; 
    params << "main.cpl";
    QProcess::startDetached(rundll32Path, params);
}

void MiniDialog::adjustDisplaySlot()
{
    LOG_DEBUG << "Adjust display";
    QString rundll32Path = SysInfoHelper::system32Folder() + "\\" + "control.exe";
    QStringList params; 
    params << "desk.cpl";
    QProcess::startDetached(rundll32Path, params);
}

void MiniDialog::onClientAdFileChangedSlot()
{
    QIcon icon;
    QString iconFile = ClientAdManager::GetInstance()->localClientAdFilePath();
    QString adUrl = ClientAdManager::GetInstance()->adUrl();
    if (iconFile.length() > 0)
    {
        icon = QIcon(iconFile);
        if (icon.pixmap(QSize(205, 115)).isNull())
        {
            //??? dont change the icon then
        } else {
            if (m_movie != NULL)
            {
                m_movie->stop();
                m_movie->deleteLater();
                m_movie = NULL;
            }
            m_movie = new QMovie(iconFile);
            ui->defaultAdLabel->setMovie(m_movie);
            m_movie->start();
            //setPixmap(icon.pixmap(QSize(205, 115)));
        }
    }
}
void MiniDialog::on_defaultAdLabel_clicked()
{
    QString adUrl = ClientAdManager::GetInstance()->adUrl();
    if (adUrl.length() > 0)
    {
        QDesktopServices::openUrl(QUrl(adUrl));
    }
}
