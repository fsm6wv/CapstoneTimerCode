#include <Arduino.h>
#include "timer.h"
#include "sensors.h"
#include "screen.h"

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
  pinMode(FAN_PIN, OUTPUT);
  Serial.println("Pins initialized");
  init_screen();
  // Timer Init
  setup_timer(TIMER_0, 20); // Lights
  setup_timer(TIMER_1, 20); // Pump
  startTime = millis();

}

void loop() {
  read_PH_sensor();
  read_DHT_sensor();
  read_WATERLEVEL_sensor();
  read_TDS_sensor();
  print_sensor_values();
  delay(1000);
  print_elapsed_time();
}



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
