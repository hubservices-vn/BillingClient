#ifndef MINIDIALOG_H
#define MINIDIALOG_H

#include <QDialog>
#include <QScopedPointer>
#include <QMenu>
#include <QAction>
#include <QMovie>

namespace Ui {
    class MiniDialog;
}

class SystemTray;

enum NetworkResult;
enum SocketStatus;
enum MessageLevel;
class SessionManager;
class cc2csNetworkManager;
class MiniDialog : public QDialog
{
    Q_OBJECT
public:
    MiniDialog(QWidget* parent = NULL);
    ~MiniDialog();

    bool init();
    bool connectSigSlot();

public: // accessors
    static bool isAlive() { return s_isAlive; }

    // show msg to history panel
    void showMessage(const QString& msg, MessageLevel level);

signals:
    void languageChanged();

public slots:
    void networkStateChangedSlot(/*NetworkResult*/int result);
    void on_orderButton_clicked();
    void on_sendMessageButton_clicked();
    void on_logoutButton_clicked();
    void on_changePwdButton_clicked();
    void on_lockButton_clicked();
    void on_utilitiesButton_clicked();

    void on_minimizeButton_clicked();

    void on_allowTrayMsgCheckBox_stateChanged();
    void on_langComboBox_currentIndexChanged(int index);

    void on_topupByCouponButton_clicked();
    // admin only
    void on_optionButton_clicked();
    void on_exitButton_clicked();

    void on_defaultAdLabel_clicked();



    // the time passes, update the "time used" and "time left" fields
    void sessionUpdatedSlot();

    void sessionEndedSlot();
//
//    // the user runs out of time, close the client and lock the screen
//    void sessionOOTSlot();

    // the user requests to change password and get response from server
    void changeAccPwdReplyRecvedSlot( /*ChangeAccPwdResult*/int result );

    // the client service is about to perform a scheduled shutdown, notify user
    void aboutToShutdownComputerSlot(const QTime &autoShutdownTime);

    //
    // context menu slots
    //
    void adjustSoundSlot(); // mmsys.cpl
    void adjustMouseSlot(); // main.cpl
    void adjustDisplaySlot(); // desk.cpl

    void onClientAdFileChangedSlot();


protected: // QDialog events
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    void _initContextMenu();

    void retranslateUiEx();
    void addTextToMsgArea(const QString& msg, const QString &textColor);
    // if the connections breaks, disable the "send message", "order product", "change password" buttons
    // if connection resumes, should enable those buttons
    void _displayConnectionStatus();
    void _updateButtonStatus();

    void _relocateDialog();

private:
    //// server tells client to shutdown itself
    //void _logoutAndEndSession();

    // refresh the "total minutes", "minutes used", "minutes left" fields
    void _updateSessionStatus();

private:
    SystemTray* _getSystemTray();

private:
    Ui::MiniDialog* ui;
    SystemTray* m_systemTray;

    QMenu* m_utilitiesContextMenu;
    QAction* m_adjustSoundAction;
    QAction* m_adjustMouseAction;
    QAction* m_adjustDisplayAction;

private: // helper
    enum DlgCloseAction
    {
        DlgCloseAction_NA = -1,
        // this close event is triggered by the "close" button 
        // on top-right corner of mini dialog
        DlgCloseAction_Hide = 1,
        // this close event is triggered by the OS shutdown/reboot
        // so if we ignore that, the computer has no way to shutdown
        DlgCloseAction_Close = 2,
    };

    // indicate whether the dialog close action 
    DlgCloseAction m_closeAction;

    static bool s_isAlive;

private: // reference
    SessionManager* m_sessionMgr;
    cc2csNetworkManager* m_networkMgr;
	QMovie *m_movie;
};

#endif // MINIDIALOG_H
