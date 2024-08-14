/********************************************************************************
** Form generated from reading UI file 'ChangePwdDialog.ui'
**
** Created: Wed Mar 25 20:42:54 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHANGEPWDDIALOG_H
#define UI_CHANGEPWDDIALOG_H

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

class Ui_ChangePwdDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *oldPwdLineEdit;
    QLabel *label_2;
    QLineEdit *newPwdLineEdit;
    QLabel *label_3;
    QLineEdit *confirmPwdLineEdit;
    QLabel *reasonLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ChangePwdDialog)
    {
        if (ChangePwdDialog->objectName().isEmpty())
            ChangePwdDialog->setObjectName(QString::fromUtf8("ChangePwdDialog"));
        ChangePwdDialog->resize(249, 143);
        verticalLayout = new QVBoxLayout(ChangePwdDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ChangePwdDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        oldPwdLineEdit = new QLineEdit(ChangePwdDialog);
        oldPwdLineEdit->setObjectName(QString::fromUtf8("oldPwdLineEdit"));
        oldPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(oldPwdLineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(ChangePwdDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        newPwdLineEdit = new QLineEdit(ChangePwdDialog);
        newPwdLineEdit->setObjectName(QString::fromUtf8("newPwdLineEdit"));
        newPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(newPwdLineEdit, 1, 1, 1, 1);

        label_3 = new QLabel(ChangePwdDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        confirmPwdLineEdit = new QLineEdit(ChangePwdDialog);
        confirmPwdLineEdit->setObjectName(QString::fromUtf8("confirmPwdLineEdit"));
        confirmPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(confirmPwdLineEdit, 2, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        reasonLabel = new QLabel(ChangePwdDialog);
        reasonLabel->setObjectName(QString::fromUtf8("reasonLabel"));
        reasonLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(reasonLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(ChangePwdDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Common/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        okButton->setIcon(icon);
        okButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout->addWidget(okButton);

        cancelButton = new QPushButton(ChangePwdDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Common/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelButton->setIcon(icon1);
        cancelButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlackButton")));

        horizontalLayout->addWidget(cancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ChangePwdDialog);

        QMetaObject::connectSlotsByName(ChangePwdDialog);
    } // setupUi

    void retranslateUi(QDialog *ChangePwdDialog)
    {
        ChangePwdDialog->setWindowTitle(QApplication::translate("ChangePwdDialog", "Change password", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ChangePwdDialog", "Old password:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ChangePwdDialog", "New password:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ChangePwdDialog", "Confirm password:", 0, QApplication::UnicodeUTF8));
        reasonLabel->setText(QString());
        okButton->setText(QApplication::translate("ChangePwdDialog", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ChangePwdDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ChangePwdDialog: public Ui_ChangePwdDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHANGEPWDDIALOG_H
