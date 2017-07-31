#include <iostream>
#include <unistd.h>
#include "SysInfo.h"
#include "oled.h"
#include "oledPic.h"

int main()
{
    SysInfo testinfo;
    cOLEDScreen oledScreen=cOLEDScreen();
    oledScreen.showLogo(picWelcome);
    sleep(3);
    while(1)
    {
        oledScreen.setCPUUsage(testinfo.getCpuUsage()*100);
        oledScreen.setMemUsage(testinfo.getMemUsage()*100);
        oledScreen.refreshScreen();
        sleep(1);
    }
    return 0;
}
