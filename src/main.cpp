#include <Arduino.h>
#include "timer.h"
#include "sensors.h"
#include "screen.h"
#include "profiles.h"

float dhtHumidityBuffer [dhtHumidityBufferSize];
int dhtHumidityBufferIndex = 0;
float dhtTemperatureBuffer [dhtTemperatureBufferSize];
int dhtTemperatureBufferIndex = 0;
float tdsBuffer [tdsBufferSize];
int tdsBufferIndex = 0;
float phBuffer [phBufferSize];
int phBufferIndex = 0;
float waterlevelBuffer [waterlevelBufferSize];
int waterlevelBufferIndex = 0;
unsigned long startTime = 0;

void print_elapsed_time();


void setup() {
  disableCore0WDT();
  // Serial and sensor init
  setup_sensors();
  Serial.println();
  Serial.println("Can You Hear Me? (serial started)");
  fill_sensor_buffers();
  // Pin init
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LIGHTING_PIN, OUTPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT); 
  digitalWrite(BACKLIGHT_PIN, HIGH);

  Serial.println("Pins initialized");
  init_screen();
  // Timer Init
  // setup_timer(TIMER_0, 20); // Lights
  // setup_timer(TIMER_1, 20); // Pump
  Serial.println("System initialization completed!");
  startTime = millis();
}

// void loop() {
//   read_PH_sensor();
//   read_DHT_sensor();
//   read_WATERLEVEL_sensor();
//   read_TDS_sensor();
//   print_sensor_values();
//   delay(1000);
//   print_elapsed_time();
//   refresh_screen();
// }

void loop() { // touch test code 1
  uint16_t x = 0, y = 0; // To store the touch coordinates
  // Pressed will be set true is there is a valid touch on the screen
  bool pressed = tft.getTouch(&x, &y);

  // Draw a white spot at the detected coordinates
  if (pressed) {
    tft.fillCircle(x, y, 2, TFT_WHITE);
    //Serial.print("x,y = ");
    //Serial.print(x);
    //Serial.print(",");
    //Serial.println(y);
  }
}

// void loop() { // touch test code 2
//   uint16_t x, y;
//   tft.getTouchRaw(&x, &y);
//   Serial.printf("x: %i     ", x);
//   Serial.printf("y: %i     ", y);
//   Serial.printf("z: %i \n", tft.getTouchRawZ());
//   delay(250);
// }



void print_elapsed_time(){
  // Calculate elapsed growth time
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  
  // Convert elapsed time to hours, minutes, and seconds for display or use
  int hours = (elapsedTime / 3600000) % 24; // 3600000 ms in an hour
  int minutes = (elapsedTime / 60000) % 60;  // 60000 ms in a minute
  int seconds = (elapsedTime / 1000) % 60;   // 1000 ms in a second
  // Print elapsed time
  Serial.print("Elapsed Time: "); Serial.print(hours); Serial.print(" hours, ");
  Serial.print(minutes); Serial.print(" minutes, "); Serial.print(seconds); Serial.println(" seconds");
}
