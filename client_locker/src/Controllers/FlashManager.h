#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H


#include "CcCore/CcSingleton.h"
#include <QObject>
#include <QFileSystemWatcher>
#include <QTimer>

enum FlashType
{
    FlashType_NA = -1,
    FlashType_Local = 1,
    FlashType_Embed = 2,
    //FlashType_Server = 3,
};
class FlashTransferReplyQtModel;
class FlashManager : public QObject, public Singleton_T<FlashManager>
{
    Q_OBJECT
public:
    ~FlashManager();

    bool init();
    bool connectSigSlot();

public:
    QString localFlashFilePath() const;
    QString localBakFilePath() const;

    void restoreDefaultFlashFile();

public:
    void procFlashReplyC(const FlashTransferReplyQtModel& qtModel);

signals:
    // tell UI to refresh self
    void flashFileChanged();

public slots:
    void requestFlashSlot();

    void verifyFlashSlot();

private: // helper
    QString _getFlashDirPath() const;

private:
    // verify the local flash file
    void _verifyLocalFlashImp();

    // calc the embed flash file hash
    void _getFlashHashAsync(/*val of FlashType*/int typeVal);
    void _getFlashHashImp(/*val of FlashType*/int typeVal);

    void _watchFlashFile();

    void _saveNewFlashFile(const QByteArray& flashContent);

private:
    QString m_localFlashFilePath;
    QString m_localFlashHash;

    QString m_embedFlashHash;

    QTimer* m_updateFlashTimer;
    QTimer* m_verifyFlashTimer;

    // if this is true, then m_verifyFlashTimer won't 
    // do anything
    bool m_isSelfChange; 
};

#endif // FLASHMANAGER_H
