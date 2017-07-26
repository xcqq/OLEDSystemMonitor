#include <iostream>
#include <unistd.h>
#include "SysInfo.h"

int main()
{
    SysInfo testinfo;
    while(1)
    {
        testinfo.getCpuUsage();
        testinfo.getMemUsage();
        sleep(1);
    }
    return 0;
}