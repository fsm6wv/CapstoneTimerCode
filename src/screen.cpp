#include "screen.h"


void init_screen(){
  TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
  //XPT2046_Touchscreen touch(40); // T_CS pin
  tft.begin();
  tft.setRotation(0); // Adjust rotation if needed
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Hello, ESP32-S2!", 0, 0, 2);
//   if (touch.begin()) {
//     Serial.println("Touchscreen initialized");
//   }
}


