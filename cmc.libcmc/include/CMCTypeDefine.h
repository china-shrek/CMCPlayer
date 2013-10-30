#pragma once

/****************************************************��̹淶***************************************
�ڲ��������κλ����������ͣ�ȫ��ʹ��CMC_TYPE����������




���� = ǰ׺ + ��׺ + ����

/////////////ǰ׺
p:      ָ��
sp:     ����ָ��
m_:     ��ĳ�Ա����
s_:     ���еľ�̬��Ա����
g_:     ȫ�ֱ���
I:      COM�ӿ�

/////////////��׺
f:      һ�����    BOOL bool
dw:     DWORD
hr:     CMC_HRESULT
h:      һ�����    CMC_HWND
ch:     һ���ַ�    CMC_CHAR
sz:     ��'\0'��β���ַ���

/////////////����
��:     ÿ����������ĸ��д(Pascal�淶 class StringList)
ö��:   ����ĸ��д(Pascal�淶 enum Type)
����:   ���ʻ�������(Pascal�淶 void Printf();void ControlList();)
�ӿ�:   ��һ��Iǰ׺(Pascal�淶 interface IControl{...})
�ṹ��: ������ĸ��д�������CMCǰ׺ ��'_'�ָ�����
��:     ������ĸ��д����'_'�ָ�����
����:   Camel�淶 exampleText,dwCount
ģ�����:�����'T'ǰ׺(Pascal�淶 T TItem);


/////////////////////////////////����////////////////////////////////////
const int *p        ָ��һ��const int��ָ��
int *const p        ָ��int��constָ��
const int *const p  ָ��const int ��constָ��
sizeof              ����ʹ��sizeof(����)��������sizeof(����)
************************************************************************************************/



typedef unsigned int        CMC_UINT;
typedef int                 CMC_INT;
typedef char                CMC_CHAR;
typedef unsigned char       CMC_BYTE;
typedef unsigned short      CMC_WORD;
typedef unsigned long       CMC_DWORD;
typedef long                CMC_LONG;
typedef float               CMC_FLOAT;
typedef int                 CMC_BOOL;
typedef double              CMC_DOUBLE;
typedef __int64             CMC_INT64;
typedef unsigned __int64    CMC_UINT64;
typedef bool                CMC_bool;
typedef void                CMC_VOID;
typedef __w64 long          CMC_LONGPTR;
typedef long                CMC_HRESULT;

#ifdef _WIN32
#include <windows.h>
typedef HWND                CMC_HWND;
typedef RECT                CMC_RECT;
typedef POINT               CMC_POINT;
#endif

#define CMCAPI _stdcall
#define CMC_UNUSED(x) (void)(x)     //������뾯��
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif

#ifdef CMC_PLAYER_EXPORTS
#define CMC_EXPORT __declspec(dllexport)
#else
#define CMC_EXPORT __declspec(dllimport)
#endif


typedef CMC_UINT (CMCAPI *CMC_ENTRY)(CMC_VOID *);



