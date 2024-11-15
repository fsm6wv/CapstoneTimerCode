#ifndef SENSORS_H
#define SENSORS_H

#include "DHT.h"
#include <ESP32AnalogRead.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11  // Sensor type
#define TDS_PIN 3
#define WATERLEVEL_PIN 4
#define PH_PIN 5

#define dhtHumidityBufferSize 9
#define dhtTemperatureBufferSize 9
#define tdsBufferSize 9
#define phBufferSize 9
#define waterlevelBufferSize 5

extern float dhtHumidityBuffer[];
extern int dhtHumidityBufferIndex;
extern float dhtTemperatureBuffer[];
extern int dhtTemperatureBufferIndex;
extern float tdsBuffer[];
extern int tdsBufferIndex;
extern float phBuffer[];
extern int phBufferIndex;
extern float waterlevelBuffer[];
extern int waterlevelBufferIndex;

void setup_sensors();
void fill_sensor_buffers();
void print_sensor_values();
float read_PH_sensor();
float* read_DHT_sensor();
float read_WATERLEVEL_sensor();
float read_TDS_sensor();
float getMedian(float* buffer, int bufferSize);


#endif