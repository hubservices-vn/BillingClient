#ifndef SCREEN_LOCK_QTMODEL_H
#define SCREEN_LOCK_QTMODEL_H


#include <QString>
#include "ICommandQtModel.h"

enum ScreenLockMode
{
    LockMode_NA = -1,
    LockMode_Lock = 1,
    LockMode_UnlockByTmpPwd = 2,
    LockMode_UnlockByAcc = 3,
};

class ScreenLockRequestQtModel : public ICommandQtModel
{
public:
    ScreenLockRequestQtModel() 
        : Mode_(LockMode_NA)
    {}

    // LOCK / UNLOCK by user set password
    ScreenLockRequestQtModel(ScreenLockMode mode, const QString &pwd)
        : Mode_(mode), Password_(pwd)
    {}

    // unlock by user account
    ScreenLockRequestQtModel(ScreenLockMode mode, const QString &accName, const QString &accPwdRaw)
        : Mode_(mode), AccountName_(accName), AccountPwdRaw_(accPwdRaw)
    {}

public:
    ScreenLockMode Mode_;
    QString Password_;
    QString AccountName_;
    QString AccountPwdRaw_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, ScreenLockRequestQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};

class ScreenLockReplyQtModel : public ICommandQtModel
{
public:
    ScreenLockReplyQtModel() 
        : Mode_(LockMode_NA), Result_(false)
    {}

    ScreenLockReplyQtModel(ScreenLockMode mode, bool result)
        : Mode_(mode), Result_(result)
    {}

public:
    ScreenLockMode Mode_;
    bool Result_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, ScreenLockReplyQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};





class ScreenUnlockRequestQtModel : public ICommandQtModel
{
public:
    ScreenUnlockRequestQtModel() 
        : Mode_(LockMode_NA)
    {}

    // LOCK / UNLOCK by user set password
    ScreenUnlockRequestQtModel(ScreenLockMode mode, const QString &pwd)
        : Mode_(mode), Password_(pwd)
    {}

    // unlock by user account
    ScreenUnlockRequestQtModel(ScreenLockMode mode, const QString &accName, const QString &accPwdRaw)
        : Mode_(mode), AccountName_(accName), AccountPwdRaw_(accPwdRaw)
    {}

public:
    ScreenLockMode Mode_;
    QString Password_;
    QString AccountName_;
    QString AccountPwdRaw_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, ScreenUnlockRequestQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};

class ScreenUnlockReplyQtModel : public ICommandQtModel
{
public:
    ScreenUnlockReplyQtModel() 
        : Mode_(LockMode_NA), Result_(false)
    {}

    ScreenUnlockReplyQtModel(ScreenLockMode mode, bool result)
        : Mode_(mode), Result_(result)
    {}

public:
    ScreenLockMode Mode_;
    bool Result_;

public:
    static void fromProtoBuf(
        ProtobufMsgPtr_s pbPtr, 
        ScreenUnlockReplyQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};


#endif // SCREEN_LOCK_QTMODEL_H
