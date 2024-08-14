/********************************************************************************
** Form generated from reading UI file 'OrderDialog.ui'
**
** Created: Wed Mar 25 20:42:54 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORDERDIALOG_H
#define UI_ORDERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OrderDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *placeOrderButton;
    QLabel *totalPriceTitleLabel;
    QLabel *totalPriceTextLabel;

    void setupUi(QDialog *OrderDialog)
    {
        if (OrderDialog->objectName().isEmpty())
            OrderDialog->setObjectName(QString::fromUtf8("OrderDialog"));
        OrderDialog->resize(611, 476);
        verticalLayout = new QVBoxLayout(OrderDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableWidget = new QTableWidget(OrderDialog);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(tableWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(15);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 6, 12, 6);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(OrderDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Common/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelButton->setIcon(icon);
        cancelButton->setProperty("cssType", QVariant(QString::fromUtf8("bigBlackButton")));

        horizontalLayout->addWidget(cancelButton);

        placeOrderButton = new QPushButton(OrderDialog);
        placeOrderButton->setObjectName(QString::fromUtf8("placeOrderButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Common/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        placeOrderButton->setIcon(icon1);
        placeOrderButton->setProperty("cssType", QVariant(QString::fromUtf8("bigBlueButton")));

        horizontalLayout->addWidget(placeOrderButton);

        totalPriceTitleLabel = new QLabel(OrderDialog);
        totalPriceTitleLabel->setObjectName(QString::fromUtf8("totalPriceTitleLabel"));

        horizontalLayout->addWidget(totalPriceTitleLabel);

        totalPriceTextLabel = new QLabel(OrderDialog);
        totalPriceTextLabel->setObjectName(QString::fromUtf8("totalPriceTextLabel"));

        horizontalLayout->addWidget(totalPriceTextLabel);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(tableWidget, placeOrderButton);
        QWidget::setTabOrder(placeOrderButton, cancelButton);

        retranslateUi(OrderDialog);

        QMetaObject::connectSlotsByName(OrderDialog);
    } // setupUi

    void retranslateUi(QDialog *OrderDialog)
    {
        OrderDialog->setWindowTitle(QApplication::translate("OrderDialog", "Ordering", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("OrderDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        placeOrderButton->setText(QApplication::translate("OrderDialog", "Place Order", 0, QApplication::UnicodeUTF8));
        totalPriceTitleLabel->setText(QApplication::translate("OrderDialog", "Total price:", 0, QApplication::UnicodeUTF8));
        totalPriceTextLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class OrderDialog: public Ui_OrderDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORDERDIALOG_H
