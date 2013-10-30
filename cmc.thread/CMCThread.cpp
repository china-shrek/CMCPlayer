#include "..\cmc.libcmc\include\CMCThread.h"
#include "..\cmc.libcmc\include\CMCErrorDefine.h"

#ifdef _WIN32
#include <Windows.h>
#include <process.h>
#include <errno.h>
#include <assert.h>

class CMCWaitObj
{
public:
    static DWORD WaitForSingleObject (HANDLE handle, DWORD delay)
    {
        return WaitForMultipleObjects (1, &handle, delay);
    }
    static DWORD WaitForMultipleObjects (DWORD count,const HANDLE *handles,DWORD delay)
    {
        DWORD ret;
        if (count == 0)
        {
            ret = SleepEx (delay, TRUE);
            if (ret == 0)
                ret = WAIT_TIMEOUT;
        }
        else
            ret = WaitForMultipleObjectsEx (count, handles, FALSE, delay, TRUE);

        /* We do not abandon objects... this would be a bug */
        assert (ret < WAIT_ABANDONED_0 || WAIT_ABANDONED_0 + count - 1 < ret);

        if (ret == WAIT_FAILED)
            abort (); /* We are screwed! */
        return ret;
    }
    static DWORD Sleep (DWORD delay)
    {
        DWORD ret = WaitForMultipleObjects (0, NULL, delay);
        return (ret != WAIT_TIMEOUT) ? ret : 0;
    }
};


CMCThread::CMCThread(void)
{
    m_hThread = nullptr;
}


CMCThread::~CMCThread(void)
{
    CloseHandle(m_hThread);
}

CMC_HRESULT CMCThread::Create(CMC_THREAD &parm)
{
    if(!m_parm.pEntry)
        return CMC_ERROR_ENTRY;

    m_hThread = (CMC_VOID*)_beginthreadex(NULL,0,parm.pEntry,parm.pData,CREATE_SUSPENDED,NULL);
    if(m_hThread == nullptr)
        return CMC_ERROR_THREAD_CREATE;

    if(parm.nPriority)
        SetThreadPriority (m_hThread,parm.nPriority);	

    Resume();
    return CMC_NOERROR;
}

CMC_HRESULT CMCThread::Suspend()
{
    CMC_DWORD dwRet = 0;
    if(m_hThread)
        dwRet = SuspendThread(m_hThread);
    return CMC_NOERROR;
}

CMC_HRESULT CMCThread::Resume()
{
    CMC_DWORD dwRet = 0;
    if(m_hThread)
        dwRet = ResumeThread (m_hThread);
    return CMC_NOERROR;
}

void CMCThread::Release()
{
    if(m_hThread)
    {
        _endthreadex((uintptr_t)m_hThread);
        CloseHandle(m_hThread);
        m_hThread = 0;
    }
}

/***************************************临界区******************************************/
CMCMutexCS::CMCMutexCS()
{
    m_pMutex = new CRITICAL_SECTION;
    if(m_pMutex)
        InitializeCriticalSection((LPCRITICAL_SECTION)m_pMutex);

    memset(m_funcName,0,sizeof(m_funcName));
}

CMCMutexCS::~CMCMutexCS()
{
    if(m_pMutex)
    {
        DeleteCriticalSection((LPCRITICAL_SECTION)m_pMutex);
        delete (LPCRITICAL_SECTION)m_pMutex;
    }
}

CMC_VOID CMCMutexCS::EnterCS(const CMC_CHAR *pFuncName)
{
    if(m_pMutex)
        EnterCriticalSection((LPCRITICAL_SECTION)m_pMutex);

    memset(m_funcName,0,sizeof(m_funcName));
    memcpy(m_funcName,pFuncName,sizeof(m_funcName));
}

CMC_HRESULT CMCMutexCS::TryEnterCS()
{
    if(m_pMutex)
        return TryEnterCriticalSection((LPCRITICAL_SECTION)m_pMutex)?0 : EBUSY;
    return CMC_ERROR_PTR;
}

CMC_VOID CMCMutexCS::LeaveCS()
{
    if(m_pMutex)
        LeaveCriticalSection((LPCRITICAL_SECTION)m_pMutex);
}


CMCMutexLock::CMCMutexLock(CMCMutexCS *pLock,const CMC_CHAR *pFuncName)
{
    if(pLock)
    {
        m_pLock = pLock;
        m_pLock->EnterCS(pFuncName);
    }
}

CMCMutexLock::~CMCMutexLock()
{
    if(m_pLock)
    {
        m_pLock->LeaveCS();
    }
}

/***************************************条件变量******************************************/
CMC_EXPORT CMCCond::CMCCond()
{
    m_handle = CreateEvent (NULL, TRUE, FALSE, NULL);
    if(m_handle)
        abort();
}

CMC_EXPORT CMCCond::~CMCCond()
{
    if(m_handle)
        CloseHandle(m_handle);
}

CMC_EXPORT CMC_VOID CMCCond::Signal()
{
    this->Broadcast();
}

CMC_EXPORT CMC_VOID CMCCond::Broadcast()
{
    if(m_handle)
        SetEvent(m_handle);
}

CMC_EXPORT CMC_VOID CMCCond::Wait()
{
    DWORD result;
    do
    {
        m_mutex.LeaveCS();
        result = CMCWaitObj::WaitForSingleObject (m_handle, INFINITE);
        m_mutex.EnterCS(__FUNCTION__);
    }
    while (result == WAIT_IO_COMPLETION);

    ResetEvent(m_handle);
}

CMC_EXPORT CMC_HRESULT CMCCond::WaitTime( CMC_INT64 deadline )
{
    DWORD result;
    do
    {
        CMC_INT64 total = CMCMisc::MediaDate();

        total = (deadline - total) / 1000;
        if( total < 0 )
            total = 0;

        DWORD delay = (DWORD)((total > 0x7fffffff) ? 0x7fffffff : total);

        m_mutex.LeaveCS();
        result = CMCWaitObj::WaitForSingleObject (m_handle, delay);
        m_mutex.EnterCS(__FUNCTION__);
    }
    while (result == WAIT_IO_COMPLETION);

    ResetEvent (m_handle);

    return (result == WAIT_OBJECT_0) ? 0 : ETIMEDOUT;
}


CMC_INT64 CMCMisc::MediaDateGiveup (void)
{
    return 0;			
}

CMC_INT64 CMCMisc::MediaDateInterrupt (void)
{
    return 0;
}

CMC_INT64 CMCMisc::MediaDatePerf (void)
{
    LARGE_INTEGER counter, freq;
    if(!QueryPerformanceCounter (&counter) || !QueryPerformanceFrequency (&freq))
        abort();

    /* Convert to from (1/freq) to microsecond resolution */
    /* We need to split the division to avoid 63-bits overflow */
    lldiv_t d = lldiv (counter.QuadPart, freq.QuadPart);

    return (d.quot * 1000000) + ((d.rem * 1000000) / freq.QuadPart);
}

CMC_EXPORT CMC_INT64 CMCMisc::MediaDate(int type)
{
    switch(type)
    {
    case 0:
        return CMCMisc::MediaDatePerf();
    }

    return CMCMisc::MediaDateGiveup();
}

CMC_EXPORT  CMC_VOID CMCMisc::MediaWait( CMC_INT64 deadline )
{
    CMC_INT64 delay;

    while ((delay = (deadline - MediaDate())) > 0)
    {
        delay /= 1000;
        if (delay > 0x7fffffff)
            delay = 0x7fffffff;
        CMCWaitObj::Sleep((DWORD)delay);
    }
}

CMC_EXPORT CMC_VOID CMCMisc::MediaSleep( CMC_INT64 delay )
{
    MediaWait(MediaDate() + delay);
}

CMC_EXPORT CMC_UINT CMCMisc::GetCPUCount()
{
    SYSTEM_INFO systemInfo;

    GetNativeSystemInfo(&systemInfo);

    return systemInfo.dwNumberOfProcessors;
}

/***************************************信号量******************************************/
CMC_EXPORT CMCSem::CMCSem(CMC_UINT min,CMC_UINT max,CMC_CHAR *name)
{
    m_handle = CreateSemaphore (NULL, min, max, NULL);
    if (m_handle == NULL)
        abort ();
}

CMC_EXPORT CMCSem::~CMCSem()
{
    if(m_handle)
        CloseHandle(m_handle);
}

CMC_EXPORT CMC_VOID CMCSem::Post(int number)
{
    if(m_handle)
        ReleaseSemaphore(m_handle,number,NULL);
}

CMC_EXPORT CMC_VOID CMCSem::Wait(int numbers)
{
    DWORD result;

    while(numbers-- > 0)
    {
        do
        {
            result = CMCWaitObj::WaitForSingleObject (m_handle, INFINITE);
        }
        while (result == WAIT_IO_COMPLETION);
    }
}

/***************************************计时器******************************************/

CMC_EXPORT CMCTimer::CMCTimer()
{
    m_handle = INVALID_HANDLE_VALUE;
}

CMC_EXPORT CMCTimer::~CMCTimer()
{
    Stop();
}

CMC_EXPORT CMC_HRESULT CMCTimer::Run( CMC_TIMER &parm )
{
    if((HANDLE)m_handle == INVALID_HANDLE_VALUE)
    {
        m_parm = parm;
        CreateTimerQueueTimer(&m_handle, 
                              NULL,
                              TimerProc,
                              this,
                              parm.nDueTime, 
                              parm.nInterval, 
                              WT_EXECUTEDEFAULT);
        if((HANDLE)m_handle == INVALID_HANDLE_VALUE)
            return CMC_ERROR_TIMER_CREATE;
    }

    return CMC_NOERROR;
}

CMC_EXPORT CMC_HRESULT CMCTimer::Stop()
{
    if(m_handle != INVALID_HANDLE_VALUE)
    {
        DeleteTimerQueueTimer (NULL, m_handle, INVALID_HANDLE_VALUE);
        m_handle = INVALID_HANDLE_VALUE;
    }

    return CMC_NOERROR;
}

void CALLBACK CMCTimer::TimerProc( void *val, unsigned char timeout )
{
    CMCTimer *pThis = (CMCTimer*)val;
    assert(timeout);
    if(pThis)
    {
        if(pThis->m_parm.pEntry)
            pThis->m_parm.pEntry(pThis->m_parm.pData);
    }

}
#endif //end Win32