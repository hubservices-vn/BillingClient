#include "QcIPWidget.h"
#include "ui_QcIPWidget.h"
#include <QRegexp>
#include "ConstConfig.h"

QcIPWidget::QcIPWidget(QWidget *parent) :
QWidget(parent), ui(new Ui::QcIPWidget)
{
    ui->setupUi(this);
    connectSigSlot();
}

QcIPWidget::~QcIPWidget()
{
    delete ui;
}

bool QcIPWidget::connectSigSlot()
{
    // Sector A sigslots
    {
        connect(ui->sectorAEdit, SIGNAL(dotPressed()),
            this, SLOT(dotPressedSlot()));
        connect(ui->sectorAEdit, SIGNAL(backspacePassedToPreviousSector()),
            this, SLOT(backspacePassedToPreviousSectorSlot()));
        connect(ui->sectorAEdit, SIGNAL(movingToPreviousSector(CursorBehavior)),
            this, SLOT(movingToPreviousSectorSlot(CursorBehavior)));
        connect(ui->sectorAEdit, SIGNAL(movingToNextSector(CursorBehavior)),
            this, SLOT(movingToNextSectorSlot(CursorBehavior)));
        connect(ui->sectorAEdit, SIGNAL(movingToIPHead()),
            this, SLOT(movingToIPHeadSlot()));
        connect(ui->sectorAEdit, SIGNAL(movingToIPTail()),
            this, SLOT(movingToIPTailSlot()));
    }
    
    // Sector B sigslots
    {
        connect(ui->sectorBEdit, SIGNAL(dotPressed()),
            this, SLOT(dotPressedSlot()));
        connect(ui->sectorBEdit, SIGNAL(backspacePassedToPreviousSector()),
            this, SLOT(backspacePassedToPreviousSectorSlot()));
        connect(ui->sectorBEdit, SIGNAL(movingToPreviousSector(CursorBehavior)),
            this, SLOT(movingToPreviousSectorSlot(CursorBehavior)));
        connect(ui->sectorBEdit, SIGNAL(movingToNextSector(CursorBehavior)),
            this, SLOT(movingToNextSectorSlot(CursorBehavior)));
        connect(ui->sectorBEdit, SIGNAL(movingToIPHead()),
            this, SLOT(movingToIPHeadSlot()));
        connect(ui->sectorBEdit, SIGNAL(movingToIPTail()),
            this, SLOT(movingToIPTailSlot()));
    }

    // Sector C sigslots
    {
        connect(ui->sectorCEdit, SIGNAL(dotPressed()),
            this, SLOT(dotPressedSlot()));
        connect(ui->sectorCEdit, SIGNAL(backspacePassedToPreviousSector()),
            this, SLOT(backspacePassedToPreviousSectorSlot()));
        connect(ui->sectorCEdit, SIGNAL(movingToPreviousSector(CursorBehavior)),
            this, SLOT(movingToPreviousSectorSlot(CursorBehavior)));
        connect(ui->sectorCEdit, SIGNAL(movingToNextSector(CursorBehavior)),
            this, SLOT(movingToNextSectorSlot(CursorBehavior)));
        connect(ui->sectorCEdit, SIGNAL(movingToIPHead()),
            this, SLOT(movingToIPHeadSlot()));
        connect(ui->sectorCEdit, SIGNAL(movingToIPTail()),
            this, SLOT(movingToIPTailSlot()));
    }

    // Sector D sigslots
    {
        // when user finishes inputing and press "return",
        // forward the signal to parent dialog/form
        connect(ui->sectorDEdit, SIGNAL(returnPressed()),
            this, SIGNAL(inputFinished()));

        connect(ui->sectorDEdit, SIGNAL(dotPressed()),
            this, SLOT(dotPressedSlot()));
        connect(ui->sectorDEdit, SIGNAL(backspacePassedToPreviousSector()),
            this, SLOT(backspacePassedToPreviousSectorSlot()));
        connect(ui->sectorDEdit, SIGNAL(movingToPreviousSector(CursorBehavior)),
            this, SLOT(movingToPreviousSectorSlot(CursorBehavior)));
        connect(ui->sectorDEdit, SIGNAL(movingToNextSector(CursorBehavior)),
            this, SLOT(movingToNextSectorSlot(CursorBehavior)));
        connect(ui->sectorDEdit, SIGNAL(movingToIPHead()),
            this, SLOT(movingToIPHeadSlot()));
        connect(ui->sectorDEdit, SIGNAL(movingToIPTail()),
            this, SLOT(movingToIPTailSlot()));
    }

    return true;
}

void QcIPWidget::dotPressedSlot()
{
    movingToNextSectorSlot(CursorBehavior::Cursor_SelectText);
}

void QcIPWidget::backspacePassedToPreviousSectorSlot()
{
    movingToPreviousSectorSlot(Cursor_MoveToTail);
    QLineEdit* focusedEdit = dynamic_cast<QLineEdit*>(this->focusWidget());
    if (focusedEdit == NULL)
    {
        return;
    }

    QString oldText = focusedEdit->text();
    if (oldText.length() > 0)
    {
        QString newText = oldText.left(oldText.length() - 1);
        focusedEdit->setText(newText);
    }
}

void QcIPWidget::movingToPreviousSectorSlot(CursorBehavior behavior)
{
    if (ui->sectorAEdit->hasFocus())
    {
        return;
    }
    this->setUpdatesEnabled(false);
    {
        focusPreviousChild();
        // put the cursor at the tail of previous sector
        QLineEdit* focusedEdit = dynamic_cast<QLineEdit*>(this->focusWidget());
        if (focusedEdit == NULL)
        {
            return;
        }
        _locateCursor(focusedEdit, behavior);
    }
    this->setUpdatesEnabled(true);
}

void QcIPWidget::movingToNextSectorSlot(CursorBehavior behavior)
{
    if (ui->sectorDEdit->hasFocus())
    {
        return;
    }
    this->setUpdatesEnabled(false);
    {
        focusNextChild();
        // put the cursor at the head of next sector
        QLineEdit* focusedEdit = dynamic_cast<QLineEdit*>(this->focusWidget());
        if (focusedEdit == NULL)
        {
            return;
        }
        
        _locateCursor(focusedEdit, behavior);
    }
    this->setUpdatesEnabled(true);
}

void QcIPWidget::movingToIPHeadSlot()
{
    ui->sectorAEdit->setUpdatesEnabled(false);
    {
        ui->sectorAEdit->setFocus();
        ui->sectorAEdit->setCursorPosition(0);
    }
    ui->sectorAEdit->setUpdatesEnabled(true);
}

void QcIPWidget::movingToIPTailSlot()
{
    ui->sectorDEdit->setUpdatesEnabled(false);
    {
        int textLength = ui->sectorDEdit->text().length();
        ui->sectorDEdit->setFocus();
        ui->sectorDEdit->setCursorPosition(textLength);
    }
    ui->sectorDEdit->setUpdatesEnabled(true);
}

void QcIPWidget::_locateCursor(QLineEdit *lineEdit, CursorBehavior behavior)
{
    lineEdit->setUpdatesEnabled(false);
    {
        switch(behavior)
        {
        case CursorBehavior::Cursor_MoveToHead :
        {
            lineEdit->setCursorPosition(0);
        }
            break;
        case CursorBehavior::Cursor_MoveToTail :
        {
            int textLength = ui->sectorDEdit->text().length();
            lineEdit->setCursorPosition(textLength);
        }
            break;
        case CursorBehavior::Cursor_SelectText :
        {
            lineEdit->selectAll();
        }
            break;
        }
    }
    lineEdit->setUpdatesEnabled(true);
}

const QString IP_SEPARATOR = ".";
const QString IP_REGEXP_PTN = "^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$";
QString QcIPWidget::ipAddress(bool& valid)
{
    QString ipAddr = 
        ui->sectorAEdit->text() + IP_SEPARATOR +
        ui->sectorBEdit->text() + IP_SEPARATOR +
        ui->sectorCEdit->text() + IP_SEPARATOR +
        ui->sectorDEdit->text();
    QRegExp rx(IP_REGEXP_PTN);
    valid = rx.exactMatch(ipAddr);

    return ipAddr;
}

void QcIPWidget::setIPAddress( const QString& ipAddr )
{
    QRegExp rx(IP_REGEXP_PTN);
    bool valid = rx.exactMatch(ipAddr);
    if (valid == false)
    {
        return;
    }
    QStringList secList = ipAddr.split(IP_SEPARATOR, QString::SkipEmptyParts, Qt::CaseInsensitive);
    if (secList.count() == 4)
    {
        ui->sectorAEdit->setText(secList[0]);
        ui->sectorBEdit->setText(secList[1]);
        ui->sectorCEdit->setText(secList[2]);
        ui->sectorDEdit->setText(secList[3]);

        // make section A selected
        ui->sectorAEdit->setFocus();
        ui->sectorAEdit->selectAll();
    }
}
