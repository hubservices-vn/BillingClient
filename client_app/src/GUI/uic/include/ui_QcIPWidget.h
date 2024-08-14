/********************************************************************************
** Form generated from reading UI file 'QcIPWidget.ui'
**
** Created: Wed Mar 25 20:42:54 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCIPWIDGET_H
#define UI_QCIPWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include <qcipsectorwidget.h>

QT_BEGIN_NAMESPACE

class Ui_QcIPWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QcIPSectorWidget *sectorAEdit;
    QLabel *label;
    QcIPSectorWidget *sectorBEdit;
    QLabel *label_2;
    QcIPSectorWidget *sectorCEdit;
    QLabel *label_3;
    QcIPSectorWidget *sectorDEdit;

    void setupUi(QWidget *QcIPWidget)
    {
        if (QcIPWidget->objectName().isEmpty())
            QcIPWidget->setObjectName(QString::fromUtf8("QcIPWidget"));
        QcIPWidget->resize(135, 19);
        QcIPWidget->setWindowTitle(QString::fromUtf8(""));
        QcIPWidget->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"	background-color:white;\n"
"	border: 0;\n"
"}\n"
"QLabel[category=\"ipSeparator\"] {\n"
"	border: 0;\n"
"	background-color:white;\n"
"}"));
        horizontalLayout = new QHBoxLayout(QcIPWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sectorAEdit = new QcIPSectorWidget(QcIPWidget);
        sectorAEdit->setObjectName(QString::fromUtf8("sectorAEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sectorAEdit->sizePolicy().hasHeightForWidth());
        sectorAEdit->setSizePolicy(sizePolicy);
        sectorAEdit->setMinimumSize(QSize(20, 0));
        sectorAEdit->setMaximumSize(QSize(30, 16777215));
        sectorAEdit->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(sectorAEdit);

        label = new QLabel(QcIPWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setText(QString::fromUtf8("."));
        label->setAlignment(Qt::AlignCenter);
        label->setProperty("category", QVariant(QString::fromUtf8("ipSeparator")));

        horizontalLayout->addWidget(label);

        sectorBEdit = new QcIPSectorWidget(QcIPWidget);
        sectorBEdit->setObjectName(QString::fromUtf8("sectorBEdit"));
        sizePolicy.setHeightForWidth(sectorBEdit->sizePolicy().hasHeightForWidth());
        sectorBEdit->setSizePolicy(sizePolicy);
        sectorBEdit->setMinimumSize(QSize(20, 0));
        sectorBEdit->setMaximumSize(QSize(30, 16777215));
        sectorBEdit->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(sectorBEdit);

        label_2 = new QLabel(QcIPWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setText(QString::fromUtf8("."));
        label_2->setAlignment(Qt::AlignCenter);
        label_2->setProperty("category", QVariant(QString::fromUtf8("ipSeparator")));

        horizontalLayout->addWidget(label_2);

        sectorCEdit = new QcIPSectorWidget(QcIPWidget);
        sectorCEdit->setObjectName(QString::fromUtf8("sectorCEdit"));
        sizePolicy.setHeightForWidth(sectorCEdit->sizePolicy().hasHeightForWidth());
        sectorCEdit->setSizePolicy(sizePolicy);
        sectorCEdit->setMinimumSize(QSize(20, 0));
        sectorCEdit->setMaximumSize(QSize(30, 16777215));
        sectorCEdit->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(sectorCEdit);

        label_3 = new QLabel(QcIPWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setText(QString::fromUtf8("."));
        label_3->setAlignment(Qt::AlignCenter);
        label_3->setProperty("category", QVariant(QString::fromUtf8("ipSeparator")));

        horizontalLayout->addWidget(label_3);

        sectorDEdit = new QcIPSectorWidget(QcIPWidget);
        sectorDEdit->setObjectName(QString::fromUtf8("sectorDEdit"));
        sizePolicy.setHeightForWidth(sectorDEdit->sizePolicy().hasHeightForWidth());
        sectorDEdit->setSizePolicy(sizePolicy);
        sectorDEdit->setMinimumSize(QSize(20, 0));
        sectorDEdit->setMaximumSize(QSize(30, 16777215));
        sectorDEdit->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(sectorDEdit);


        retranslateUi(QcIPWidget);

        QMetaObject::connectSlotsByName(QcIPWidget);
    } // setupUi

    void retranslateUi(QWidget *QcIPWidget)
    {
        Q_UNUSED(QcIPWidget);
    } // retranslateUi

};

namespace Ui {
    class QcIPWidget: public Ui_QcIPWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCIPWIDGET_H
