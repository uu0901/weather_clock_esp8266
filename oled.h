#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <U8g2lib.h>
extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
// extern U8G2_SSD1309_128X64_NONAME2_1_SW_I2C u8g2;
// extern U8G2_SH1106_128X64_NONAME_1_SW_I2C u8g2;
// extern U8G2_SH1106_128X64_VCOMH0_1_SW_I2C u8g2;
void OledInit();
void OledShow(uint8_t x, uint8_t y, String text);
void OledClear();
#endif