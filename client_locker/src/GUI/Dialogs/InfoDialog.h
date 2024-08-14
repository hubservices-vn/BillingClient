#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
    class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    InfoDialog(QWidget* parent = 0);
    ~InfoDialog();

    void closeDialog();

public: // methods
    bool connectSigSlot();

protected:
    // translate
    void changeEvent(QEvent *e);

    //// hide this dialog when user presses "ESC" or "ALT + F4"
    //void keyPressEvent(QKeyEvent* event);

    void hideEvent( QHideEvent* event );
    void closeEvent( QCloseEvent* event );
    // 1. clear all the text fields and msg label;
    // 2. set the login dialog at center of screen;
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private:
    void retranslateUiEx();
private:
    Ui::InfoDialog* ui;

    enum DlgCloseAction
    {
        DlgClose_NA = -1,
        DlgClose_Default = 1,
        DlgClose_LoginSucceeded = 2,
    };

    // indicate whether the dialog close action
    DlgCloseAction m_closeAction;
};

#endif // INFODIALOGQT_H
