#include "GPlainTextEdit.h"
#include <QtGui>
#include "log.h"

GPlainTextEdit::GPlainTextEdit(QWidget *parent) : QPlainTextEdit(parent), m_maxTextLength(INT_MAX)
{
}

void GPlainTextEdit::keyPressEvent( QKeyEvent *e )
{
    int keyVal = e->key();

    bool ctrlDown = false;
    bool altDown = false;
    bool shiftDown = false;
    if (e->modifiers() & Qt::ControlModifier)
    {
        ctrlDown = true;
    }
    if (e->modifiers() & Qt::AltModifier)
    {
        altDown = true;
    }
    if (e->modifiers() & Qt::ShiftModifier)
    {
        shiftDown = true;
    }

    LOG_DEBUG4 << "Key pressed. keyval: " << keyVal 
        << ", text: " << qPrintable(e->text())
        << ", ctrlDown: " << ctrlDown
        << ", altDown: " << altDown
        << ", shiftDown: " << shiftDown;

    if (keyVal == Qt::Key_Enter)
    {
        if (ctrlDown || shiftDown)
        {
            this->appendPlainText("\r\n");
            e->ignore();
            return;
        }
        else
        {
            emit returnPressed();
            e->ignore();
            return;
        }
    }

    int textLength = this->toPlainText().length();
    // doesn't meet the max limit yet
    if (textLength < m_maxTextLength)
    {
        QPlainTextEdit::keyPressEvent(e);
        e->ignore();
        return;
    }

    // already meet the limit
    // if ctrl or alt is down, there's no way to input chars, so don't worry when one of them is down
    if (!(ctrlDown || altDown))
    {
        // these are printable chars
        if (keyVal >= 32 && keyVal <= 126)
        {
            e->ignore();
            return;
        }

        // "tab" key to input multi spaces at once
        if (keyVal == Qt::Key_Tab)
        {
            e->ignore();
            return;
        }
    }

    // user may press "backspace" or other function keys
    QPlainTextEdit::keyPressEvent(e);
}

void GPlainTextEdit::keyReleaseEvent( QKeyEvent *e )
{
    //int keyVal = e->key();

    //bool ctrlDown = false;
    //bool altDown = false;
    //bool shiftDown = false;
    //if (e->modifiers() & Qt::ControlModifier)
    //{
    //    ctrlDown = true;
    //}
    //if (e->modifiers() & Qt::AltModifier)
    //{
    //    altDown = true;
    //}
    //if (e->modifiers() & Qt::ShiftModifier)
    //{
    //    shiftDown = true;
    //}

    QPlainTextEdit::keyReleaseEvent(e);
}
