#include "LockOutSettingDialog.h"
#include <QMessageBox>
#include "Utils.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExp>
#include "ui_LockOutSettingDialog.h"
#include "ConfigManager.h"
#include "RegexpSettings.h"
#include "SystemManager.h"
#include "ScreenLockQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "cc2csNetworkManager.h"

LockOutSettingDialog::LockOutSettingDialog( QWidget *parent /* = 0*/ )
: QDialog(parent), ui(new Ui::LockOutSettingDialog)
{   
    ui->setupUi(this);

    {
        QRegExp rx(SIMPLE_PWD_REGEXP_PTN);
        QValidator *validator = new QRegExpValidator(rx, this);
        ui->pwdLineEdit->setValidator(validator);
        ui->pwdLineEdit->setPlaceholderText(SIMPLE_PWD_PLACEHOLDER);
        ui->pwdRptLineEdit->setValidator(validator);
    }

    ui->lockButton->setEnabled(false);

    //QString oldCode = CConfigManager::tempLockCode();
    //if (!oldCode.isEmpty())
    //{
    //    ui->pwdLineEdit->setText(oldCode);
    //    ui->pwdRptLineEdit->setText(oldCode);
    //    ui->lockButton->setEnabled(true);
    //}

    this->setModal(true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowSystemMenuHint);

    // listen to client service "lock reply"
    {
        SystemManager* mgr = SystemManager::GetInstance();
        connect(mgr, SIGNAL(screenLockReplyRecved(int,bool)),
            this, SLOT(screenLockReplyRecvedSlot(int,bool)));
    }
}

void LockOutSettingDialog::on_lockButton_clicked()
{
    ui->msgLabel->clear();
    if (ui->pwdLineEdit->text() != ui->pwdRptLineEdit->text())
    {
        ui->msgLabel->setText(tr("The passwords entered does NOT match"));
        return;
    }

    QString rawCode = ui->pwdLineEdit->text();
    if (cc2csNetworkManager::GetInstance()->isServiceConnected())
    {
        ScreenLockRequestQtModel qtModel(LockMode_Lock, rawCode);
        cc2csSender::send(cc2cs::ScreenLockCommandC(qtModel));
    }
    else
    {
        QString msg = tr("Client service is not running, try later");
        ui->msgLabel->setText(msg);
        return;
    }
}

void LockOutSettingDialog::on_cancelButton_clicked()
{
    this->done(0);
}

void LockOutSettingDialog::on_pwdLineEdit_textChanged( const QString & /*text*/ )
{
    _verifyPwds();
}

void LockOutSettingDialog::on_pwdRptLineEdit_textChanged( const QString & /*text*/ )
{
    _verifyPwds();
}

void LockOutSettingDialog::_verifyPwds()
{
    QString pwd = ui->pwdLineEdit->text();
    QString pwdRepeat = ui->pwdRptLineEdit->text();
    bool valid = (
        pwd.isEmpty() == false &&
        pwd == pwdRepeat);
    ui->lockButton->setEnabled(valid);
    if (pwd != pwdRepeat)
    {
        ui->msgLabel->setText(tr("The passwords entered does NOT match"));
    }
    else
    {
        ui->msgLabel->clear();
    }
}

void LockOutSettingDialog::screenLockReplyRecvedSlot( int modeVal, bool result )
{
    if (modeVal == (int)ScreenLockMode::LockMode_Lock)
    {
        if (result == true)
        {
            this->done(1);
        }
        else
        {
            ui->msgLabel->setText("Failed to lock screen. Try later");
        }
    }
}
