#include "..\cmc.libcmc\include\CMCError.h"
#include "..\cmc.libcmc\include\CMCErrorDefine.h"
#include "..\cmc.libcmc\include\CMCTypeDefine.h"

typedef struct  
{
    CMC_INT nKey;
    const CMC_CHAR *pValue;
}CMC_ERROR_MSG;


CMC_ERROR_MSG ArryErr[] = 
{
    {CMC_NOERROR,"No Error"},
    {CMC_UNKNOW_ERROR,"Unknow Error"},
    /*******add here************************/
    {CMC_ERROR_ENTRY,"Error Entery Function"},
    {CMC_ERROR_PTR,"Error Pointer"},
    {CMC_ERROR_THREAD_CREATE,"Create Thread Failed"},
    {CMC_ERROR_TIMER_CREATE,"Create Timer Failed"},
    
    /***************************************/
    {0,nullptr},
};


const char* CMCError::GetError(int nCode)
{
    for(const CMC_ERROR_MSG *e = ArryErr;e->pValue != nullptr; e++)
    {
        if(nCode == e->nKey)
            return e->pValue;
    }

    return nullptr;
}