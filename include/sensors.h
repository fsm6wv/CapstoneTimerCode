#ifndef SENSORS_H
#define SENSORS_H

#include "DHT.h"
#include <ESP32AnalogRead.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11  // Sensor type
#define TDS_PIN 3
#define WATERLEVEL_PIN 4
#define PH_PIN 5

void setup_sensors();
float read_PH_sensor();
float* read_DHT_sensor();
float read_WATERLEVEL_sensor();
float read_TDS_sensor();
int getMedianNum(int bArray[], int iFilterLen);


#endif