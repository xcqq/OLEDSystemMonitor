//
// Created by xczhang on 17-7-12.
//

#ifndef OLED_H
#define OLED_H

#include "u8x8.h"
#include "wiringPi.h"
#include "wiringPiSPI.h"

using namespace std;

class cOLEDScreen {

public:
    cOLEDScreen();
    ~cOLEDScreen();

    void setCPUUsage(float cpuUsage);
    void setMemUsage(float memUsage);
    void setNetworkUpSpeed(float networkUpSpeed);
    void setNetworkDownSpeed(float networkDownSpeed);
    void refreshScreen();
    void clearScreen();


private:
    float mCPUUsage = 0;
    float mMemUsage = 0;
    float mNetworkUpSpeed = 0;
    float mNetworkDownSpeed = 0;
    u8x8_t mU8g2;

};


#endif //OLED_H
