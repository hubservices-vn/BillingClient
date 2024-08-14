/********************************************************************************
** Form generated from reading UI file 'MiniDialog.ui'
**
** Created: Wed Mar 25 20:42:54 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINIDIALOG_H
#define UI_MINIDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "gclickablelabel.h"
#include "qcribbonpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_MiniDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *dlgHeaderWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *userNameLabel;
    QLabel *connStatusTextLabel;
    QLabel *connStatusIconLabel;
    QPushButton *minimizeButton;
    QWidget *dlgContentWidget;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_3;
    QLabel *totalTimeTitleLabel;
    QFrame *frame_7;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_6;
    QLabel *totalTimeLabel;
    QLabel *timeUsedTitleLabel;
    QFrame *frame_8;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_7;
    QLabel *timeUsedTextLabel;
    QLabel *timeLeftTitleLabel;
    QFrame *frame_9;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_8;
    QLabel *timeLeftTextLabel;
    QLabel *timeFeeTitleLabel;
    QFrame *frame_10;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_9;
    QLabel *timeFeeTextLabel;
    QLabel *serviceFeeTitleLabel;
    QFrame *frame_11;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_10;
    QLabel *serviceFeeTextLabel;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *sysMsgIconLabel;
    QLabel *sysMsgTextLabel;
    QGridLayout *gridLayout_2;
    QcRibbonPushButton *sendMessageButton;
    QcRibbonPushButton *orderButton;
    QcRibbonPushButton *logoutButton;
    QcRibbonPushButton *changePwdButton;
    QcRibbonPushButton *lockButton;
    QcRibbonPushButton *utilitiesButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *optionButton;
    QPushButton *exitButton;
    GClickableLabel *defaultAdLabel;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *allowTrayMsgCheckBox;
    QComboBox *langComboBox;

    void setupUi(QDialog *MiniDialog)
    {
        if (MiniDialog->objectName().isEmpty())
            MiniDialog->setObjectName(QString::fromUtf8("MiniDialog"));
        MiniDialog->resize(222, 472);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MiniDialog->sizePolicy().hasHeightForWidth());
        MiniDialog->setSizePolicy(sizePolicy);
        MiniDialog->setMinimumSize(QSize(220, 0));
        MiniDialog->setMaximumSize(QSize(222, 16777215));
        MiniDialog->setWindowTitle(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Common/client.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MiniDialog->setWindowIcon(icon);
        MiniDialog->setStyleSheet(QString::fromUtf8(""));
        MiniDialog->setProperty("cssType", QVariant(QString::fromUtf8("customFrameDlg")));
        verticalLayout = new QVBoxLayout(MiniDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        dlgHeaderWidget = new QWidget(MiniDialog);
        dlgHeaderWidget->setObjectName(QString::fromUtf8("dlgHeaderWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dlgHeaderWidget->sizePolicy().hasHeightForWidth());
        dlgHeaderWidget->setSizePolicy(sizePolicy1);
        dlgHeaderWidget->setMinimumSize(QSize(0, 30));
        dlgHeaderWidget->setMaximumSize(QSize(16777215, 30));
        dlgHeaderWidget->setStyleSheet(QString::fromUtf8(""));
        dlgHeaderWidget->setProperty("cssType", QVariant(QString::fromUtf8("dialogHeader")));
        horizontalLayout_3 = new QHBoxLayout(dlgHeaderWidget);
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, 4, 0);
        userNameLabel = new QLabel(dlgHeaderWidget);
        userNameLabel->setObjectName(QString::fromUtf8("userNameLabel"));
        userNameLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #ffffff;\n"
"	font: bold 11px;\n"
"}"));
        userNameLabel->setText(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(userNameLabel);

        connStatusTextLabel = new QLabel(dlgHeaderWidget);
        connStatusTextLabel->setObjectName(QString::fromUtf8("connStatusTextLabel"));
        connStatusTextLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	font:10px;\n"
"}"));
        connStatusTextLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(connStatusTextLabel);

        connStatusIconLabel = new QLabel(dlgHeaderWidget);
        connStatusIconLabel->setObjectName(QString::fromUtf8("connStatusIconLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(connStatusIconLabel->sizePolicy().hasHeightForWidth());
        connStatusIconLabel->setSizePolicy(sizePolicy2);
        connStatusIconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/MiniWindow/connection-green.png")));
        connStatusIconLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(connStatusIconLabel);

        minimizeButton = new QPushButton(dlgHeaderWidget);
        minimizeButton->setObjectName(QString::fromUtf8("minimizeButton"));
        sizePolicy.setHeightForWidth(minimizeButton->sizePolicy().hasHeightForWidth());
        minimizeButton->setSizePolicy(sizePolicy);
        minimizeButton->setMinimumSize(QSize(20, 20));
        minimizeButton->setMaximumSize(QSize(20, 20));
        minimizeButton->setStyleSheet(QString::fromUtf8("QPushButton[cssType=\"smallButton\"] {\n"
"	border:none;\n"
"    image: url(:/styles/styles/Button/btn-close-normal.png);\n"
"}\n"
"\n"
"QPushButton[cssType=\"smallButton\"]:selected {\n"
"	image: url(:/styles/styles/Button/btn-close-down.png);\n"
"}\n"
"\n"
"QPushButton[cssType=\"smallButton\"]:hover {\n"
"	image: url(:/styles/styles/Button/btn-close-hover.png);\n"
"}\n"
"\n"
"QPushButton[cssType=\"smallButton\"]:disabled {\n"
"	image: url(:/styles/styles/Button/btn-close-disabled.png);\n"
"}"));
        minimizeButton->setIconSize(QSize(16, 16));
        minimizeButton->setFlat(false);
        minimizeButton->setProperty("cssType", QVariant(QString::fromUtf8("smallButton")));

        horizontalLayout_3->addWidget(minimizeButton);


        verticalLayout->addWidget(dlgHeaderWidget);

        dlgContentWidget = new QWidget(MiniDialog);
        dlgContentWidget->setObjectName(QString::fromUtf8("dlgContentWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(dlgContentWidget->sizePolicy().hasHeightForWidth());
        dlgContentWidget->setSizePolicy(sizePolicy3);
        dlgContentWidget->setMinimumSize(QSize(0, 0));
        dlgContentWidget->setMaximumSize(QSize(16777215, 16777215));
        dlgContentWidget->setStyleSheet(QString::fromUtf8("QFrame[cssType=\"blueFrame\"] {\n"
"	border: 1px solid #7F9CB9;\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(dlgContentWidget);
        verticalLayout_3->setSpacing(12);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox = new QGroupBox(dlgContentWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	background: #fcfcfc;\n"
"	border: 1px solid #dddddd;\n"
"	border-radius: 5px;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(8, 8, 8, 8);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setVerticalSpacing(6);
        gridLayout_3->setContentsMargins(0, 6, 0, 6);
        totalTimeTitleLabel = new QLabel(groupBox);
        totalTimeTitleLabel->setObjectName(QString::fromUtf8("totalTimeTitleLabel"));
        QFont font;
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        totalTimeTitleLabel->setFont(font);
        totalTimeTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #666666;\n"
"	font: 13px;\n"
"}"));

        gridLayout_3->addWidget(totalTimeTitleLabel, 0, 0, 1, 1);

        frame_7 = new QFrame(groupBox);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setStyleSheet(QString::fromUtf8(""));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        frame_7->setProperty("cssType", QVariant(QString::fromUtf8("blueFrame")));
        horizontalLayout_10 = new QHBoxLayout(frame_7);
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalSpacer_6 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        totalTimeLabel = new QLabel(frame_7);
        totalTimeLabel->setObjectName(QString::fromUtf8("totalTimeLabel"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("12"));
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        totalTimeLabel->setFont(font1);
        totalTimeLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #333333;\n"
"	font: 13px bold;\n"
"}"));
        totalTimeLabel->setText(QString::fromUtf8(""));

        horizontalLayout_10->addWidget(totalTimeLabel);


        gridLayout_3->addWidget(frame_7, 0, 1, 1, 1);

        timeUsedTitleLabel = new QLabel(groupBox);
        timeUsedTitleLabel->setObjectName(QString::fromUtf8("timeUsedTitleLabel"));
        timeUsedTitleLabel->setFont(font);
        timeUsedTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #666666;\n"
"	font: 13px;\n"
"}"));

        gridLayout_3->addWidget(timeUsedTitleLabel, 1, 0, 1, 1);

        frame_8 = new QFrame(groupBox);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setStyleSheet(QString::fromUtf8(""));
        frame_8->setFrameShape(QFrame::StyledPanel);
        frame_8->setFrameShadow(QFrame::Raised);
        frame_8->setProperty("cssType", QVariant(QString::fromUtf8("blueFrame")));
        horizontalLayout_11 = new QHBoxLayout(frame_8);
        horizontalLayout_11->setSpacing(0);
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalSpacer_7 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_7);

        timeUsedTextLabel = new QLabel(frame_8);
        timeUsedTextLabel->setObjectName(QString::fromUtf8("timeUsedTextLabel"));
        timeUsedTextLabel->setFont(font1);
        timeUsedTextLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #333333;\n"
"	font: 13px bold;\n"
"}"));
        timeUsedTextLabel->setText(QString::fromUtf8(""));

        horizontalLayout_11->addWidget(timeUsedTextLabel);


        gridLayout_3->addWidget(frame_8, 1, 1, 1, 1);

        timeLeftTitleLabel = new QLabel(groupBox);
        timeLeftTitleLabel->setObjectName(QString::fromUtf8("timeLeftTitleLabel"));
        timeLeftTitleLabel->setFont(font);
        timeLeftTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #666666;\n"
"	font: 13px;\n"
"}"));

        gridLayout_3->addWidget(timeLeftTitleLabel, 2, 0, 1, 1);

        frame_9 = new QFrame(groupBox);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setStyleSheet(QString::fromUtf8(""));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        frame_9->setProperty("cssType", QVariant(QString::fromUtf8("blueFrame")));
        horizontalLayout_12 = new QHBoxLayout(frame_9);
        horizontalLayout_12->setSpacing(0);
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalSpacer_8 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_8);

        timeLeftTextLabel = new QLabel(frame_9);
        timeLeftTextLabel->setObjectName(QString::fromUtf8("timeLeftTextLabel"));
        timeLeftTextLabel->setFont(font1);
        timeLeftTextLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #333333;\n"
"	font: 13px bold;\n"
"}"));
        timeLeftTextLabel->setText(QString::fromUtf8(""));

        horizontalLayout_12->addWidget(timeLeftTextLabel);


        gridLayout_3->addWidget(frame_9, 2, 1, 1, 1);

        timeFeeTitleLabel = new QLabel(groupBox);
        timeFeeTitleLabel->setObjectName(QString::fromUtf8("timeFeeTitleLabel"));
        timeFeeTitleLabel->setFont(font);
        timeFeeTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #666666;\n"
"	font: 13px;\n"
"}"));

        gridLayout_3->addWidget(timeFeeTitleLabel, 3, 0, 1, 1);

        frame_10 = new QFrame(groupBox);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setStyleSheet(QString::fromUtf8(""));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        frame_10->setProperty("cssType", QVariant(QString::fromUtf8("blueFrame")));
        horizontalLayout_13 = new QHBoxLayout(frame_10);
        horizontalLayout_13->setSpacing(0);
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalSpacer_9 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_9);

        timeFeeTextLabel = new QLabel(frame_10);
        timeFeeTextLabel->setObjectName(QString::fromUtf8("timeFeeTextLabel"));
        timeFeeTextLabel->setFont(font1);
        timeFeeTextLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #333333;\n"
"	font: 13px bold;\n"
"}"));
        timeFeeTextLabel->setText(QString::fromUtf8(""));

        horizontalLayout_13->addWidget(timeFeeTextLabel);


        gridLayout_3->addWidget(frame_10, 3, 1, 1, 1);

        serviceFeeTitleLabel = new QLabel(groupBox);
        serviceFeeTitleLabel->setObjectName(QString::fromUtf8("serviceFeeTitleLabel"));
        serviceFeeTitleLabel->setFont(font);
        serviceFeeTitleLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #666666;\n"
"	font: 13px;\n"
"}"));

        gridLayout_3->addWidget(serviceFeeTitleLabel, 4, 0, 1, 1);

        frame_11 = new QFrame(groupBox);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setStyleSheet(QString::fromUtf8(""));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        frame_11->setProperty("cssType", QVariant(QString::fromUtf8("blueFrame")));
        horizontalLayout_14 = new QHBoxLayout(frame_11);
        horizontalLayout_14->setSpacing(0);
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalSpacer_10 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_10);

        serviceFeeTextLabel = new QLabel(frame_11);
        serviceFeeTextLabel->setObjectName(QString::fromUtf8("serviceFeeTextLabel"));
        serviceFeeTextLabel->setFont(font1);
        serviceFeeTextLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #333333;\n"
"	font: 12px bold;\n"
"}"));
        serviceFeeTextLabel->setText(QString::fromUtf8(""));

        horizontalLayout_14->addWidget(serviceFeeTextLabel);


        gridLayout_3->addWidget(frame_11, 4, 1, 1, 1);

        gridLayout_3->setColumnStretch(0, 3);
        gridLayout_3->setColumnStretch(1, 2);

        verticalLayout_2->addLayout(gridLayout_3);

        frame_2 = new QFrame(groupBox);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8(""));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame_2);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        sysMsgIconLabel = new QLabel(frame_2);
        sysMsgIconLabel->setObjectName(QString::fromUtf8("sysMsgIconLabel"));
        sizePolicy2.setHeightForWidth(sysMsgIconLabel->sizePolicy().hasHeightForWidth());
        sysMsgIconLabel->setSizePolicy(sizePolicy2);
        sysMsgIconLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/MiniWindow/icon-system-msg.png")));
        sysMsgIconLabel->setScaledContents(false);

        horizontalLayout_2->addWidget(sysMsgIconLabel);

        sysMsgTextLabel = new QLabel(frame_2);
        sysMsgTextLabel->setObjectName(QString::fromUtf8("sysMsgTextLabel"));
        sizePolicy3.setHeightForWidth(sysMsgTextLabel->sizePolicy().hasHeightForWidth());
        sysMsgTextLabel->setSizePolicy(sizePolicy3);
        sysMsgTextLabel->setMinimumSize(QSize(0, 0));
        sysMsgTextLabel->setMaximumSize(QSize(160, 16777215));
        sysMsgTextLabel->setStyleSheet(QString::fromUtf8("QLabel {\n"
"	color: #666666;\n"
"	font: 11px;\n"
"}"));
        sysMsgTextLabel->setText(QString::fromUtf8(""));
        sysMsgTextLabel->setScaledContents(true);
        sysMsgTextLabel->setWordWrap(true);

        horizontalLayout_2->addWidget(sysMsgTextLabel);


        verticalLayout_2->addWidget(frame_2);


        verticalLayout_3->addWidget(groupBox);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        sendMessageButton = new QcRibbonPushButton(dlgContentWidget);
        sendMessageButton->setObjectName(QString::fromUtf8("sendMessageButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(sendMessageButton->sizePolicy().hasHeightForWidth());
        sendMessageButton->setSizePolicy(sizePolicy4);
        sendMessageButton->setProperty("cssType", QVariant(QString::fromUtf8("bigRibbonButton")));

        gridLayout_2->addWidget(sendMessageButton, 0, 0, 1, 1);

        orderButton = new QcRibbonPushButton(dlgContentWidget);
        orderButton->setObjectName(QString::fromUtf8("orderButton"));
        sizePolicy4.setHeightForWidth(orderButton->sizePolicy().hasHeightForWidth());
        orderButton->setSizePolicy(sizePolicy4);
        orderButton->setProperty("cssType", QVariant(QString::fromUtf8("bigRibbonButton")));

        gridLayout_2->addWidget(orderButton, 0, 1, 1, 1);

        logoutButton = new QcRibbonPushButton(dlgContentWidget);
        logoutButton->setObjectName(QString::fromUtf8("logoutButton"));
        sizePolicy4.setHeightForWidth(logoutButton->sizePolicy().hasHeightForWidth());
        logoutButton->setSizePolicy(sizePolicy4);
        logoutButton->setProperty("cssType", QVariant(QString::fromUtf8("bigRibbonButton")));

        gridLayout_2->addWidget(logoutButton, 0, 2, 1, 1);

        changePwdButton = new QcRibbonPushButton(dlgContentWidget);
        changePwdButton->setObjectName(QString::fromUtf8("changePwdButton"));
        sizePolicy4.setHeightForWidth(changePwdButton->sizePolicy().hasHeightForWidth());
        changePwdButton->setSizePolicy(sizePolicy4);
        changePwdButton->setProperty("cssType", QVariant(QString::fromUtf8("bigRibbonButton")));

        gridLayout_2->addWidget(changePwdButton, 1, 0, 1, 1);

        lockButton = new QcRibbonPushButton(dlgContentWidget);
        lockButton->setObjectName(QString::fromUtf8("lockButton"));
        sizePolicy4.setHeightForWidth(lockButton->sizePolicy().hasHeightForWidth());
        lockButton->setSizePolicy(sizePolicy4);
        lockButton->setProperty("cssType", QVariant(QString::fromUtf8("bigRibbonButton")));

        gridLayout_2->addWidget(lockButton, 1, 1, 1, 1);

        utilitiesButton = new QcRibbonPushButton(dlgContentWidget);
        utilitiesButton->setObjectName(QString::fromUtf8("utilitiesButton"));
        sizePolicy4.setHeightForWidth(utilitiesButton->sizePolicy().hasHeightForWidth());
        utilitiesButton->setSizePolicy(sizePolicy4);
        utilitiesButton->setProperty("cssType", QVariant(QString::fromUtf8("bigRibbonButton")));

        gridLayout_2->addWidget(utilitiesButton, 1, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(16);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        optionButton = new QPushButton(dlgContentWidget);
        optionButton->setObjectName(QString::fromUtf8("optionButton"));
        sizePolicy4.setHeightForWidth(optionButton->sizePolicy().hasHeightForWidth());
        optionButton->setSizePolicy(sizePolicy4);
        optionButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/MiniWindow/icon-btn-settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        optionButton->setIcon(icon1);
        optionButton->setIconSize(QSize(24, 24));
        optionButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout->addWidget(optionButton);

        exitButton = new QPushButton(dlgContentWidget);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        sizePolicy4.setHeightForWidth(exitButton->sizePolicy().hasHeightForWidth());
        exitButton->setSizePolicy(sizePolicy4);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/MiniWindow/icon-btn-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitButton->setIcon(icon2);
        exitButton->setIconSize(QSize(24, 24));
        exitButton->setProperty("cssType", QVariant(QString::fromUtf8("smallBlueButton")));

        horizontalLayout->addWidget(exitButton);


        verticalLayout_3->addLayout(horizontalLayout);

        defaultAdLabel = new GClickableLabel(dlgContentWidget);
        defaultAdLabel->setObjectName(QString::fromUtf8("defaultAdLabel"));
        sizePolicy3.setHeightForWidth(defaultAdLabel->sizePolicy().hasHeightForWidth());
        defaultAdLabel->setSizePolicy(sizePolicy3);
        defaultAdLabel->setMinimumSize(QSize(0, 0));
        defaultAdLabel->setPixmap(QPixmap(QString::fromUtf8(":/extra/extra/extra1.gif")));
        defaultAdLabel->setScaledContents(true);

        verticalLayout_3->addWidget(defaultAdLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        allowTrayMsgCheckBox = new QCheckBox(dlgContentWidget);
        allowTrayMsgCheckBox->setObjectName(QString::fromUtf8("allowTrayMsgCheckBox"));
        sizePolicy1.setHeightForWidth(allowTrayMsgCheckBox->sizePolicy().hasHeightForWidth());
        allowTrayMsgCheckBox->setSizePolicy(sizePolicy1);
        allowTrayMsgCheckBox->setChecked(true);

        horizontalLayout_4->addWidget(allowTrayMsgCheckBox);

        langComboBox = new QComboBox(dlgContentWidget);
        langComboBox->setObjectName(QString::fromUtf8("langComboBox"));
        sizePolicy4.setHeightForWidth(langComboBox->sizePolicy().hasHeightForWidth());
        langComboBox->setSizePolicy(sizePolicy4);
        langComboBox->setLayoutDirection(Qt::LeftToRight);
        langComboBox->setFrame(true);

        horizontalLayout_4->addWidget(langComboBox);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(dlgContentWidget);


        retranslateUi(MiniDialog);

        QMetaObject::connectSlotsByName(MiniDialog);
    } // setupUi

    void retranslateUi(QDialog *MiniDialog)
    {
        connStatusTextLabel->setText(QApplication::translate("MiniDialog", "connected", 0, QApplication::UnicodeUTF8));
        connStatusIconLabel->setText(QString());
#ifndef QT_NO_TOOLTIP
        minimizeButton->setToolTip(QApplication::translate("MiniDialog", "minimize to system tray", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        minimizeButton->setText(QString());
        dlgContentWidget->setProperty("cssType", QVariant(QApplication::translate("MiniDialog", "dialogContent", 0, QApplication::UnicodeUTF8)));
        groupBox->setTitle(QString());
        totalTimeTitleLabel->setText(QApplication::translate("MiniDialog", "Total time:", 0, QApplication::UnicodeUTF8));
        timeUsedTitleLabel->setText(QApplication::translate("MiniDialog", "Time used:", 0, QApplication::UnicodeUTF8));
        timeLeftTitleLabel->setText(QApplication::translate("MiniDialog", "Time left:", 0, QApplication::UnicodeUTF8));
        timeFeeTitleLabel->setText(QApplication::translate("MiniDialog", "Time cost:", 0, QApplication::UnicodeUTF8));
        serviceFeeTitleLabel->setText(QApplication::translate("MiniDialog", "Service cost:", 0, QApplication::UnicodeUTF8));
        sysMsgIconLabel->setText(QString());
        sendMessageButton->setText(QString());
        orderButton->setText(QString());
        logoutButton->setText(QString());
        changePwdButton->setText(QString());
        lockButton->setText(QString());
        utilitiesButton->setText(QString());
        optionButton->setText(QApplication::translate("MiniDialog", "&Options", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("MiniDialog", "&Exit", 0, QApplication::UnicodeUTF8));
        defaultAdLabel->setText(QString());
        allowTrayMsgCheckBox->setText(QApplication::translate("MiniDialog", "Tray message", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MiniDialog);
    } // retranslateUi

};

namespace Ui {
    class MiniDialog: public Ui_MiniDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINIDIALOG_H
