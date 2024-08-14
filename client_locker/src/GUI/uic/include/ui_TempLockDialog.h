/********************************************************************************
** Form generated from reading UI file 'TempLockDialog.ui'
**
** Created: Thu Mar 26 14:13:04 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEMPLOCKDIALOG_H
#define UI_TEMPLOCKDIALOG_H

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
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TempLockDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget;
    QWidget *unlockByCodeTab;
    QVBoxLayout *verticalLayout_2;
    QLabel *descriptLabel_2;
    QGridLayout *gridLayout_2;
    QLabel *pwdTitleLabel_2;
    QLineEdit *tempPwdLineEdit;
    QLabel *msgLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *unlockButton;
    QWidget *unlockByAccountTab;
    QVBoxLayout *verticalLayout;
    QLabel *descriptLabel_3;
    QGridLayout *gridLayout_3;
    QLabel *pwdTitleLabel_3;
    QLineEdit *userNameLineEdit;
    QLabel *pwdTitleLabel_4;
    QLineEdit *pwdLineEdit;
    QLabel *msgLabel_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *unlocByAccountkButton;

    void setupUi(QDialog *TempLockDialog)
    {
        if (TempLockDialog->objectName().isEmpty())
            TempLockDialog->setObjectName(QString::fromUtf8("TempLockDialog"));
        TempLockDialog->resize(238, 184);
        verticalLayout_3 = new QVBoxLayout(TempLockDialog);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget = new QTabWidget(TempLockDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        unlockByCodeTab = new QWidget();
        unlockByCodeTab->setObjectName(QString::fromUtf8("unlockByCodeTab"));
        verticalLayout_2 = new QVBoxLayout(unlockByCodeTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        descriptLabel_2 = new QLabel(unlockByCodeTab);
        descriptLabel_2->setObjectName(QString::fromUtf8("descriptLabel_2"));

        verticalLayout_2->addWidget(descriptLabel_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pwdTitleLabel_2 = new QLabel(unlockByCodeTab);
        pwdTitleLabel_2->setObjectName(QString::fromUtf8("pwdTitleLabel_2"));

        gridLayout_2->addWidget(pwdTitleLabel_2, 0, 0, 1, 1);

        tempPwdLineEdit = new QLineEdit(unlockByCodeTab);
        tempPwdLineEdit->setObjectName(QString::fromUtf8("tempPwdLineEdit"));
        tempPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout_2->addWidget(tempPwdLineEdit, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        msgLabel = new QLabel(unlockByCodeTab);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));

        verticalLayout_2->addWidget(msgLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        unlockButton = new QPushButton(unlockByCodeTab);
        unlockButton->setObjectName(QString::fromUtf8("unlockButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/TempLockDialog/icon-btn-unlock.png"), QSize(), QIcon::Normal, QIcon::Off);
        unlockButton->setIcon(icon);
        unlockButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout_2->addWidget(unlockButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/TempLockDialog/icon-unlock-by-code.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(unlockByCodeTab, icon1, QString());
        unlockByAccountTab = new QWidget();
        unlockByAccountTab->setObjectName(QString::fromUtf8("unlockByAccountTab"));
        verticalLayout = new QVBoxLayout(unlockByAccountTab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        descriptLabel_3 = new QLabel(unlockByAccountTab);
        descriptLabel_3->setObjectName(QString::fromUtf8("descriptLabel_3"));

        verticalLayout->addWidget(descriptLabel_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        pwdTitleLabel_3 = new QLabel(unlockByAccountTab);
        pwdTitleLabel_3->setObjectName(QString::fromUtf8("pwdTitleLabel_3"));

        gridLayout_3->addWidget(pwdTitleLabel_3, 0, 0, 1, 1);

        userNameLineEdit = new QLineEdit(unlockByAccountTab);
        userNameLineEdit->setObjectName(QString::fromUtf8("userNameLineEdit"));
        userNameLineEdit->setEchoMode(QLineEdit::Normal);

        gridLayout_3->addWidget(userNameLineEdit, 0, 1, 1, 1);

        pwdTitleLabel_4 = new QLabel(unlockByAccountTab);
        pwdTitleLabel_4->setObjectName(QString::fromUtf8("pwdTitleLabel_4"));

        gridLayout_3->addWidget(pwdTitleLabel_4, 1, 0, 1, 1);

        pwdLineEdit = new QLineEdit(unlockByAccountTab);
        pwdLineEdit->setObjectName(QString::fromUtf8("pwdLineEdit"));
        pwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout_3->addWidget(pwdLineEdit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_3);

        msgLabel_3 = new QLabel(unlockByAccountTab);
        msgLabel_3->setObjectName(QString::fromUtf8("msgLabel_3"));

        verticalLayout->addWidget(msgLabel_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(108, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        unlocByAccountkButton = new QPushButton(unlockByAccountTab);
        unlocByAccountkButton->setObjectName(QString::fromUtf8("unlocByAccountkButton"));
        unlocByAccountkButton->setIcon(icon);
        unlocByAccountkButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout_3->addWidget(unlocByAccountkButton);


        verticalLayout->addLayout(horizontalLayout_3);

        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/TempLockDialog/icon-unlock-by-acc.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(unlockByAccountTab, icon2, QString());

        verticalLayout_3->addWidget(tabWidget);


        retranslateUi(TempLockDialog);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(TempLockDialog);
    } // setupUi

    void retranslateUi(QDialog *TempLockDialog)
    {
        TempLockDialog->setWindowTitle(QApplication::translate("TempLockDialog", "Unlock computer", 0, QApplication::UnicodeUTF8));
        descriptLabel_2->setText(QApplication::translate("TempLockDialog", "Enter the password you just set", 0, QApplication::UnicodeUTF8));
        pwdTitleLabel_2->setText(QApplication::translate("TempLockDialog", "Password:", 0, QApplication::UnicodeUTF8));
        msgLabel->setText(QString());
        unlockButton->setText(QApplication::translate("TempLockDialog", "Unlock", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(unlockByCodeTab), QApplication::translate("TempLockDialog", "Unlock", 0, QApplication::UnicodeUTF8));
        descriptLabel_3->setText(QApplication::translate("TempLockDialog", "Enter your user name and password", 0, QApplication::UnicodeUTF8));
        pwdTitleLabel_3->setText(QApplication::translate("TempLockDialog", "User name:", 0, QApplication::UnicodeUTF8));
        pwdTitleLabel_4->setText(QApplication::translate("TempLockDialog", "Password:", 0, QApplication::UnicodeUTF8));
        msgLabel_3->setText(QString());
        unlocByAccountkButton->setText(QApplication::translate("TempLockDialog", "Unlock", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(unlockByAccountTab), QApplication::translate("TempLockDialog", "Forget password?", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TempLockDialog: public Ui_TempLockDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEMPLOCKDIALOG_H
