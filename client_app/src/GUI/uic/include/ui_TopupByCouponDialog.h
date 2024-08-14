/********************************************************************************
** Form generated from reading UI file 'TopupByCouponDialog.ui'
**
** Created: Wed Mar 25 20:42:53 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPUPBYCOUPONDIALOG_H
#define UI_TOPUPBYCOUPONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TopupByCouponDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QGridLayout *gridLayout_2;
    QLabel *userNameTitleLabel_2;
    QLineEdit *couponNameLineEdit;
    QLabel *userPwdTitleLabel_2;
    QLineEdit *couponPwdLineEdit;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *topupButton;
    QPushButton *closeButton;

    void setupUi(QDialog *TopupByCouponDialog)
    {
        if (TopupByCouponDialog->objectName().isEmpty())
            TopupByCouponDialog->setObjectName(QString::fromUtf8("TopupByCouponDialog"));
        TopupByCouponDialog->resize(260, 136);
        verticalLayout = new QVBoxLayout(TopupByCouponDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(TopupByCouponDialog);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(TopupByCouponDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        userNameTitleLabel_2 = new QLabel(TopupByCouponDialog);
        userNameTitleLabel_2->setObjectName(QString::fromUtf8("userNameTitleLabel_2"));

        gridLayout_2->addWidget(userNameTitleLabel_2, 0, 0, 1, 1);

        couponNameLineEdit = new QLineEdit(TopupByCouponDialog);
        couponNameLineEdit->setObjectName(QString::fromUtf8("couponNameLineEdit"));
        couponNameLineEdit->setInputMask(QString::fromUtf8(""));

        gridLayout_2->addWidget(couponNameLineEdit, 0, 1, 1, 1);

        userPwdTitleLabel_2 = new QLabel(TopupByCouponDialog);
        userPwdTitleLabel_2->setObjectName(QString::fromUtf8("userPwdTitleLabel_2"));

        gridLayout_2->addWidget(userPwdTitleLabel_2, 1, 0, 1, 1);

        couponPwdLineEdit = new QLineEdit(TopupByCouponDialog);
        couponPwdLineEdit->setObjectName(QString::fromUtf8("couponPwdLineEdit"));
        couponPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(couponPwdLineEdit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(78, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        topupButton = new QPushButton(TopupByCouponDialog);
        topupButton->setObjectName(QString::fromUtf8("topupButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Common/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        topupButton->setIcon(icon);
        topupButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout_3->addWidget(topupButton);

        closeButton = new QPushButton(TopupByCouponDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Common/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon1);
        closeButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlackButton")));

        horizontalLayout_3->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(TopupByCouponDialog);

        QMetaObject::connectSlotsByName(TopupByCouponDialog);
    } // setupUi

    void retranslateUi(QDialog *TopupByCouponDialog)
    {
        TopupByCouponDialog->setWindowTitle(QApplication::translate("TopupByCouponDialog", "Topup by Coupon", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TopupByCouponDialog", "Enter the Coupon Account name and password:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TopupByCouponDialog", "NOTE: Coupon will be voided after the topup", 0, QApplication::UnicodeUTF8));
        userNameTitleLabel_2->setText(QApplication::translate("TopupByCouponDialog", "Account:", 0, QApplication::UnicodeUTF8));
        userPwdTitleLabel_2->setText(QApplication::translate("TopupByCouponDialog", "Password", 0, QApplication::UnicodeUTF8));
        topupButton->setText(QApplication::translate("TopupByCouponDialog", "Topup", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("TopupByCouponDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TopupByCouponDialog: public Ui_TopupByCouponDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPUPBYCOUPONDIALOG_H
