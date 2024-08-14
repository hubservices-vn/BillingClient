#ifndef QCIPSECTORWIDGET_H
#define QCIPSECTORWIDGET_H

#pragma warning(disable: 4482)

#include <QLineEdit>
#include <QNamespace.h>

enum CursorBehavior;
class QcIPSectorWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit QcIPSectorWidget(QWidget *parent = 0);
    
signals:
    void numberPressed(int number);
    void dotPressed(); // move to next sector
    void backspacePassedToPreviousSector();
    void movingToPreviousSector(CursorBehavior behavior); // raised by left arrow key
    void movingToNextSector(CursorBehavior behavior); // raised by right arrow key
    void movingToIPHead(); // move to head of sector A
    void movingToIPTail(); // move to tail of sector D
public slots:
    void textChangedSlot(const QString& text);
protected:
    virtual void keyPressEvent ( QKeyEvent * event );

private:
    // param passToParent: if the value is true, we'll let QLineEdit
    // to handle the keyPressEvent
    bool verifyKeyAndNotify(int keyValue, Qt::KeyboardModifiers modifiers, /*out*/bool& passToParent);
};

#endif // QCIPSECTORWIDGET_H
