#include "TextMessageManager.h"
#include "TextMessageDialog.h"
#include "TextMessageQtModel.h"
#include "cc2csSender.h"
#include "cc2csCmds.h"
#include "ConfigManager.h"
#include "SessionManager.h"
#include <QMessageBox>
#include "CommonSettings.h"

TextMessageManager::~TextMessageManager()
{
    if (m_msgDlg != NULL)
    {
        m_msgDlg->deleteLater();
        m_msgDlg = NULL;
    }
}

bool TextMessageManager::init()
{
    m_msgDlg = NULL;
    qRegisterMetaType<TextMessageQtModel>("TextMessageQtModel");
    return true;
}

bool TextMessageManager::connectSigSlot()
{
    // listen to session end event and reset self
    {
        SessionManager* mgr = SessionManager::GetInstance();
        connect(mgr, SIGNAL(sessionEnded()),
            this, SLOT(sessionEndedSlot()));
    }

    return true;
}

void TextMessageManager::procTextMsgRecved( const TextMessageQtModel &qtModel )
{
    if (qtModel.MsgContent_.length() > 0)
    {
        getDlg()->appendMsgRecved(qtModel.MsgContent_);
    }
    MessageOption isDisabled = qtModel.Disable_;
    if (isDisabled == MessageOption_Disabled)
    {
        getDlg()->setEnabled(false);
    } else if (isDisabled == MessageOption_Enabled){
        getDlg()->setEnabled(true);
    }
    getDlg()->updateAvailableStatus();
}

TextMessageDialog* TextMessageManager::getDlg()
{
    if (m_msgDlg == NULL)
    {
        m_msgDlg = new TextMessageDialog();
    }

    return m_msgDlg;
}

void TextMessageManager::sendMessage( const QString &msgContent )
{
    TextMessageQtModel qtModel(CConfigManager::clientId(), msgContent);
    cc2csSender::send(cc2cs::TextMessageCommand(qtModel));
}

void TextMessageManager::showDialog()
{
    getDlg()->show();
}

void TextMessageManager::sessionEndedSlot()
{
    if (m_msgDlg != NULL)
    {
        m_msgDlg->setEnabled(true);
        m_msgDlg->reset();
        m_msgDlg->done(0);
    }
}

