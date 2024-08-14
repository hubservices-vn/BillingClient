#ifndef TOPUPBYCOUPONDIALOG_H
#define TOPUPBYCOUPONDIALOG_H

#include <QDialog>

namespace Ui {
    class TopupByCouponDialog;
}

class TopupByCouponDialog : public QDialog 
{
    Q_OBJECT

public:
    explicit TopupByCouponDialog(QWidget *parent = 0);
    ~TopupByCouponDialog();

    QString couponAcc() const { return m_couponAcc; }
    QString couponPwd() const { return m_couponPwd; }

signals:
    
public slots:
    void on_topupButton_clicked();
    void on_closeButton_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void retranslateUiEx();

private:
    Ui::TopupByCouponDialog* ui;
    QString m_couponAcc;
    QString m_couponPwd;
};

#endif // TOPUPBYCOUPONDIALOG_H
