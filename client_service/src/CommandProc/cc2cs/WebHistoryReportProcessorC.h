#ifndef WEBHISTORYREPORTPROCESSORC_H
#define WEBHISTORYREPORTPROCESSORC_H

#include <QObject>
#include "CommandProcessorBase.h"
#include "pbc/WebHistoryPack.pb.h"

namespace cc2cs
{
    class WebHistoryReportProcessorC_p : public QObject
    {
        Q_OBJECT
    public:
        static void Handle( QTcpSocket* socket, ProtobufMsgPtr_s ptr );
    };

    typedef CommandProcessorBase<
        WebHistoryReportProcessorC_p,
        Models::WebHistoryPack,
        /*encrypt*/false
    >
    WebHistoryReportProcessorC;
}



#endif // WEBHISTORYREPORTPROCESSORC_H
