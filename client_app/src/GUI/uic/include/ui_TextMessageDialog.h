/********************************************************************************
** Form generated from reading UI file 'TextMessageDialog.ui'
**
** Created: Wed Mar 25 20:42:53 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTMESSAGEDIALOG_H
#define UI_TEXTMESSAGEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include "gplaintextedit.h"

QT_BEGIN_NAMESPACE

class Ui_TextMessageDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *historyTextEdit;
    QLabel *msgLabel;
    QHBoxLayout *horizontalLayout;
    GPlainTextEdit *inputTextEdit;
    QPushButton *sendButton;

    void setupUi(QDialog *TextMessageDialog)
    {
        if (TextMessageDialog->objectName().isEmpty())
            TextMessageDialog->setObjectName(QString::fromUtf8("TextMessageDialog"));
        TextMessageDialog->resize(352, 275);
        verticalLayout = new QVBoxLayout(TextMessageDialog);
        verticalLayout->setSpacing(0);
#ifndef Q_OS_MAC
        verticalLayout->setContentsMargins(9, 9, 9, 9);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        historyTextEdit = new QTextEdit(TextMessageDialog);
        historyTextEdit->setObjectName(QString::fromUtf8("historyTextEdit"));
        historyTextEdit->setReadOnly(true);

        verticalLayout->addWidget(historyTextEdit);

        msgLabel = new QLabel(TextMessageDialog);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));

        verticalLayout->addWidget(msgLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inputTextEdit = new GPlainTextEdit(TextMessageDialog);
        inputTextEdit->setObjectName(QString::fromUtf8("inputTextEdit"));
        inputTextEdit->setMaximumSize(QSize(16777215, 50));

        horizontalLayout->addWidget(inputTextEdit);

        sendButton = new QPushButton(TextMessageDialog);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setMaximumSize(QSize(16777215, 50));
        sendButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color:#ffffff;\n"
"	font: 13pt bold;\n"
"    border-image: url(:/styles/styles/Button/btn-send-normal.png);\n"
"    min-height:47px;\n"
"    min-width:70px;\n"
"}\n"
"QPushButton:selected {\n"
"    border-image: url(:/styles/styles/Button/btn-send-down.png);\n"
"}\n"
"QPushButton:hover {\n"
"    border-image: url(:/styles/styles/Button/btn-send-hover.png);\n"
"}\n"
"QPushButton:disabled {\n"
"    border-image: url(:/styles/styles/Button/btn-send-disabled.png);\n"
"}"));

        horizontalLayout->addWidget(sendButton);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(inputTextEdit, sendButton);
        QWidget::setTabOrder(sendButton, historyTextEdit);

        retranslateUi(TextMessageDialog);

        QMetaObject::connectSlotsByName(TextMessageDialog);
    } // setupUi

    void retranslateUi(QDialog *TextMessageDialog)
    {
        TextMessageDialog->setWindowTitle(QApplication::translate("TextMessageDialog", "Chat window", 0, QApplication::UnicodeUTF8));
        msgLabel->setText(QString());
        sendButton->setText(QApplication::translate("TextMessageDialog", "Send", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TextMessageDialog: public Ui_TextMessageDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTMESSAGEDIALOG_H
