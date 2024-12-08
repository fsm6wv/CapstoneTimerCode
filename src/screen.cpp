#include "screen.h"
#include "sensors.h"

TFT_eSPI tft = TFT_eSPI();
  char tdsString[30];
  char phString[30];
  char humidityString[30];
  char tempString[30];
  char waterLevelString[30];



void init_screen(){
  tft.begin();
  uint16_t calData[5] = { 310, 3455, 408, 3440, 4 };
  tft.setTouch(calData);
  tft.setRotation(0); // Adjust rotation if needed
  tft.fillScreen(TFT_BLUE);
  tft.setFreeFont();

  tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("Welcome to the plant", 0, 0, 4);
  tft.drawString("selection menu!", 0, 20, 4);
  refresh_screen();

  // touch_calibrate();
  // tft.fillScreen(TFT_BLACK);
  // tft.drawCentreString("Touch screen to test!",tft.width()/2, tft.height()/2, 2);
}

  void refresh_screen(){
  sprintf(tdsString, "TDS (ppm): %.2f%", getTdsValue());
  sprintf(phString, "pH: %.2f", getPhValue());
  sprintf(humidityString, "Humidity: %.2f", getDhtHumidityValue());
  sprintf(tempString, "Temp: %.2fC", getDhtTemperatureValue());
  tft.drawString(humidityString, 0, 60, 4);
  tft.drawString(tempString, 0, 100, 4);
  tft.drawString(phString, 0, 140, 4);
  tft.drawString(tdsString, 0, 180, 4);
  sprintf(waterLevelString, "WL: %.2f", getWaterlevelValue());
  tft.drawString(waterLevelString, 0, 220, 4);
  }


































void touch_calibrate(){
  uint16_t calData[5];
  uint8_t calDataOK = 0;
  // Calibrate
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Touch corners as indicated");
  tft.setTextFont(1);
  tft.println();
  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
  Serial.println(); Serial.println();
  Serial.println("// Use this calibration code in setup():");
  Serial.print("  uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++){
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  Serial.print("  tft.setTouch(calData);");
  Serial.println(); Serial.println();
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");
  delay(4000);
}

