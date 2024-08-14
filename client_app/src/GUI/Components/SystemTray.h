#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QWidget;
QT_END_NAMESPACE

enum MessageLevel;
class SessionManager;
class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QWidget *parent = 0);
    ~SystemTray();

    bool init();
    bool connectSigSlot();

    void retranslateUiEx();

    void showMessage(const QString& msg, MessageLevel level);
    void clearMessage();

    void show();
    void hide();

signals:
    void restoreMinidialogSignal();

public slots:
    void restoreActionSlot();
    void trayIconActivatedSlot( QSystemTrayIcon::ActivationReason reason);

private:
    void _showMsgImp(const QString& title, const QString& msg, QSystemTrayIcon::MessageIcon icon);
    void _restoreMiniDialog();

private:
    QWidget* m_parent;
    QSystemTrayIcon* m_trayIcon;
    QMenu *trayIconMenu;

    QAction *restoreAction;
    QAction *quitAction;

};

#endif // SYSTEMTRAY_H
