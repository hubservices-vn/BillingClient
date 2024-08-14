#ifndef PENDINGSOCKETINFO_H
#define PENDINGSOCKETINFO_H

struct PendingSocketInfo
{
public:
   PendingSocketInfo()
       : FailCount(0), WaitingCount(0)
   {}
   PendingSocketInfo(int fc, int wc)
       : FailCount(fc), WaitingCount(wc)
   {}
   int FailCount;
   int WaitingCount;
};

#endif // PENDINGSOCKETINFO_H
