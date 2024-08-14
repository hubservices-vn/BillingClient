#ifndef TEXTMESSAGEMANAGER_H
#define TEXTMESSAGEMANAGER_H
#include <QObject>

#include "Common/Singleton_T.h"
using SDK::Common::Singleton_T;

class TextMessageQtModel;
class TextMessageManager : public QObject, public Singleton_T<TextMessageManager>
{
public:
    bool init();
    bool connectSigSlot();

public:
    void procTextMsgRecved(const TextMessageQtModel &qtModel);
};

#endif // TEXTMESSAGEMANAGER_H
