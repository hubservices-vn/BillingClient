#ifndef CLIENT_COMMON_SETTINGS_H
#define CLIENT_COMMON_SETTINGS_H
#include <QString>

// company config
extern const QString G_COMPANY;

// put client to system auto-start(Internal version and public version should share the same, otherwise 2 versions might launch at the same time)
extern const QString APP_AUTORUN_REGKEY_NAME;

extern const QString SVC_EXE_NAMEWITHEXT;
extern const QString APP_EXE_NAMEWITHEXT;
extern const QString LOCKER_EXE_NAMEWITHEXT;

extern const QString G_DISPLAY_SOFTWARE;
extern const QString CLIENT_DATA_REGKEY_NAME;

extern const QString SVC_DATA_FOLDER_NAME;
extern const QString APP_DATA_FOLDER_NAME;
extern const QString LOCKER_DATA_FOLDER_NAME;

// xtea of "GARENA" (notice the xtea result is not the same every time, so don't compare the encrypted result simply)
extern const QString DEFAULT_ADMIN_NAME;
// utils:hashUserData() method
extern const QString DEFAULT_ADMIN_CODE;

enum RecordChangeType
{
    RecordChangeType_NA = -1,
    RecordChangeType_Add = 1,
    RecordChangeType_Edit = 2,
    RecordChangeType_Del = 3,
};


// how long should client service restart in Windows Services.msc settings
extern const int Win_Service_Restart_Interval_MS;

#endif // CLIENT_COMMON_SETTINGS_H
