#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget* parent = 0);
    ~LoginDialog();

    void closeDialog();

public: // methods
    bool connectSigSlot();

protected:
    // translate
    void changeEvent(QEvent *e);

    //// hide this dialog when user presses "ESC" or "ALT + F4"
    //void keyPressEvent(QKeyEvent* event);

    // 1. clear all the text fields and msg label;
    // 2. set the login dialog at center of screen;
    void showEvent(QShowEvent *event);
    // triggered by login successful, so shouldn't hook to keyboard/mouse event,
    // as the user is using the pc
    void hideEvent(QHideEvent *); 
    // triggered by "on_closeButton_clicked" method when user clicks the "cancel" button in login dialog
    // the pc still is NOT in use, so MUST ENSURE the hook is active
    void closeEvent(QCloseEvent *e);

private slots:
    void on_userLoginButton_clicked();
    void on_closeButton_clicked();

    // show the login failure reason to UI
    void loginReplyRecvedSlot(/*LoginResult*/int resultVal, int accTypeVal);

private:
    void retranslateUiEx();

private:
    Ui::LoginDialog* ui;

    enum DlgCloseAction
    {
        DlgClose_NA = -1,
        DlgClose_Default = 1,
        DlgClose_LoginSucceeded = 2,
    };

    // indicate whether the dialog close action 
    DlgCloseAction m_closeAction;
};

#endif // LOGINDIALOGQT_H
