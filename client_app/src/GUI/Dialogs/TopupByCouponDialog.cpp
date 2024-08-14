#include "TopupByCouponDialog.h"
#include "Utils.h"
#include <QRegExpValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegExp>
#include <QMessageBox>
#include "UserAccountManager.h"
#include "RegexpSettings.h"
#include "GUpperCaseValidator.h"
#include "ui_TopupByCouponDialog.h"
#include "TopupByCouponQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "ConfigManager.h"

TopupByCouponDialog::TopupByCouponDialog(QWidget *parent) 
: QDialog(parent), ui(new Ui::TopupByCouponDialog)
{
    ui->setupUi(this);

    {
        QRegExp rx(SIMPLE_USER_NAME_REGEXP_PTN);
        QValidator *validator = new GUpperCaseValidator(rx, this);
        ui->couponNameLineEdit->setValidator(validator);

        QRegExp rxPwd(PWD_REGEXP_PTN);
        QValidator *validatorPwd = new QRegExpValidator(rxPwd, this);
        ui->couponPwdLineEdit->setValidator(validatorPwd);
    }

    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowSystemMenuHint);
}

TopupByCouponDialog::~TopupByCouponDialog()
{
    if (ui != NULL)
    {
        delete ui;
        ui = NULL;
    }
}

void TopupByCouponDialog::on_topupButton_clicked()
{
    m_couponAcc = ui->couponNameLineEdit->text();
    m_couponPwd = ui->couponPwdLineEdit->text();

    if (m_couponAcc.isEmpty() ||
        m_couponPwd.isEmpty())
    {
        return;
    }

    TopupByCouponRequestQtModel qtModel(
            CConfigManager::clientId(), 
            UserAccountManager::currentUserId(),
            UserAccountManager::currentUserType(),
            m_couponAcc, 
            m_couponPwd);
        cc2csSender::send(cc2cs::TopupByCouponRequestCommandC(qtModel));    
    this->done(1);
}

void TopupByCouponDialog::on_closeButton_clicked()
{
    this->done(0);
}

void TopupByCouponDialog::changeEvent( QEvent *e )
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

void TopupByCouponDialog::retranslateUiEx()
{
}

