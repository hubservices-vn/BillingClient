#ifndef CC_HANDSHAKE_QTMODEL_H
#define CC_HANDSHAKE_QTMODEL_H

#include <QString>
#include "ICommandQtModel.h"


class CCHandshakeQtModel : public ICommandQtModel
{
public:
    CCHandshakeQtModel()
        : Timestamp_(0)
    {}

    CCHandshakeQtModel(const QString& ver, qint64 timestamp)
        :Version_(ver), Timestamp_(timestamp)
    {}

public:
    QString Version_;
    qint64  Timestamp_; // unix timestamp

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, CCHandshakeQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};



class CCHandshakeAckQtModel : public ICommandQtModel
{
public:
    CCHandshakeAckQtModel()
        : ClientId_(-1)
    {}

    CCHandshakeAckQtModel(int clientId)
        :ClientId_(clientId)
    {}

public:
    int ClientId_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, CCHandshakeAckQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};

#endif // CCHANDSHAKEQTMODEL_H
