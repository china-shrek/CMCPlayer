#pragma once

/****************************************************编程规范***************************************
内部不出现任何基础数据类型，全部使用CMC_TYPE的数据类型




名字 = 前缀 + 中缀 + 名字

/////////////前缀
p:      指针
sp:     智能指针
m_:     类的成员变量
s_:     类中的静态成员变量
g_:     全局变量
I:      COM接口

/////////////中缀
f:      一个旗标    BOOL bool
dw:     DWORD
hr:     CMC_HRESULT
h:      一个句柄    CMC_HWND
ch:     一个字符    CMC_CHAR
sz:     以'\0'结尾的字符串

/////////////名字
类:     每个单词首字母大写(Pascal规范 class StringList)
枚举:   首字母大写(Pascal规范 enum Type)
函数:   名词或则动名词(Pascal规范 void Printf();void ControlList();)
接口:   带一个I前缀(Pascal规范 interface IControl{...})
结构体: 所有字母大写，必须加CMC前缀 以'_'分隔单词
宏:     所有字母大写，以'_'分隔单词
变量:   Camel规范 exampleText,dwCount
模版参数:必须带'T'前缀(Pascal规范 T TItem);


/////////////////////////////////其他////////////////////////////////////
const int *p        指向一个const int的指针
int *const p        指向int的const指针
const int *const p  指向const int 的const指针
sizeof              尽量使用sizeof(变量)，而不是sizeof(类型)
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
#define CMC_UNUSED(x) (void)(x)     //避免编译警告
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



