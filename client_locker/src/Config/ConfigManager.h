#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <QString>
#include <QLocale>
#include <QSettings>

#include "stdtypes.h"

class CConfigManager
{
public:
    static bool init();

    static bool loadRegData();

public: // debugging only
    static bool isDisableSecurityRestrictions();
    static bool isNoFullScreen();
    static bool isNoFlash();

    // different environments
    static bool isDeveloperEnv();

public: // accessors (peseudo)
    static int svrFlashVersion();
    static bool setSvrFlashVersion(int val);

    static QString svrFlashHash();
    static bool setSvrFlashHash(const QString& val);

public: // helpers (read directly from registry)
    static int clientId();
    static bool inTempLockMode();
    static QString adminNameEnc();
    static QString adminNameRaw();
    static QString adminCodeHash ();

private:
    // registry operation
    // read-only
    static QVariant _getRegData(const QSettings& setting, const QString& key);
    // if registry doesn't have key, then create the key and set default value
    static QVariant _getRegData(QSettings& setting, const QString& key, const QVariant& defaultValue);

private:
    static QString m_productKeyHive;
    static int m_clientId;

    // cache the flash content version and hash
    static int m_svrFlashVersion;
    static QString m_svrFlashHash;
};


#endif // CONFIGMANAGER_H
