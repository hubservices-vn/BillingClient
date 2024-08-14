#include "TempLockDialog.h"
#include "ui_TempLockDialog.h"
//#include "Client.h"
#include "Utils.h"
#include <QRegExpValidator>
#include <QRegExp>
#include "CommonSettings.h"
#include "cl2csNetworkManager.h"
#include "RegexpSettings.h"
#include "ConfigManager.h"
#include "ScreenLockQtModel.h"
#include "cl2csSender.h"
#include "cl2csCmds.h"
#include "SystemManager.h"
#include "GuiController.h"
#include "ThreadHookHelper.h"

TempLockDialog::TempLockDialog(QWidget *parent) 
: QDialog(parent), ui(new Ui::TempLockDialog)
{
    ui->setupUi(this);

    {
        QRegExp rx(SIMPLE_PWD_REGEXP_PTN);
        QValidator *validator = new QRegExpValidator(rx, this);
        ui->pwdLineEdit->setValidator(validator);
    }

    this->setModal(true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);

    {
        // listen to unlock command reply
        SystemManager* mgr = SystemManager::GetInstance();
        connect(mgr, SIGNAL(screenUnlockReplyRecved(int,bool)),
            this, SLOT(screenUnlockReplyRecvedSlot(int,bool)));
    }
}

TempLockDialog::~TempLockDialog()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }
}

void TempLockDialog::on_unlockButton_clicked()
{
    QString tempPwd = ui->tempPwdLineEdit->text();
    if (tempPwd.isEmpty())
    {
        return;
    }

    ScreenUnlockRequestQtModel qtModel(LockMode_UnlockByTmpPwd, tempPwd);
    cl2csSender::send(cl2cs::ScreenUnlockRequestCommandC(qtModel));
}

void TempLockDialog::on_unlocByAccountkButton_clicked()
{
    QString userName = ui->userNameLineEdit->text();
    QString pwd = ui->pwdLineEdit->text();
    if (userName.isEmpty() || pwd.isEmpty())
    {
        return;
    }

    ScreenUnlockRequestQtModel qtModel(LockMode_UnlockByAcc, userName, pwd);
    cl2csSender::send(cl2cs::ScreenUnlockRequestCommandC(qtModel));
}

void TempLockDialog::showEvent( QShowEvent *event )
{
    ThreadHookHelper::stop();
    ui->tempPwdLineEdit->clear();
    ui->pwdLineEdit->clear();
    ui->msgLabel->clear();
    ui->userNameLineEdit->clear();

    ui->tabWidget->setCurrentIndex(0);

    QDialog::showEvent(event);
}

void TempLockDialog::hideEvent( QHideEvent *e )
{

}

void TempLockDialog::screenUnlockReplyRecvedSlot( int modeVal, bool result )
{
    ui->msgLabel->clear();

    if (modeVal == (int)ScreenLockMode::LockMode_UnlockByTmpPwd ||
        modeVal == (int)ScreenLockMode::LockMode_UnlockByAcc)
    {
        if (result == true)
        {
            this->done(1);
            GuiController::GetInstance()->unlockScreen();
        }
        else
        {
            ui->msgLabel->setText("wrong password");
        }
    }
}

void TempLockDialog::changeEvent( QEvent *e )
{
    QDialog::changeEvent(e);
    switch (e->type()) 
    {
    case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
            retranslateUiEx();
        }
        break;
    default:
        break;
    }
}

void TempLockDialog::retranslateUiEx()
{
}

