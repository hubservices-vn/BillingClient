/********************************************************************************
** Form generated from reading UI file 'LockOutSettingDialog.ui'
**
** Created: Wed Mar 25 20:42:54 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCKOUTSETTINGDIALOG_H
#define UI_LOCKOUTSETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LockOutSettingDialog
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *iconLabel;
    QLabel *descriptLabel;
    QFormLayout *formLayout;
    QLabel *pwdTitleLabel;
    QLineEdit *pwdLineEdit;
    QLabel *pwdRptTitleLabel;
    QLineEdit *pwdRptLineEdit;
    QLabel *msgLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *lockButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *LockOutSettingDialog)
    {
        if (LockOutSettingDialog->objectName().isEmpty())
            LockOutSettingDialog->setObjectName(QString::fromUtf8("LockOutSettingDialog"));
        LockOutSettingDialog->resize(260, 179);
        horizontalLayout_3 = new QHBoxLayout(LockOutSettingDialog);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        iconLabel = new QLabel(LockOutSettingDialog);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        iconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/LockSettingDialog/icon-lock.png")));

        horizontalLayout->addWidget(iconLabel);

        descriptLabel = new QLabel(LockOutSettingDialog);
        descriptLabel->setObjectName(QString::fromUtf8("descriptLabel"));

        horizontalLayout->addWidget(descriptLabel);


        verticalLayout->addLayout(horizontalLayout);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        pwdTitleLabel = new QLabel(LockOutSettingDialog);
        pwdTitleLabel->setObjectName(QString::fromUtf8("pwdTitleLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, pwdTitleLabel);

        pwdLineEdit = new QLineEdit(LockOutSettingDialog);
        pwdLineEdit->setObjectName(QString::fromUtf8("pwdLineEdit"));
        pwdLineEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(0, QFormLayout::FieldRole, pwdLineEdit);

        pwdRptTitleLabel = new QLabel(LockOutSettingDialog);
        pwdRptTitleLabel->setObjectName(QString::fromUtf8("pwdRptTitleLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, pwdRptTitleLabel);

        pwdRptLineEdit = new QLineEdit(LockOutSettingDialog);
        pwdRptLineEdit->setObjectName(QString::fromUtf8("pwdRptLineEdit"));
        pwdRptLineEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, pwdRptLineEdit);


        verticalLayout->addLayout(formLayout);

        msgLabel = new QLabel(LockOutSettingDialog);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));

        verticalLayout->addWidget(msgLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        lockButton = new QPushButton(LockOutSettingDialog);
        lockButton->setObjectName(QString::fromUtf8("lockButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/LockSettingDialog/icon-btn-lock.png"), QSize(), QIcon::Normal, QIcon::Off);
        lockButton->setIcon(icon);
        lockButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout_2->addWidget(lockButton);

        cancelButton = new QPushButton(LockOutSettingDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Common/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelButton->setIcon(icon1);
        cancelButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlackButton")));

        horizontalLayout_2->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(LockOutSettingDialog);

        QMetaObject::connectSlotsByName(LockOutSettingDialog);
    } // setupUi

    void retranslateUi(QDialog *LockOutSettingDialog)
    {
        LockOutSettingDialog->setWindowTitle(QApplication::translate("LockOutSettingDialog", "Lock computer", 0, QApplication::UnicodeUTF8));
        iconLabel->setText(QString());
        descriptLabel->setText(QApplication::translate("LockOutSettingDialog", "Set a temp password here.\n"
"IMPORTANT: \n"
"be sure to remember this password.\n"
"it will be used to unlock the computer later", 0, QApplication::UnicodeUTF8));
        pwdTitleLabel->setText(QApplication::translate("LockOutSettingDialog", "Password:", 0, QApplication::UnicodeUTF8));
        pwdRptTitleLabel->setText(QApplication::translate("LockOutSettingDialog", "Repeat password:", 0, QApplication::UnicodeUTF8));
        msgLabel->setText(QString());
        lockButton->setText(QApplication::translate("LockOutSettingDialog", "&Lock!", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("LockOutSettingDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LockOutSettingDialog: public Ui_LockOutSettingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCKOUTSETTINGDIALOG_H
