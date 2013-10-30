#pragma once
#include "CMCTypeDefine.h"

class CMCMisc;
class CMCThread;
class CMCMutexCS;
class CMCMutexLock;
class CMCCond;
class CMCSem;
class CMCTimer;

/***********************************************Struct define********************************/
struct CMC_THREAD 
{
    CMC_ENTRY   pEntry;
    CMC_VOID    *pData;
    CMC_INT     nPriority;      //eg:THREAD_PRIORITY_NORMAL 
    CMC_THREAD()
    {
        pEntry      = nullptr;
        pData       = nullptr;
        nPriority   = 0;
    }
};

struct CMC_TIMER 
{
    CMC_ENTRY   pEntry;
    CMC_VOID    *pData;
    CMC_INT     nInterval;      //时间间隔
    CMC_INT     nDueTime;       //等待多少时间后才开始运行计时器
    CMC_TIMER()
    {
        pEntry      = nullptr;
        pData       = nullptr;
        nInterval   = 1000;     //1s
        nDueTime    = 0;
    }
};

/**********************************************class define***********************************/
class CMCMisc
{
public:
    
    CMC_EXPORT static CMC_INT64 MediaDate(int type = 0);
    CMC_EXPORT static CMC_VOID MediaWait(CMC_INT64 deadline);
    CMC_EXPORT static CMC_VOID MediaSleep(CMC_INT64 delay);
    CMC_EXPORT static CMC_UINT GetCPUCount();

private:
    static CMC_INT64 MediaDateGiveup(void);
    static CMC_INT64 MediaDateInterrupt(void);
    static CMC_INT64 MediaDatePerf (void);
};

class CMCThread
{
public:
    CMC_EXPORT CMCThread(void);
    CMC_EXPORT ~CMCThread(void);

    CMC_EXPORT CMC_HRESULT Create(CMC_THREAD &parm);
    CMC_EXPORT CMC_HRESULT Suspend();
    CMC_EXPORT CMC_HRESULT Resume();
    CMC_EXPORT void Release();			    //强行退出,不提倡使用

private:
    CMC_THREAD m_parm;
    CMC_VOID *m_hThread;
};

class CMCMutexCS
{
public:
    CMC_EXPORT CMCMutexCS();
    CMC_EXPORT ~CMCMutexCS();

    CMC_EXPORT CMC_VOID EnterCS(const CMC_CHAR *pFuncName);
    CMC_EXPORT CMC_HRESULT TryEnterCS();
    CMC_EXPORT CMC_VOID LeaveCS();

private:
    CMC_VOID    *m_pMutex;
    CMC_CHAR    m_funcName[256];
};

class CMCMutexLock
{
public:
    CMC_EXPORT CMCMutexLock(CMCMutexCS *pLock,const CMC_CHAR *pFuncName);
    CMC_EXPORT ~CMCMutexLock();

private:
    CMCMutexCS *m_pLock;
};

class CMCCond
{
public: 
    CMC_EXPORT CMCCond();
    CMC_EXPORT ~CMCCond();

    CMC_EXPORT CMC_VOID Signal();
    CMC_EXPORT CMC_VOID Broadcast();
    CMC_EXPORT CMC_VOID Wait();
    CMC_EXPORT CMC_HRESULT WaitTime(CMC_INT64 deadline);

private:
     CMC_VOID   *m_handle;
     CMCMutexCS m_mutex;
};

class CMCSem
{
public: 
    CMC_EXPORT CMCSem(CMC_UINT min=0,CMC_UINT max=0x7fffffff,CMC_CHAR *name=NULL);
    CMC_EXPORT ~CMCSem();

    CMC_EXPORT CMC_VOID Post(int numbers=1);
    CMC_EXPORT CMC_VOID Wait(int numbers=1);
    
private:
    CMC_VOID   *m_handle;
};

class CMCTimer
{
public:
    CMC_EXPORT CMCTimer();
    CMC_EXPORT ~CMCTimer();
    CMC_EXPORT CMC_HRESULT Run(CMC_TIMER &parm);
    CMC_EXPORT CMC_HRESULT Stop();

private:
    static void CALLBACK TimerProc (void *val, unsigned char timeout);

private:
    CMC_TIMER   m_parm;
    CMC_VOID    *m_handle;
};