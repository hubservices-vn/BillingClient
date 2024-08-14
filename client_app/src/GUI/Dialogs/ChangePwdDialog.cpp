#include "ChangePwdDialog.h"
#include "Utils.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExp>
#include <QMessageBox>
#include "UserAccountManager.h"
#include "RegexpSettings.h"
#include "ui_ChangePwdDialog.h"
#include "ChangeAccPwdQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "ConfigManager.h"

ChangePwdDialog::ChangePwdDialog(QWidget *parent) 
: QDialog(parent), ui(new Ui::ChangePwdDialog)
{
    ui->setupUi(this);

    {
        QRegExp rx(PWD_REGEXP_PTN);
        QValidator *validator = new QRegExpValidator(rx, this);
        ui->oldPwdLineEdit->setValidator(validator);
        ui->newPwdLineEdit->setValidator(validator);
        ui->newPwdLineEdit->setPlaceholderText(PWD_PLACEHOLDER);
        ui->confirmPwdLineEdit->setValidator(validator);
    }

    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowSystemMenuHint);

    connect(ui->oldPwdLineEdit, SIGNAL(textChanged(QString)), 
        this, SLOT(enableDisableButtons(QString)));
    connect(ui->newPwdLineEdit, SIGNAL(textChanged(QString)), 
        this, SLOT(enableDisableButtons(QString)));
    connect(ui->confirmPwdLineEdit, SIGNAL(textChanged(QString)), 
        this, SLOT(enableDisableButtons(QString)));
}

ChangePwdDialog::~ChangePwdDialog()
{
    if (ui != NULL)
    {
        delete ui;
        ui = NULL;
    }
}

void ChangePwdDialog::on_okButton_clicked()
{
    m_oldPwd = ui->oldPwdLineEdit->text();
    m_newPwd = ui->newPwdLineEdit->text();

    if (m_oldPwd.isEmpty() ||
        m_newPwd.isEmpty())
    {
        return;
    }

    if(QString::compare(m_newPwd, m_oldPwd, Qt::CaseSensitive) != 0)
    {
        ChangeAccPwdRequestQtModel qtModel(
            CConfigManager::clientId(), 
            UserAccountManager::currentUserName(),
            m_oldPwd, 
            m_newPwd);
        cc2csSender::send(cc2cs::ChangeAccPwdRequestCommand(qtModel));
    }
    else
    {
        QString msg = tr("The new password is the same as the old one, the password won't be changed.");
        QMessageBox::information(this, tr("Notification"), msg);
    }

    this->done(1);
}

void ChangePwdDialog::on_cancelButton_clicked()
{
    this->done(0);
}

void ChangePwdDialog::enableDisableButtons( const QString & text )
{
    QString oldPwd = ui->oldPwdLineEdit->text();
    QString newPwd = ui->newPwdLineEdit->text();
    QString confirmPwd = ui->confirmPwdLineEdit->text();

    bool oldPwdValid = !oldPwd.isEmpty();
    
    bool newValid = 
        !newPwd.isEmpty() && 
        !confirmPwd.isEmpty() &&
        newPwd == confirmPwd;
    if (newPwd != confirmPwd)
    {
        ui->reasonLabel->setText(tr("The passwords do NOT match"));
    }
    else
    {
        ui->reasonLabel->clear();
    }

    ui->okButton->setEnabled(oldPwdValid && newValid);
}

void ChangePwdDialog::changeEvent( QEvent *e )
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

void ChangePwdDialog::retranslateUiEx()
{
}

