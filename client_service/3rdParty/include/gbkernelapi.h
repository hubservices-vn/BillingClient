#pragma once

class IGBKernel
{
public:
    virtual bool Install()=0;
    virtual bool Uninstall()=0;
    virtual void HeartBeat(long long pid)=0;
};

IGBKernel* __stdcall GBKernel_Create();
void __stdcall GBKernel_Destory(IGBKernel* p);