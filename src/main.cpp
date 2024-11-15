#include <Arduino.h>
#include "timer.h"
#include "sensors.h"

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
  Serial.println("Pins initialized");
  // Timer Init
  setup_timer(TIMER_0, 5); // Lights
  setup_timer(TIMER_1, 8); // Pump

}

void loop() {
  read_PH_sensor();
  read_DHT_sensor();
  read_WATERLEVEL_sensor();
  read_TDS_sensor();
  print_sensor_values();
  delay(1000);
}
