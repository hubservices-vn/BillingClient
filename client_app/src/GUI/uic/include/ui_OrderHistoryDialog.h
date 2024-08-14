/********************************************************************************
** Form generated from reading UI file 'OrderHistoryDialog.ui'
**
** Created: Wed Mar 25 20:42:53 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORDERHISTORYDIALOG_H
#define UI_ORDERHISTORYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OrderHistoryDialog
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeButton;

    void setupUi(QDialog *OrderHistoryDialog)
    {
        if (OrderHistoryDialog->objectName().isEmpty())
            OrderHistoryDialog->setObjectName(QString::fromUtf8("OrderHistoryDialog"));
        OrderHistoryDialog->resize(380, 271);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/OrderDialog/order_history.png"), QSize(), QIcon::Normal, QIcon::Off);
        OrderHistoryDialog->setWindowIcon(icon);
        horizontalLayout_2 = new QHBoxLayout(OrderHistoryDialog);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        treeWidget = new QTreeWidget(OrderHistoryDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout->addWidget(treeWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        closeButton = new QPushButton(OrderHistoryDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/OrderDialog/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon1);

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(OrderHistoryDialog);

        QMetaObject::connectSlotsByName(OrderHistoryDialog);
    } // setupUi

    void retranslateUi(QDialog *OrderHistoryDialog)
    {
        OrderHistoryDialog->setWindowTitle(QApplication::translate("OrderHistoryDialog", "Order history", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("OrderHistoryDialog", "&Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OrderHistoryDialog: public Ui_OrderHistoryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORDERHISTORYDIALOG_H
