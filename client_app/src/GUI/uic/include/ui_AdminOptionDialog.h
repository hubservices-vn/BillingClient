/********************************************************************************
** Form generated from reading UI file 'AdminOptionDialog.ui'
**
** Created: Wed Mar 25 20:42:54 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINOPTIONDIALOG_H
#define UI_ADMINOPTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "qcipwidget.h"

QT_BEGIN_NAMESPACE

class Ui_AdminOptionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QcIPWidget *qcIPWidget;
    QGroupBox *adminAccountGroupBox;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *loginNameLineEdit;
    QLabel *label_3;
    QLineEdit *oldPwdLineEdit;
    QLabel *label_4;
    QLineEdit *newPwdLineEdit;
    QLabel *label_5;
    QLineEdit *confirmPwdLineEdit;
    QLabel *staticMessageLabel;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *changeButton;
    QPushButton *closeButton;

    void setupUi(QDialog *AdminOptionDialog)
    {
        if (AdminOptionDialog->objectName().isEmpty())
            AdminOptionDialog->setObjectName(QString::fromUtf8("AdminOptionDialog"));
        AdminOptionDialog->resize(282, 257);
        verticalLayout = new QVBoxLayout(AdminOptionDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(AdminOptionDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"	background-color:white;\n"
"	border: 0;\n"
"}\n"
"QLabel[cate=\"ipSep\"] {\n"
"	border: 0;\n"
"	background-color:white;\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
#ifndef Q_OS_MAC
        horizontalLayout_2->setContentsMargins(9, 9, 9, 9);
#endif
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        qcIPWidget = new QcIPWidget(groupBox_2);
        qcIPWidget->setObjectName(QString::fromUtf8("qcIPWidget"));
        qcIPWidget->setMinimumSize(QSize(80, 16));
        qcIPWidget->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_2->addWidget(qcIPWidget);


        verticalLayout->addWidget(groupBox_2);

        adminAccountGroupBox = new QGroupBox(AdminOptionDialog);
        adminAccountGroupBox->setObjectName(QString::fromUtf8("adminAccountGroupBox"));
        adminAccountGroupBox->setEnabled(true);
        adminAccountGroupBox->setCheckable(true);
        adminAccountGroupBox->setChecked(false);
        gridLayout = new QGridLayout(adminAccountGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(adminAccountGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        loginNameLineEdit = new QLineEdit(adminAccountGroupBox);
        loginNameLineEdit->setObjectName(QString::fromUtf8("loginNameLineEdit"));
        loginNameLineEdit->setInputMask(QString::fromUtf8(""));

        gridLayout->addWidget(loginNameLineEdit, 0, 1, 1, 1);

        label_3 = new QLabel(adminAccountGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        oldPwdLineEdit = new QLineEdit(adminAccountGroupBox);
        oldPwdLineEdit->setObjectName(QString::fromUtf8("oldPwdLineEdit"));
        oldPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(oldPwdLineEdit, 1, 1, 1, 1);

        label_4 = new QLabel(adminAccountGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        newPwdLineEdit = new QLineEdit(adminAccountGroupBox);
        newPwdLineEdit->setObjectName(QString::fromUtf8("newPwdLineEdit"));
        newPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(newPwdLineEdit, 2, 1, 1, 1);

        label_5 = new QLabel(adminAccountGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        confirmPwdLineEdit = new QLineEdit(adminAccountGroupBox);
        confirmPwdLineEdit->setObjectName(QString::fromUtf8("confirmPwdLineEdit"));
        confirmPwdLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(confirmPwdLineEdit, 3, 1, 1, 1);


        verticalLayout->addWidget(adminAccountGroupBox);

        staticMessageLabel = new QLabel(AdminOptionDialog);
        staticMessageLabel->setObjectName(QString::fromUtf8("staticMessageLabel"));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        staticMessageLabel->setFont(font);
        staticMessageLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(staticMessageLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        changeButton = new QPushButton(AdminOptionDialog);
        changeButton->setObjectName(QString::fromUtf8("changeButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Common/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
        changeButton->setIcon(icon);
        changeButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout->addWidget(changeButton);

        closeButton = new QPushButton(AdminOptionDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Common/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon1);
        closeButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlackButton")));

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(adminAccountGroupBox, loginNameLineEdit);
        QWidget::setTabOrder(loginNameLineEdit, oldPwdLineEdit);
        QWidget::setTabOrder(oldPwdLineEdit, newPwdLineEdit);
        QWidget::setTabOrder(newPwdLineEdit, confirmPwdLineEdit);
        QWidget::setTabOrder(confirmPwdLineEdit, changeButton);
        QWidget::setTabOrder(changeButton, closeButton);

        retranslateUi(AdminOptionDialog);

        QMetaObject::connectSlotsByName(AdminOptionDialog);
    } // setupUi

    void retranslateUi(QDialog *AdminOptionDialog)
    {
        AdminOptionDialog->setWindowTitle(QApplication::translate("AdminOptionDialog", "Option", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("AdminOptionDialog", "Server address", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AdminOptionDialog", "IP address of server:", 0, QApplication::UnicodeUTF8));
        adminAccountGroupBox->setTitle(QApplication::translate("AdminOptionDialog", "Change admin account on this workstation only", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AdminOptionDialog", "Login name:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AdminOptionDialog", "Old password:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AdminOptionDialog", "New password:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AdminOptionDialog", "Confirm password:", 0, QApplication::UnicodeUTF8));
        staticMessageLabel->setText(QString());
        changeButton->setText(QApplication::translate("AdminOptionDialog", "Change", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("AdminOptionDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AdminOptionDialog: public Ui_AdminOptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINOPTIONDIALOG_H
