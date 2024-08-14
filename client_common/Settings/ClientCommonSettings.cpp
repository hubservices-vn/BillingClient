#include "ClientCommonSettings.h"


// company config
const QString G_COMPANY = "Garena" ;


// put client to system auto-start(Internal version and public version should share the same, otherwise 2 versions might launch at the same time)
const QString APP_AUTORUN_REGKEY_NAME = "GBClientApp";

const QString SVC_EXE_NAMEWITHEXT = "gbClientService.exe";
const QString APP_EXE_NAMEWITHEXT = "gbClientApp.exe";;
const QString LOCKER_EXE_NAMEWITHEXT = "gbClientLocker.exe";

const QString G_DISPLAY_SOFTWARE = QT_TRANSLATE_NOOP("ClientCommonSettings", "GCafe+ client");

// product root key: HKEY_LOCAL_MACHINE\\Software\\Garena\\{CLIENT_DATA_REGKEY_NAME}
const QString CLIENT_DATA_REGKEY_NAME = "GBillingClient";
const QString SVC_DATA_FOLDER_NAME = "GBClientSvc";
const QString APP_DATA_FOLDER_NAME = "GBClientApp";
const QString LOCKER_DATA_FOLDER_NAME = "GBClientLocker";


const QString DEFAULT_ADMIN_NAME = "AJSXxqIIpl09BDXdR38yTQ=="; // xtea of "GARENA" (notice the xtea result is not the same every time, so don't compare the encrypted result simply)
const QString DEFAULT_ADMIN_CODE = "b854ed213062921bd84d179a47a90dff34b9340c54922d350357a5fc3adaf1fe7ca21e5cde9cfc6315753eac4fbf6f40d420b65a4ae5f0df5451049705b5b2ca"; // utils:hashUserData() method


// prochook will check the existence of client service (21s interval, 3 time max), 
// and if no client service is running for 63 secones, it will reboot pc
const int Win_Service_Restart_Interval_MS = 30000; // 30000 ms
