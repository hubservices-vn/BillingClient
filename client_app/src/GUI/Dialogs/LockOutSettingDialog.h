#ifndef LOCKOUTSETTINGDIALOG_H
#define LOCKOUTSETTINGDIALOG_H

namespace Ui{
    class LockOutSettingDialog;
}

#include <QDialog>

class LockOutSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LockOutSettingDialog(QWidget *parent = 0);

signals:

public slots:
    void on_lockButton_clicked();
    void on_cancelButton_clicked();
    void on_pwdLineEdit_textChanged( const QString & text) ;
    void on_pwdRptLineEdit_textChanged( const QString & text) ;

    void screenLockReplyRecvedSlot(int modeVal, bool result);

private:
    void _verifyPwds();

private:
    Ui::LockOutSettingDialog* ui;
};

#endif // LOCKOUTSETTINGDIALOG_H
