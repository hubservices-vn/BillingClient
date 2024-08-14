#ifndef CL_HANDSHAKEQTMODEL_H
#define CL_HANDSHAKEQTMODEL_H

#include <QString>
#include "ICommandQtModel.h"

class CLHandshakeQtModel : public ICommandQtModel
{
public:
    CLHandshakeQtModel()
        : Timestamp_(0)
    {}

    CLHandshakeQtModel(const QString& ver, qint64 timestamp)
        :Version_(ver), Timestamp_(timestamp)
    {}

public:
    QString Version_;
    qint64 Timestamp_; // unix timestamp

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, CLHandshakeQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};




class CLHandshakeAckQtModel : public ICommandQtModel
{
public:
    CLHandshakeAckQtModel()
        : ClientId_(-1)
    {}

    CLHandshakeAckQtModel(int clientId)
        :ClientId_(clientId)
    {}

public:
    int ClientId_;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, CLHandshakeAckQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};

#endif // CLHANDSHAKEQTMODEL_H
