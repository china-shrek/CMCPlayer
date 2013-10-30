#pragma once
#include "CMCTypeDefine.h"
#include "CMCThread.h"
#include <map>
#include <string>
using namespace std;

/*********************************ÄÚ´æ¹ÜÀí***************************************/
class CMCMemory
{
public:
    CMC_EXPORT CMCMemory();
    CMC_EXPORT ~CMCMemory();

    CMC_EXPORT CMC_VOID AddAddress(CMC_CHAR *pAddr,const CMC_CHAR *pVarName,const CMC_CHAR *pFuncName);
    CMC_EXPORT CMC_VOID DelAddress(CMC_CHAR *pAddr);

private:
    struct ADDR_INFO
    {
        string strVarName;
        string strFuncName;
    };

    map<void *,ADDR_INFO>   m_mapAddr;
    CMCMutexCS              m_entryCS;
};
