#include "AdminOptionDialog.h"
#include "ui_AdminOptionDialog.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExp>
#include "GUpperCaseValidator.h"

#include <QMessageBox>
#include "ConfigManager.h"
#include "cc2csSocketManager.h"
#include "Utils.h"
#include "RegexpSettings.h"
#include "CommonSettings.h"
#include "cc2csNetworkManager.h"
#include "AdminOptionQtModelC.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "AdminOptionManager.h"
#include "cc2csNetworkManager.h"

const QString IP_SEPARATOR = ".";

AdminOptionDialog::AdminOptionDialog(QWidget *parent) 
: QDialog(parent), ui(new Ui::AdminOptionDialog)
{
    ui->setupUi(this);

    {
        QRegExp rx(USER_NAME_REGEXP_PTN);
        QValidator *validator = new GUpperCaseValidator(rx, this);
        ui->loginNameLineEdit->setValidator(validator);
    }

    {
        QRegExp rx(PWD_REGEXP_PTN);
        QValidator *validator = new QRegExpValidator(rx, this);
        ui->newPwdLineEdit->setValidator(validator);
        ui->newPwdLineEdit->setPlaceholderText(PWD_PLACEHOLDER);
        ui->confirmPwdLineEdit->setValidator(validator);
    }

    // fill in the user name
    QString oldUserName = CConfigManager::adminNameRaw();
    ui->loginNameLineEdit->setText(oldUserName.toUpper());

    // fill the IP address
    ui->qcIPWidget->setIPAddress(CConfigManager::serverAddress());

    // update connection status
    displayConnectionStatusSlot();

    // connect sig/slots
    // the connections breaks/established, change the message
    {
        cc2csNetworkManager* mgr = cc2csNetworkManager::GetInstance();
        connect(mgr, SIGNAL(serverConnected()),
            this, SLOT(displayConnectionStatusSlot()));
        connect(mgr, SIGNAL(serverDisconnected()),
            this, SLOT(displayConnectionStatusSlot()));
    }

    {
        AdminOptionManager* mgr = AdminOptionManager::GetInstance();
        connect(mgr, SIGNAL(adminOptionReplyRecved(int,int)),
            this, SLOT(adminOptionReplyRecvedSlot(int,int)));
    }
}

AdminOptionDialog::~AdminOptionDialog()
{
    delete ui;
}

void AdminOptionDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AdminOptionDialog::on_changeButton_clicked()
{
    bool valid = false;
    QString ipAddr = ui->qcIPWidget->ipAddress(valid);

    if (valid == false)
    {
        QString msg = tr("All the 4 sections of Server IP should be filled with proper values");
        QMessageBox::critical(this, tr("error"), msg);
        return;
    }

    AdminOptionRequestQtModelC qtModel;

    int opTypeVal = AdminOp_NA;
    if (QString::compare(ipAddr, CConfigManager::serverAddress(), Qt::CaseInsensitive) != 0)
    {
        opTypeVal |= AdminOp_ServerAddress;
        qtModel.ServerAddress_ = ipAddr;
    }

    if (ui->adminAccountGroupBox->isChecked() == true)
    {
        if (!cc2csNetworkManager::GetInstance()->isServerConnected())
        {
            QString msg = tr("Please connect to server before changing admin password");
            QMessageBox::critical(this, tr("error"), msg);
            return;
        }
        QString newAdminPwdRaw = ui->newPwdLineEdit->text();
        QString confirmPwdRaw = ui->confirmPwdLineEdit->text();
        // make sure the 2 pwds entered are the same
        if (newAdminPwdRaw != confirmPwdRaw)
        {
            QString msg = tr("The passwords entered are NOT the same");
            QMessageBox::critical(this, tr("error"), msg);
            return;
        }

        QString newAdminNameRaw = ui->loginNameLineEdit->text().toUpper();
        QString oldPwdRaw = ui->oldPwdLineEdit->text();

        opTypeVal |= AdminOp_LocalAdminAcc;
        qtModel.NewAdminName_ = newAdminNameRaw;
        qtModel.OldAdminPwdRaw_ = oldPwdRaw;
        qtModel.NewAdminPwdRaw_ = newAdminPwdRaw;
    }

    qtModel.OpType_ = opTypeVal;
    cc2csSender::send(cc2cs::AdminOptionRequestCommand(qtModel));
}

void AdminOptionDialog::on_closeButton_clicked()
{
    this->done(0);
}

void AdminOptionDialog::displayConnectionStatusSlot()
{
    if (true == cc2csNetworkManager::GetInstance()->isServerConnected())
    {
        ui->staticMessageLabel->setText(tr("Connected."));
        ui->staticMessageLabel->setStyleSheet("color: rgb(0, 85, 0);"); // dark green
    }
    else
    {
        ui->staticMessageLabel->setText(tr("Disconnected"));
        ui->staticMessageLabel->setStyleSheet("color: red;");
    }
}

void AdminOptionDialog::adminOptionReplyRecvedSlot( int opTypeVal, int resultVal )
{
    if (opTypeVal == (int)AdminOp_LocalAdminAcc)
    {
        if (resultVal == (int)AdminOpResult_WrongNamePwd)
        {
            QString msg = tr("The old password entered is wrong.");
            QMessageBox::information(this, tr("Error"), msg);
            //QString msg = tr("The old username/password entered is wrong.");
            //ui->staticMessageLabel->setText(msg);
            //ui->staticMessageLabel->setStyleSheet("color: red;");
        }
        else if(resultVal == (int)AdminOpResult_OK)
        {
            QString msg = tr("The account has been updated.");
            QMessageBox::information(this, tr("Notification"), msg);

            //QString msg = tr("Change local admin account succeeded");
            //ui->staticMessageLabel->setText(msg);
            //ui->staticMessageLabel->setStyleSheet("color: rgb(0, 85, 0);"); // dark green
        }

        // clear the filled data
        ui->oldPwdLineEdit->clear();
        ui->newPwdLineEdit->clear();
        ui->confirmPwdLineEdit->clear();
    }
}
