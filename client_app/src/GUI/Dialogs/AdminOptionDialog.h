#ifndef ADMINOPTIONDIALOG_H
#define ADMINOPTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class AdminOptionDialog;
}

class AdminOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdminOptionDialog(QWidget *parent = 0);
    ~AdminOptionDialog();

public slots:
    void on_changeButton_clicked();
    void on_closeButton_clicked();

    void displayConnectionStatusSlot();

    void adminOptionReplyRecvedSlot(int opTypeVal, int resultVal);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AdminOptionDialog *ui;
};

#endif // ADMINOPTIONDIALOG_H
