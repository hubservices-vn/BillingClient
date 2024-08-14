#ifndef ADMINOPTIONQTMODELC_H
#define ADMINOPTIONQTMODELC_H

#include <QString>
#include "ICommandQtModel.h"


// bitwise
enum AdminOperationType
{
    AdminOp_NA = 0x0000,
    AdminOp_ServerAddress = 0x0001,
    AdminOp_LocalAdminAcc = 0x0002,
};

class AdminOptionRequestQtModelC : public ICommandQtModel
{
public:
    AdminOptionRequestQtModelC() 
        : OpType_(0)
    {}

    AdminOptionRequestQtModelC(int typeVal)
        : OpType_(typeVal)
    {}

public:
    int OpType_; // bit-OR result of AdminOperationType
    QString ServerAddress_;
    QString NewAdminName_;
    QString OldAdminPwdRaw_;
    QString NewAdminPwdRaw_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, AdminOptionRequestQtModelC& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};



enum AdminOperationResult
{
    AdminOpResult_NA = -1,
    AdminOpResult_OK = 1,
    AdminOpResult_WrongNamePwd = 2,
};

class AdminOptionReplyQtModelC : public ICommandQtModel
{
public:
    AdminOptionReplyQtModelC() 
        : OpType_(0), Result_(AdminOpResult_NA)
    {}

    AdminOptionReplyQtModelC(int typeVal, AdminOperationResult result)
        : OpType_(typeVal), Result_(result)
    {}

public:
    int OpType_; // bit-OR result of AdminOperationType
    AdminOperationResult Result_;
    QString ServerAddress_;
    QString NewAdminName_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, AdminOptionReplyQtModelC& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};


#endif // ADMINOPTIONQTMODELC_H
