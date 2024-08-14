#ifndef GPLAINTEXTEDIT_H
#define GPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class GPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit GPlainTextEdit(QWidget *parent = 0);

    // properties
public:
    int maxTextLength() const { return m_maxTextLength; }
    void setMaxTextLength(int val) { m_maxTextLength = val; }

signals:
    void returnPressed();

public slots:

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    int m_maxTextLength;

};

#endif // GPLAINTEXTEDIT_H
