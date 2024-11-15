#include <Arduino.h>
#include "timer.h"
#include "sensors.h"

int dhtBuffer [10];
int dhtBufferIndex = 0;
int tdsBuffer [10];
int tdsBufferIndex = 0;
int phBuffer [10];
int phBufferIndex = 0;
int waterlevelBuffer [10];
int waterlevelBufferIndex = 0;

void setup() {
  disableCore0WDT();
  // Serial init
  Serial.begin(9600);
  Serial.println();
  Serial.println("Can You Hear Me? (serial started)");
  // Pin init
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LIGHTING_PIN, OUTPUT);
  Serial.println("Pins initialized");
  // Timer Init
  setup_timer(TIMER_0, 5); // Lights
  setup_timer(TIMER_1, 8); // Pump

}

void loop() {
  Serial.println("looping");
  // digitalWrite(LIGHTING_PIN, HIGH);
  // delay(1000);
  // digitalWrite(LIGHTING_PIN, LOW);
  delay(1000);
}
