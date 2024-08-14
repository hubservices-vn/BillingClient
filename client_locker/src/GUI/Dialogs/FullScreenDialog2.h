#ifndef FULLSCREENDIALOG2QT_H
#define FULLSCREENDIALOG2QT_H

#include <QDialog>

class QTimer;
class uiFullScreenDialog2;
enum SocketStatus;
enum NetworkResult;

class FullScreenDialog2 : public QDialog
{
    Q_OBJECT
public:
    FullScreenDialog2(QWidget* parent = NULL);
    ~FullScreenDialog2();

    bool init();
    void connectSigSlot();

    void showDialog();
    void closeDialog();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    void closeEvent(QCloseEvent* event);

    //// only works in debug environment
    //bool eventFilter(QObject *, QEvent *);

protected:
    void retranslateUiEx();

signals:
    void popupLoginDialogSignal();

public slots:
    void makeSelfTopMostSlot();
    void flashFileChangedSlot();

private:
    void _refreshBackground();

private:
    uiFullScreenDialog2* ui;

    QTimer* m_topMostTimer;

    enum DlgCloseAction
    {
        DlgCloseAction_NA = -1,
        DlgCloseAction_Close = 2,
    };
    // indicate whether the dialog close action 
    DlgCloseAction m_closeAction;
};

#endif // FULLSCREENDIALOGQT_H
