#include "QcIPSectorWidget.h"
#include <QtGui>
#include "ConstConfig.h"

QcIPSectorWidget::QcIPSectorWidget(QWidget *parent) : QLineEdit(parent)
{
    connect (this, SIGNAL(textChanged(QString)),
             this, SLOT(textChangedSlot(QString)));
}


void QcIPSectorWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    bool passToParent = true;

    // pre-check
    bool keyValid = verifyKeyAndNotify(key, event->modifiers(), passToParent);
    if (keyValid == false)
    {
        return;
    }

    // apply key
    if (passToParent == true)
    {
        QLineEdit::keyPressEvent(event);

    }
}

// valid key contains:
/////-----------inside sector actions-----------
// [0-9] (add digits to the sector)
// Del (to delete a key in this sector)
// other default key bindings (see QLineEdit's key bindings)
/////-----------cross sector actions---------
// left/right arrow (move the cursor inside the IP address)
// shift + left/right arror (move cursur to another sector if nothing is selectable, otherwise select text and move cursor)
// home/end (move to the IP head or tail)
// Backspace (to delete a key in the whole IP address)
// '.' (if the sector has at least a digit, jump to next sector; else do nothing)
// Tab key (go to next sector)
/////-----------invalid actions---------
// ctrl + backspace
bool QcIPSectorWidget::verifyKeyAndNotify( int key, Qt::KeyboardModifiers modifiers, 
                                          /*out*/bool& passToParent )
{
    bool valid = false;
    passToParent = false;

    bool ctrlPressed = (modifiers & Qt::ControlModifier) != 0;
    bool shiftPressed = (modifiers & Qt::ShiftModifier) != 0;
    //bool altPressed = (modifiers & Qt::AltModifier) != 0;
    // if user enables numpad, we allow it (enter ip address from numpad)
    bool noModifier = (modifiers == Qt::NoModifier || modifiers == Qt::KeypadModifier);

    int cursorPosition = QLineEdit::cursorPosition();
    int textLength = this->text().length();
    bool cursorAtHead = (cursorPosition == 0);
    bool cursorAtTail = (cursorPosition == textLength);
    bool textSelected = (this->selectedText().length() > 0);

    // number pressed
    if (key >= (int)('0') &&
        key <= (int)('9') &&
        noModifier == true)
    {
        // if text is selected, then the selected part will be replaced by the new input
        // so it shouldn't be restricted by the 3 char limitation
        if (textLength < 3 ||
            textSelected == true)
        {
            int number = key - (int)('0');
            emit numberPressed(number);
            passToParent = true;
            valid = true;
        }
        else
        {
            valid = false;
        }
    }
    
    // default system key bindings
    if (
        (key == Qt::Key::Key_Delete && noModifier == true) || // delete pressed
        (key == Qt::Key_Delete && ctrlPressed == true) || // ctrl + delete
        ((key == (int)'a' || key == (int)'A') && ctrlPressed == true) || // ctrl + a
        ((key == (int)'c' || key == (int)'C') && ctrlPressed == true) || // ctrl + c
        (key == Qt::Key_Insert && ctrlPressed == true) || // ctrl + insert
        ((key == (int)'k' || key == (int)'K') && ctrlPressed == true) || // ctrl + k
        ((key == (int)'v' || key == (int)'V') && ctrlPressed == true) || // ctrl + v
        (key == Qt::Key_Insert && shiftPressed == true) || // shift + insert
        ((key == (int)'x' || key == (int)'X') && ctrlPressed == true) || // ctrl + x
        (key == Qt::Key_Delete && shiftPressed == true) || // shift + delete
        ((key == (int)'z' || key == (int)'Z') && ctrlPressed == true) || // ctrl + z
        ((key == (int)'y' || key == (int)'Y') && ctrlPressed == true) // ctrl + y
        )
    {
        passToParent = true;
        valid = true;
    }

    // handle "<---" key
    if (key == Qt::Key_Left && noModifier == true)
    {
        valid = true;
        if (cursorAtHead == true)
        {
            emit movingToPreviousSector(CursorBehavior::Cursor_MoveToTail);
            passToParent = false;
        }
        else
        {
            passToParent = true;
        }
    }
    // handle "--->" key
    if (key == Qt::Key_Right && noModifier == true)
    {
        valid = true;
        if (cursorAtTail == true)
        {
            emit movingToNextSector(CursorBehavior::Cursor_MoveToHead);
            passToParent = false;
        }
        else
        {
            passToParent = true;
        }
    }
    // handle shift + left/right arrow key
    // handle shift + left (if no text is selected, move to previous sector if at head)
    if (key == Qt::Key_Left && shiftPressed == true)
    {
        valid = true;
        if (textSelected == true)
        {
            passToParent = true;
        }
        else
        {
            if (cursorAtHead == true)
            {
                emit movingToPreviousSector(CursorBehavior::Cursor_MoveToTail);
                passToParent = false;
            }
        }
    }

    // handle shift + right (if no text is selected, move to next sector if at tail)
    if (key == Qt::Key_Right && shiftPressed == true)
    {
        valid = true;
        if (textSelected == true)
        {
            passToParent = true;
        }
        else
        {
            if (cursorAtTail == true)
            {
                emit movingToNextSector(CursorBehavior::Cursor_MoveToHead);
                passToParent = false;
            }
        }
    }

    // handle home key
    if (key == Qt::Key_Home && noModifier == true)
    {
        valid = true;
        emit movingToIPHead();
        passToParent = false;
    }

    // handle end key
    if (key == Qt::Key_End && noModifier == true)
    {
        valid = true;
        emit movingToIPTail();
        passToParent = false;
    }

    if (key == Qt::Key_Backspace && noModifier == true)
    {
        valid = true;
        if (cursorAtHead == true)
        {
            emit backspacePassedToPreviousSector();
            passToParent = false;
        }
        else
        {
            passToParent = true;
        }
    }

    // press dot char (if the sector has at least a digit and cursor at tail and no text selected, jump to next sector; else do nothing)
    if (key == (int)'.' && noModifier == true)
    {
        valid = true;
        passToParent = false;
        if (textLength > 0 &&
            cursorAtTail == true &&
            textSelected == false)
        {
            emit dotPressed();
        }
    }

    if (key == Qt::Key_Tab)
    {
        valid = true;
        passToParent = true;
    }

    return valid;
}

void QcIPSectorWidget::textChangedSlot(const QString &text)
{
    if(text.length() == 3)
    {
        emit movingToNextSector(CursorBehavior::Cursor_SelectText);
    }
}
