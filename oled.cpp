#include "oled.h"
#include <unistd.h>
#include <stdio.h>
using namespace std;

extern "C" uint8_t u8x8_gpio_and_delay(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch(msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:

        pinMode(21, OUTPUT);
        pinMode(22, OUTPUT);
        pinMode(23, OUTPUT);
        pinMode(24, OUTPUT);

        break;

    case U8X8_MSG_DELAY_MILLI:
        usleep(arg_int*1000);
        break;

    case U8X8_MSG_GPIO_DC:
        digitalWrite(24, arg_int);
        break;

    case U8X8_MSG_GPIO_CS:
        digitalWrite(23, arg_int);
        break;

    case U8X8_MSG_GPIO_RESET:
        digitalWrite(8, arg_int);
        break;

    case U8X8_MSG_GPIO_D0:
        digitalWrite(21, arg_int);
        break;
    case U8X8_MSG_GPIO_D1:
        digitalWrite(22, arg_int);
        break;
    case U8X8_MSG_DELAY_NANO:
        break;
    default:
        u8x8_SetGPIOResult(u8g2, 1);
        break;
    }
    return 1;
}

extern "C" uint8_t u8x8_byte_hw_spi(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    uint8_t *data;

    switch(msg)
    {
    case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        wiringPiSPIDataRW(0,data,arg_int);
        break;
    case U8X8_MSG_BYTE_INIT:
        u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_disable_level);
        break;
    case U8X8_MSG_BYTE_SET_DC:
        u8x8_gpio_SetDC(u8g2, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_enable_level);
        u8g2->gpio_and_delay_cb(u8g2, U8X8_MSG_DELAY_NANO, u8g2->display_info->post_chip_enable_wait_ns, NULL);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        u8g2->gpio_and_delay_cb(u8g2, U8X8_MSG_DELAY_NANO, u8g2->display_info->pre_chip_disable_wait_ns, NULL);
        u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_disable_level);
        break;
    default:
        return 0;
    }
    return 1;
}

extern "C" void u8x8_Setup_SH1106_128x64_NONAME(u8x8_t *u8x8)
{
    /* setup defaults */
    u8x8_SetupDefaults(u8x8);

    /* setup specific callbacks */
    u8x8->display_cb = u8x8_d_sh1106_128x64_noname;
    u8x8->cad_cb = u8x8_cad_001;
    //u8x8->byte_cb = u8x8_byte_4wire_sw_spi;
    u8x8->byte_cb = u8x8_byte_hw_spi;
    u8x8->gpio_and_delay_cb = u8x8_gpio_and_delay;

    /* setup display info */
    u8x8_SetupMemory(u8x8);
}

cOLEDScreen::cOLEDScreen()
{

    wiringPiSetup();
    wiringPiSPISetup(0,1000000);
    u8g2_Setup_sh1106_128x64_noname_f(&mU8g2,U8G2_R0,u8x8_byte_hw_spi,u8x8_gpio_and_delay);
    u8g2_InitDisplay(&mU8g2);
    u8g2_SetPowerSave(&mU8g2, 0);
    u8g2_ClearBuffer(&mU8g2);

}

cOLEDScreen::~cOLEDScreen()
{
    //nothing to do
}

void cOLEDScreen::setCPUUsage(float cpuUsage)
{
    mCPUUsage=cpuUsage;
}
void cOLEDScreen::setMemUsage(float memUsage)
{
    mMemUsage=memUsage;
}
void cOLEDScreen::setNetworkDownSpeed(float networkDownSpeed)
{
    mNetworkDownSpeed=networkDownSpeed;
}
void cOLEDScreen::setNetworkUpSpeed(float networkUpSpeed)
{
    mNetworkUpSpeed=networkUpSpeed;
}
void cOLEDScreen::refreshScreen()
{
    char buf[1024];
    u8g2_SetFontMode(&mU8g2,1);
    u8g2_SetFontDirection(&mU8g2,0);
    u8g2_SetFont(&mU8g2, u8g2_font_6x12_tf);
    u8g2_SetFontPosTop(&mU8g2);
    sprintf(buf,"CPU:%4.1f%%",mCPUUsage);
    u8g2_DrawStr(&mU8g2, 5, 5, buf);
    sprintf(buf,"Mem:%4.1f%%",mMemUsage);
    u8g2_DrawStr(&mU8g2, 65, 5, buf);
    u8g2_SendBuffer(&mU8g2);
}
void cOLEDScreen::clearScreen()
{
    u8g2_ClearDisplay(&mU8g2);
}
void cOLEDScreen::showLogo(unsigned char *logo)
{
    u8g2_ClearDisplay(&mU8g2);
    u8g2_ClearBuffer(&mU8g2);
    u8g2_DrawXBM(&mU8g2,0,0,128,64,logo);
    u8g2_SendBuffer(&mU8g2);
}


