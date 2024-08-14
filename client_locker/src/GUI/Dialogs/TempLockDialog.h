#ifndef TEMPLOCKDIALOG_H
#define TEMPLOCKDIALOG_H

namespace Ui {
    class TempLockDialog;
}

#include <QDialog>
#include <QShowEvent>

class TempLockDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TempLockDialog(QWidget *parent = 0);
    ~TempLockDialog();

protected: // event handler
    // translate string
    void changeEvent(QEvent *e);

    // reset the text fields
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *);

signals:

public slots:
    void on_unlockButton_clicked();
    void on_unlocByAccountkButton_clicked();
    
    void screenUnlockReplyRecvedSlot(int modeVal, bool result);

private:
    void retranslateUiEx();

private:
    Ui::TempLockDialog* ui;
};

#endif // TEMPLOCKDIALOG_H
