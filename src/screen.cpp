#include "screen.h"
#include "sensors.h"

TFT_eSPI tft = TFT_eSPI();
void init_screen(){
  //XPT2046_Touchscreen touch(40); // T_CS pin
  tft.begin();
  delay(150); 
  tft.setRotation(0); // Adjust rotation if needed
  tft.fillScreen(TFT_RED);
  //tft.setFreeFont();
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  char fullString[20];
  sprintf(fullString, "Sample floar data: %.2f", getTdsValue());
  tft.drawString("Welcome to the plant", 0, 0, 4);
  tft.drawString("selection menu!", 0, 20, 4);
  tft.drawString(fullString, 0, 80, 4);


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



