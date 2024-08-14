#ifndef PCACTIONPROCESSOR_H
#define PCACTIONPROCESSOR_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/PCActionPack.pb.h"

namespace s2c
{
    class PcActionRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        PcActionRequestProcessor_p, 
        Models::PCActionRequestPack,
        CMD_ENCRYPT
    > 
    PcActionRequestProcessor;
}


namespace s2c
{
    class VolumeQueryRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        VolumeQueryRequestProcessor_p, 
        Models::VolumeQueryRequestPack,
        CMD_ENCRYPT
    > 
    VolumeQueryRequestProcessor;
}



namespace s2c
{
    class VolumeControlRequestProcessor_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle(QTcpSocket* socket, ProtobufMsgPtr_s ptr);
    };

    typedef CommandProcessorBase<
        VolumeControlRequestProcessor_p, 
        Models::VolumeControlRequestPack,
        CMD_ENCRYPT
    > 
    VolumeControlRequestProcessor;
}


#endif // PCACTIONPROCESSOR_H
