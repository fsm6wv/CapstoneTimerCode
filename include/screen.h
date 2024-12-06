#ifndef SCREEN_H
#define SCREEN_H
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#define BACKLIGHT_PIN 14

extern TFT_eSPI tft;


void init_screen();
void resetTFT();
void touch_calibrate();
void refresh_screen();





#endif