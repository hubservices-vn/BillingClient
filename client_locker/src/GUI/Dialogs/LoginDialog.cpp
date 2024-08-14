#include <QDesktopWidget>
#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include <QMessageBox>
#include <QtGui>
#include "ConfigManager.h"
#include "GUpperCaseValidator.h"
#include "CommonSettings.h"
#include "RegexpSettings.h"
#include "cl2csSender.h"
#include "cs2ssCmds.h"
#include "Utils.h"
#include "ThreadHookHelper.h"
#include "UserAccountManager.h"
#include "ClientCommonSettings.h"
#include "UserLoginQtModel.h"
#include "cl2csCmds.h"
#include "log.h"


LoginDialog::LoginDialog(QWidget* parent) 
: QDialog(parent), ui(new Ui::LoginDialog), m_closeAction(DlgClose_NA)
{
    ui->setupUi(this);

    connectSigSlot();

    // set the fore color to red
    ui->failureReasonLabel->setStyleSheet("color: rgb(255, 0, 0);");

    ui->userNameLineEdit->setMaxLength(MAX_USERNAME_LENGTH);
    ui->userPwdLineEdit->setMaxLength(MAX_USERPWD_LENGTH);

    {
        QRegExp rx(USER_NAME_REGEXP_PTN);
        QValidator *validator = new GUpperCaseValidator(rx, this);
        ui->userNameLineEdit->setValidator(validator);
    }

    {
        QRegExp rx(PWD_REGEXP_PTN);
        QValidator *validator = new QRegExpValidator(rx, this);
        ui->userPwdLineEdit->setValidator(validator);
        ui->userPwdLineEdit->setPlaceholderText(PWD_PLACEHOLDER);
    }

    this->setModal(true);

    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);

    this->setWindowTitle(G_DISPLAY_SOFTWARE);
}

LoginDialog::~LoginDialog()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }
}

bool LoginDialog::connectSigSlot()
{
    // listen to the user login reply event
    {
        UserAccountManager* userAccMgr = UserAccountManager::GetInstance();
        connect(userAccMgr, SIGNAL(loginReplyRecved(int,int)),
            this, SLOT(loginReplyRecvedSlot(int,int)));
    }

    return true;
}

void LoginDialog::changeEvent( QEvent *e )
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

void LoginDialog::retranslateUiEx()
{
}

void LoginDialog::showEvent( QShowEvent *event )
{
    LOG_DEBUG << "login dlg show";
    this->setUpdatesEnabled(false);
    {
        ui->userNameLineEdit->clear();
        ui->failureReasonLabel->clear();
        ui->userPwdLineEdit->clear();

        ui->userNameLineEdit->setFocus();

        // calculate its location (pop on center of screen)
        QDesktopWidget* desktop = QApplication::desktop();
        QRect screenRect = desktop->screenGeometry(desktop->primaryScreen());
        QSize dlgSize = this->frameSize();
        int x = (screenRect.width() - dlgSize.width()) / 2;
        int y = (screenRect.height() - dlgSize.height()) / 2;;
        this->move(x, y);
    }
    this->setUpdatesEnabled(true);

    QDialog::showEvent(event);
    ThreadHookHelper::stop();
    LOG_DEBUG << "login dlg show done";
}

void LoginDialog::hideEvent(QHideEvent*)
{
    LOG_DEBUG << "Login dlg hiding";
    if (m_closeAction != DlgClose_LoginSucceeded)
    {
        ThreadHookHelper::start();
    }
    LOG_DEBUG << "Login dlg hidden";
}

void LoginDialog::closeEvent( QCloseEvent *e )
{
    LOG_DEBUG << "Login dlg closing";
    if (m_closeAction != DlgClose_LoginSucceeded)
    {
        ThreadHookHelper::start();
    }
    LOG_DEBUG << "Login dlg closed";
}

void LoginDialog::on_userLoginButton_clicked()
{
    QString userName = ui->userNameLineEdit->text().toUpper();
    QString rawPwd = ui->userPwdLineEdit->text();
    
    // user must fill in both user name and password
    if (userName.isEmpty() ||
        rawPwd.isEmpty())
    {
        return;
    }
    
    if (cl2csSocketManager::GetInstance()->isSocketConnected() == false)
    {
        QString pwdHash = Utils::hashUserData(userName, rawPwd, HashType_GCB_MD5);
        if (QString::compare(userName, CConfigManager::adminNameRaw(), Qt::CaseInsensitive) == 0 &&
            QString::compare(pwdHash, CConfigManager::adminCodeHash()) == 0)
        {
            QString questionMsg = tr("Client service is NOT running. Close Client UI?");
            if (QMessageBox::Yes == QMessageBox::question(this, tr("confirm"), questionMsg, 
                QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel))
            {
                LOG_INFO << "clientsvc not running, closed by admin";
                exit(0);
                return;
            }
        }

        // user either clicks "cancel" or enters wrong admin account, prompt error and return
        {
            QString msg = tr("Client service is NOT running");
            ui->failureReasonLabel->setText(msg);
            return;
        }
    }

    UserLoginRequestQtModel qtModel;
    qtModel.ClientId = CConfigManager::clientId();
    qtModel.UserName = userName;
    qtModel.RawPwd = rawPwd;
    cl2csSender::send(cl2cs::UserLoginCommandC(qtModel));

    // post event, clear the password text field, to prevent user keep clicking login repeatedly
    ui->userPwdLineEdit->clear();
}

void LoginDialog::on_closeButton_clicked()
{
    this->close();
}

//void LoginDialog::keyPressEvent( QKeyEvent* event )
//{
//    if (event->key() == Qt::Key_Escape)
//    {
//        this->close();
//        return;
//    }
//
//    if ((event->key() == Qt::Key_F4) &&
//        (event->modifiers() & Qt::AltModifier) != 0)
//    {
//        this->close();
//        return;
//    }
//
//    QDialog::keyPressEvent(event);
//}

void LoginDialog::loginReplyRecvedSlot(/*LoginResult*/int resultVal, int accTypeVal)
{
    QString status;

    UserGroupType accType = (UserGroupType)accTypeVal;
    LoginResult loginResult = (LoginResult)resultVal;

    if (accType == UserGroup_Guest)
    {
        loginResult = LoginResult_NoUser;
    }

    switch(loginResult)
    {
    case LoginResult_OK:
        status = QString();
        break;
    case LoginResult::LoginResult_AlreadyLoggedIn :
        status = tr("The account has already logged in from another computer.");
        break;
    case LoginResult::LoginResult_Locked :
        status = tr("The account is locked.");
        break;
    case LoginResult::LoginResult_NoUser :
        status = tr("Wrong username or password.");
        break;
    case LoginResult::LoginResult_UserRemoved :
        status = tr("The user account has been deleted.");
        break;
    case LoginResult::LoginResult_WrongPwd :
        status = tr("Wrong password. Please try again.");
        break;
    case LoginResult::LoginResult_NoMoney :
        status = tr("Not enough money. Please topup some money.");
        break;
    case LoginResult::LoginResult_Expired :
        status = tr("This account has expired. Please contact administrator.");
        break;
    case LoginResult::LoginResult_InTimeControl :
        status = tr("This account is under 'Time Control'. Please contact administrator.");
        break;
    default:
        status = tr("Unknown error. Code: ").arg(QString::number((int)loginResult));
        break;
    }

    ui->failureReasonLabel->setText(status);
}

void LoginDialog::closeDialog()
{
    m_closeAction = DlgClose_LoginSucceeded;
    {
        this->done(1);
    }
    m_closeAction = DlgClose_NA;
}
