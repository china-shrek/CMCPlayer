#include "..\cmc.libcmc\include\CMCMemory.h"

CMCMemory::CMCMemory()
{

}

CMCMemory::~CMCMemory()
{

}

CMC_VOID CMCMemory::AddAddress(CMC_CHAR *pAddr,const CMC_CHAR *pVarName,const CMC_CHAR *pFuncName)
{
    CMCMutexLock(&m_entryCS,__FUNCTION__);
    if(pAddr)
    {
        ADDR_INFO info;
        info.strFuncName    = pFuncName;
        info.strVarName     = pVarName;
        m_mapAddr[pAddr]    = info;
    }
}

CMC_VOID CMCMemory::DelAddress(CMC_CHAR *pAddr)
{
    CMCMutexLock(&m_entryCS,__FUNCTION__);
    if(pAddr)
    {
        map<void *,ADDR_INFO>::iterator itor = m_mapAddr.find(pAddr);
        if(itor != m_mapAddr.end())
        {
            m_mapAddr.erase(itor);
        }
    }
}