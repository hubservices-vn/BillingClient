#ifndef QCIPWIDGET_H
#define QCIPWIDGET_H

#include <QWidget>

namespace Ui {
class QcIPWidget;
}

class QLineEdit;
enum CursorBehavior;
class QcIPWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QcIPWidget(QWidget *parent = 0);
    ~QcIPWidget();
    
public:
    bool connectSigSlot();

public:
    QString ipAddress(bool& valid);
    void setIPAddress(const QString& ip);

signals:
    void inputFinished();

public slots:
    void dotPressedSlot();
    void backspacePassedToPreviousSectorSlot();
    void movingToPreviousSectorSlot(CursorBehavior behavior);
    void movingToNextSectorSlot(CursorBehavior behavior);
    void movingToIPHeadSlot();
    void movingToIPTailSlot();
private:
    void _locateCursor(QLineEdit* lineEdit, CursorBehavior behavior);
private:
    Ui::QcIPWidget *ui;
};

#endif // QCIPWIDGET_H
