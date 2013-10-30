#include "CMCThread.h"
#include "CMCGlobalObject.h"
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"cmc.thread.lib")
#pragma comment(lib,"cmc.global.object.lib")

int main()
{
    int key;
    while(cin>>key)
    {
        switch(key)
        {
        case 0:
        case 1:
        default:
         printf("%s\n",GLOBAL_ERROR(key));
         break;

        }
    }

    return 0;
}