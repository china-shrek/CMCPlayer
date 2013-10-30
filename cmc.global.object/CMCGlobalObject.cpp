#include "..\cmc.libcmc\include\CMCGlobalObject.h"

CMCGlobalObject::CMCGlobalObject(void)
{
}


CMCGlobalObject::~CMCGlobalObject(void)
{
}

CMCGlobalObject &CMCGlobalObject::GetInstance()
{
    static CMCGlobalObject instance;
    return instance;
}