#ifndef NETWORKSTATEQTMODEL_H
#define NETWORKSTATEQTMODEL_H

#include <QString>
#include "ICommandQtModel.h"

class NetworkStateQtModel : public ICommandQtModel
{
public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, NetworkStateQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};


enum NetworkResult;
class NetworkStateReplyQtModel : public ICommandQtModel
{
public:
    NetworkStateReplyQtModel() 
        : ClientId(-1)
    {}

    NetworkStateReplyQtModel(int clientId, NetworkResult result)
        : ClientId(clientId), State(result)
    {}

public:
    int ClientId;
    NetworkResult State;

public:
    static void fromProtoBuf(ProtobufMsgPtr_s pbPtr, NetworkStateReplyQtModel& qtModel);

public: // implementation of interface
    std::string toStdString() const;
    ProtobufMsgPtr_s toProtoBuf();
};


#endif // NETWORKSTATEQTMODEL_H
