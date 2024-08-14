#ifndef TEXTMESSAGEMANAGER_H
#define TEXTMESSAGEMANAGER_H

#include <QObject>
#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class TextMessageDialog;
class TextMessageQtModel;
class TextMessageManager : public QObject, public Singleton_T<TextMessageManager>
{
    Q_OBJECT
public:
    ~TextMessageManager();

    bool init();
    bool connectSigSlot();

    TextMessageDialog* getDlg();

    void showDialog();
    void sendMessage(const QString &msgContent);

public slots: // cmd handler
    void procTextMsgRecved(const TextMessageQtModel &qtModel);
    void sessionEndedSlot();

private:
    TextMessageDialog* m_msgDlg;
};

#endif // TEXTMESSAGEMANAGER_H
