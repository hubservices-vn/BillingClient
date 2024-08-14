#ifndef TEXTMESSAGEDIALOG_H
#define TEXTMESSAGEDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
    class TextMessageDialog;
}

const int MAX_TEXT_LENGTH = 200;
class TextMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextMessageDialog(QWidget *parent = 0);
    ~TextMessageDialog();

    bool connectSigSlot();

    void appendMsgSend(const QString& msg);

    void appendMsgRecved(const QString& msg);

    void reset();
    
    void updateAvailableStatus();

public slots:
    void on_sendButton_clicked();
    void on_inputTextEdit_textChanged();

protected:
    // overwrite some event
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

    void changeEvent(QEvent *e);
    //bool eventFilter(QObject *object, QEvent *event);

private:
    void _appendMsgImp(const QString &userName, const QString &content,
        const QColor& headerColor, const QColor& msgColor);

private:
    Ui::TextMessageDialog *ui;
    // if the text exceeds the max allowed length(say, 200 chars), we call "undo" to revert the action
    // in this method, the "textChanged" signal also raised
    // so use this flag to identify whether it's in user edit mode or in "undo mode"
    bool m_inUndo;
};

#endif // TEXTMESSAGEDIALOG_H
