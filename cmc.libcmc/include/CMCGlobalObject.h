#pragma once

#pragma comment(lib,"cmc.error.lib")
#pragma comment(lib,"cmc.memory.lib")

#include "CMCError.h"
#include "CMCMemory.h"

#define GLOBAL_ERROR(val) CMCGlobalObject::GetInstance().m_error.GetError(val)
#define GLOBAL_ADD(val,name)  CMCGlobalObject::GetInstance().m_memory.AddAddress(val,name,__FUNCTION__)

class CMCGlobalObject
{
public:
    CMC_EXPORT static CMCGlobalObject &GetInstance();

protected:
    CMCGlobalObject(void);
    ~CMCGlobalObject(void);

public:
    CMCError    m_error;
    CMCMemory   m_memory;
};

