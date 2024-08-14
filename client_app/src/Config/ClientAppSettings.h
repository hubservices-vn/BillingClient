#ifndef CLIENT_APP_SETTINGS_H
#define CLIENT_APP_SETTINGS_H

#include <QString>
#include "Settings/ClientCommonSettings.h"

// bit flag, support bit OR AND operation
enum MessageDisplay
{
    MessageDisplay_TextBox = 0x0001,
    MessageDisplay_SystemTray = 0x0002,
    MessageDisplay_MsgBox  = 0x0004,
    MessageDisplay_PlaySound = 0x008,
};

#endif // CLIENT_APP_SETTINGS_H
