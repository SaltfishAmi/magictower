#include "Main.h"
#include <time.h>
// exercise for multi-threading
long globalTime=0;
DWORD WINAPI timer(LPVOID Param)  //ȫ�ּ�ʱ��
{
    while(1)
    {
        Sleep(1000);
        WaitForSingleObject(hTimerMutex, INFINITE);
        globalTime++;
        ReleaseMutex(hTimerMutex);
    }
}



DWORD WINAPI doAfter(LPVOID param)  //����ʱִ��ĳ��������Ϊֻ����һ���������Ը���struct
{
    int secs=((tmr_para*)param)->secs;
    WaitForSingleObject(hTimerMutex, INFINITE);
    int timeNow=globalTime;
    ReleaseMutex(hTimerMutex);
    while(1)
    {
        Sleep(1000);
        WaitForSingleObject(hTimerMutex, INFINITE);
        if(globalTime==timeNow+secs)break;
        ReleaseMutex(hTimerMutex);
    }
    ((tmr_para*)param)->pFunction(((tmr_para*)param)->pParameter);
    ReleaseMutex(hTimerMutex);
    return 1;
}
