#include "TextMessageDialog.h"
#include "ui_TextMessageDialog.h"
#include <QTime>
#include "Utils.h"
#include "UserAccountManager.h"
#include "DateTimeUtils.h"
#include "TextMessageManager.h"

TextMessageDialog::TextMessageDialog(QWidget *parent) 
: QDialog(parent), ui(new Ui::TextMessageDialog)
{
    ui->setupUi(this);

    ui->historyTextEdit->clear();
    ui->inputTextEdit->clear();
    //ui->inputTextEdit->installEventFilter(this);
    ui->inputTextEdit->setMaxTextLength(MAX_TEXT_LENGTH);
    this->setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    connectSigSlot();
}

TextMessageDialog::~TextMessageDialog()
{
    delete ui;
}

bool TextMessageDialog::connectSigSlot()
{
    //TextMessageNotifier* msgNoti = TextMessageNotifier::GetInstance();
    //connect(msgNoti, SIGNAL(cmdRecved(QString)),
    //    this, SLOT(msgReceivedSlot(QString)));
    return true;
}

void TextMessageDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TextMessageDialog::showEvent( QShowEvent *event )
{
    ui->inputTextEdit->setFocus();
    QDialog::showEvent(event);
}

void TextMessageDialog::on_sendButton_clicked()
{
    QString content = ui->inputTextEdit->toPlainText();
    int size = content.size();
    if (content.isEmpty())
    {
        return;
    }
    QChar ch = content[size-1];
    if(ch == '\n' || ch == '\r\n')
    {
        content = content.left(size-1);
    }
    appendMsgSend(content);
    TextMessageManager::GetInstance()->sendMessage(content);

    ui->inputTextEdit->clear();
	ui->msgLabel->setText(tr("%1/%2 characters").arg(0).arg(MAX_TEXT_LENGTH));
    ui->msgLabel->setStyleSheet("color:black;");
}

void TextMessageDialog::on_inputTextEdit_textChanged()
{
    if (m_inUndo == true)
    {
        return;
    }

    QString content = ui->inputTextEdit->toPlainText();
    int count = content.count();

    // only allow to send message when there is content
    ui->sendButton->setEnabled(content.trimmed().count() != 0);

    // only allow 200 chars at maximum
    if (count >= MAX_TEXT_LENGTH)
    {
        m_inUndo = true;
        {
            ui->msgLabel->setText(tr("maximum %1 characters allowed").arg(MAX_TEXT_LENGTH));
            ui->msgLabel->setStyleSheet("color:red;");

            if (count > MAX_TEXT_LENGTH)
            {
                QChar ch = content[MAX_TEXT_LENGTH];
                if(ch != '\n')
                {
                    // undo has issue. suppose we have 200 chars now, do the following action:
                    // press "backspace" to delete a key (199 chars now)
                    // key in that just-deleted-char again (200 chars now);
                    // key in a random key, you'll find that the last key was deleted mistakenly
                    // ui->inputTextEdit->undo();

                    QTextCursor cursor = ui->inputTextEdit->textCursor();
                    ui->inputTextEdit->setPlainText(content.left(MAX_TEXT_LENGTH));
                    ui->inputTextEdit->setTextCursor(cursor);
                }
                else
                {
                    on_sendButton_clicked();
                }
            }

        }
        m_inUndo = false;
    }
    else
    {
        ui->msgLabel->setText(tr("%1/%2 characters").arg(count).arg(MAX_TEXT_LENGTH));
        ui->msgLabel->setStyleSheet("color:black;");
    }
}

void TextMessageDialog::appendMsgSend( const QString& msg )
{
    QString userName = UserAccountManager::currentUserName();
    _appendMsgImp(userName, msg, QColor("#2288aa"), QColor(Qt::black));
}

void TextMessageDialog::appendMsgRecved(const QString& msg)
{
    QString userName = "ADMIN";
    _appendMsgImp(userName, msg, QColor("#11aa00"), QColor(Qt::black));
    ::FlashWindow((HWND)(this->effectiveWinId()), TRUE);
}

void TextMessageDialog::_appendMsgImp( const QString &userName, const QString &content,
                                      const QColor& headerColor, const QColor& msgColor)
{
    if (content.isEmpty())
    {
        return;
    }

    QString headerStr = tr("%1(%2): ")
        .arg(userName)
        .arg(DateTimeUtils::timeToString(QTime::currentTime()));;
    ui->historyTextEdit->setTextColor(headerColor);
    ui->historyTextEdit->append(headerStr);

    ui->historyTextEdit->setTextColor(msgColor);
    ui->historyTextEdit->append(content);

    if (!this->isVisible())
    {
        this->show();
    }
}

void TextMessageDialog::closeEvent( QCloseEvent *event )
{
    ui->historyTextEdit->clear();
    ui->inputTextEdit->clear();
    this->hide();
    event->ignore();
}

void TextMessageDialog::reset()
{
    ui->historyTextEdit->clear();
    ui->inputTextEdit->clear();
}

void TextMessageDialog::updateAvailableStatus()
{
    if(this->isEnabled() == false)
    {
        ui->msgLabel->setText(tr("Chatting has been disabled"));
        ui->msgLabel->setStyleSheet("color:red;");
    }
    else
    {
       on_inputTextEdit_textChanged();
    }
}
//
//bool TextMessageDialog::eventFilter( QObject *object, QEvent *event )
//{
//    if (object == ui->inputTextEdit)
//    {
//        if (event->type() == QEvent::KeyPress)
//        {
//            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(eveyt);
//
//            int key = keyEvent->key();
//            if (key == Qt::Key_Up ||
//                key == Qt::Key_Down ||
//                key == Qt::Key_Left ||
//                key == Qt::Key_Right ||
//                key == Qt::Key_PageUp ||
//                key == Qt::Key_PageDown ||
//                key == Qt::Key_Home ||
//                key == Qt::Key_End ||
//                key == Qt::Key_Delete ||
//                key == Qt::Key_Backspace)
//            {
//                return QObject::eventFilter(object, event);
//            }
//
//            // TODO: 
//
//            return true;
//        }
//    }
//
//    return QObject::eventFilter(object, event);
//}
