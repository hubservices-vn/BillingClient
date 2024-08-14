#include "OSAgentWinQt.h"
#include <ConfigManager.h>
#include <QCoreApplication>
#include "log.h"
#include "ClientCommonSettings.h"


QString OSAgentWinQt::m_productKeyHive = "HKEY_LOCAL_MACHINE\\Software\\" + G_COMPANY + "\\" + CLIENT_DATA_REGKEY_NAME;

OSAgentWinQt::OSAgentWinQt()
: OSAgentWin()
{
}

OSAgentWinQt::~OSAgentWinQt()
{

}
