#ifndef CHANGEPWDDIALOG_H
#define CHANGEPWDDIALOG_H

#include <QDialog>

namespace Ui {
    class ChangePwdDialog;
}

class ChangePwdDialog : public QDialog 
{
    Q_OBJECT

public:
    explicit ChangePwdDialog(QWidget *parent = 0);
    ~ChangePwdDialog();

    QString oldPwd() const { return m_oldPwd; }
    QString newPwd() const { return m_newPwd; }

signals:
    
public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void enableDisableButtons ( const QString & text );

protected:
    void changeEvent(QEvent *e);

private:
    void retranslateUiEx();

private:
    Ui::ChangePwdDialog* ui;
    QString m_oldPwd;
    QString m_newPwd;
};

#endif // CHANGEPWDDIALOG_H
