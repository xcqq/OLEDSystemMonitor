#include <iostream>
#include <unistd.h>
#include "SysInfo.h"
#include "oled.h"


int main()
{
    SysInfo testinfo;
    cOLEDScreen oledScreen=cOLEDScreen();
    while(1)
    {
        testinfo.getCpuUsage();
        testinfo.getMemUsage();
        oledScreen.refreshScreen();
        sleep(1);
        oledScreen.clearScreen();
    }
    return 0;
}
