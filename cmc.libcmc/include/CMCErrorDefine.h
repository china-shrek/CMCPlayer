#pragma once


enum
{
    CMC_NOERROR = 0,
    CMC_UNKNOW_ERROR,
    CMC_ERROR_ENTRY,                //错误的入口函数
    CMC_ERROR_PTR,                  //错误的指针
    CMC_ERROR_THREAD_CREATE,        //创建线程失败
    CMC_ERROR_TIMER_CREATE,         //创建计时器失败
};