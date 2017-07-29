#include "oled.h"
#include <unistd.h>
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
        usleep(arg_int*2000);
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

    default:
        return 0;
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
      /* disable chipselect */
      u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_disable_level);
      /* no wait required here */

      /* for SPI: setup correct level of the clock signal */
      //digitalWrite(13, u8x8_GetSPIClockPhase(u8g2_GetU8x8(u8g2)));
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC(u8g2, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        wiringPiSPISetup(0,500000);

      u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_enable_level);
      u8g2->gpio_and_delay_cb(u8g2, U8X8_MSG_DELAY_NANO, u8g2->display_info->post_chip_enable_wait_ns, NULL);
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      u8g2->gpio_and_delay_cb(u8g2, U8X8_MSG_DELAY_NANO, u8g2->display_info->pre_chip_disable_wait_ns, NULL);
      u8x8_gpio_SetCS(u8g2, u8g2->display_info->chip_disable_level);
      //SPI.end();
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
    u8x8_Setup_SH1106_128x64_NONAME(&mU8g2);
    u8x8_InitDisplay(&mU8g2);
    u8x8_SetPowerSave(&mU8g2, 0);
    u8x8_SetContrast(&mU8g2, 10);
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
    u8x8_SetFont(&mU8g2, u8x8_font_chroma48medium8_r);
    u8x8_DrawString(&mU8g2, 0, 0, "Hello World");
    u8x8_DrawString(&mU8g2, 3, 1, "ABCdefg");

}
void cOLEDScreen::clearScreen()
{
    u8x8_ClearDisplay(&mU8g2);
}


