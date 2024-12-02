#include "screen.h"
#include "sensors.h"


void init_screen(){
  //XPT2046_Touchscreen touch(40); // T_CS pin
  TFT_eSPI tft = TFT_eSPI();
  tft.begin();
  resetTFT();
  tft.setRotation(0); // Adjust rotation if needed
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  char fullString[20];
  sprintf(fullString, "Hello %.2f", 1.1); // getPhValue()
  tft.drawString("whyyyy", 0, 0, 2);
//   if (touch.begin()) {
//     Serial.println("Touchscreen initialized");
//   }
}

void resetTFT() {
  digitalWrite(TFT_RST, LOW);   // Reset signal
  delay(10);                    // Wait for a short period
  digitalWrite(TFT_RST, HIGH);  // End reset
  delay(10);                    // Wait for the display to stabilize
}



