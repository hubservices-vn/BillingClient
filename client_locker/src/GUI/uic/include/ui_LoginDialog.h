/********************************************************************************
** Form generated from reading UI file 'LoginDialog.ui'
**
** Created: Thu Mar 26 14:13:04 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

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

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *userLoginDescriptLabel;
    QGridLayout *gridLayout;
    QLabel *userNameTitleLabel;
    QLineEdit *userNameLineEdit;
    QLabel *userPwdTitleLabel;
    QLineEdit *userPwdLineEdit;
    QLabel *failureReasonLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *userLoginButton;
    QPushButton *closeButton;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QString::fromUtf8("LoginDialog"));
        LoginDialog->resize(237, 186);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Common/client.ico"), QSize(), QIcon::Normal, QIcon::Off);
        LoginDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(LoginDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        userLoginDescriptLabel = new QLabel(LoginDialog);
        userLoginDescriptLabel->setObjectName(QString::fromUtf8("userLoginDescriptLabel"));

        verticalLayout->addWidget(userLoginDescriptLabel);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        userNameTitleLabel = new QLabel(LoginDialog);
        userNameTitleLabel->setObjectName(QString::fromUtf8("userNameTitleLabel"));

        gridLayout->addWidget(userNameTitleLabel, 0, 0, 1, 1);

        userNameLineEdit = new QLineEdit(LoginDialog);
        userNameLineEdit->setObjectName(QString::fromUtf8("userNameLineEdit"));
        userNameLineEdit->setInputMask(QString::fromUtf8(""));

        gridLayout->addWidget(userNameLineEdit, 0, 1, 1, 1);

        userPwdTitleLabel = new QLabel(LoginDialog);
        userPwdTitleLabel->setObjectName(QString::fromUtf8("userPwdTitleLabel"));

        gridLayout->addWidget(userPwdTitleLabel, 1, 0, 1, 1);

        userPwdLineEdit = new QLineEdit(LoginDialog);
        userPwdLineEdit->setObjectName(QString::fromUtf8("userPwdLineEdit"));
        userPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(userPwdLineEdit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        failureReasonLabel = new QLabel(LoginDialog);
        failureReasonLabel->setObjectName(QString::fromUtf8("failureReasonLabel"));

        verticalLayout->addWidget(failureReasonLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(78, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        userLoginButton = new QPushButton(LoginDialog);
        userLoginButton->setObjectName(QString::fromUtf8("userLoginButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Common/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        userLoginButton->setIcon(icon1);
        userLoginButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout_2->addWidget(userLoginButton);

        closeButton = new QPushButton(LoginDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/Common/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon2);
        closeButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlackButton")));

        horizontalLayout_2->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "Garena Cafe Billing", 0, QApplication::UnicodeUTF8));
        userLoginDescriptLabel->setText(QApplication::translate("LoginDialog", "Enter the user name and password", 0, QApplication::UnicodeUTF8));
        userNameTitleLabel->setText(QApplication::translate("LoginDialog", "Username:", 0, QApplication::UnicodeUTF8));
        userPwdTitleLabel->setText(QApplication::translate("LoginDialog", "Password", 0, QApplication::UnicodeUTF8));
        failureReasonLabel->setText(QString());
        userLoginButton->setText(QApplication::translate("LoginDialog", "Login", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("LoginDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
