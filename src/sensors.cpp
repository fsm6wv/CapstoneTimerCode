#include "sensors.h"

DHT dht(DHT_PIN, DHT_TYPE); // Create a DHT object
ESP32AnalogRead adc1;     // For TDS sensor
ESP32AnalogRead adc2;     // For Water level sensor
ESP32AnalogRead adc3;     // For pH sensor

const int SCOUNT = 10; // Sum of sample points for median filter
int analogBuffer[SCOUNT]; // Buffer for TDS sensor readings
int analogBufferIndex = 0;
float temperature = 25.0; // Initial temperature for TDS compensation

void setup_sensors() {
  Serial.begin(9600);           // Initialize serial communication
  dht.begin();                  // Initialize DHT sensor
  adc1.attach(TDS_PIN);         // Attach TDS pin
  adc2.attach(WATERLEVEL_PIN);  // Attach water level sensor pin
  adc3.attach(PH_PIN);          // Attach pH sensor pin
}

void fill_sensor_buffers(){
    for (int i=0; i<10; i+=1){
        read_DHT_sensor();
        read_TDS_sensor();
        read_PH_sensor();
        read_WATERLEVEL_sensor();
    }
}

void print_sensor_values(){
Serial.println("Median ph: " + String(getPhValue()));
Serial.println("Median waterLevel: " + String(getWaterlevelValue()));
Serial.println("Median Humidity: " + String(getDhtHumidityValue()) + " %");
Serial.println("Median Temperature: " + String(getDhtTemperatureValue()) + " degrees C");
Serial.println("Median tds : " + String(getTdsValue()) + " (ppm)");
Serial.println();
}

float read_PH_sensor(){
  float phVoltage = adc3.readVoltage();
  float phValue = (3.5 * phVoltage) - 2.2; // Adjusted so tapwater = pH 7
  phBuffer[phBufferIndex] = phValue;
  phBufferIndex = (phBufferIndex + 1) % phBufferSize;
  return phValue;
}
float* read_DHT_sensor(){
  static float data[2]; // Static to ensure the data doesn't go out of scope
  data[0] = dht.readTemperature();
  data[1] = dht.readHumidity();
  dhtTemperatureBuffer[dhtTemperatureBufferIndex] = data[0];
  dhtHumidityBuffer[dhtHumidityBufferIndex] = data[1];
  dhtTemperatureBufferIndex = (dhtTemperatureBufferIndex + 1) % dhtTemperatureBufferSize;
  dhtHumidityBufferIndex = (dhtHumidityBufferIndex + 1) % dhtHumidityBufferSize;
  return data;
}
float read_WATERLEVEL_sensor(){
  float waterlevelVoltage = adc2.readVoltage();
  waterlevelBuffer[waterlevelBufferIndex] = waterlevelVoltage;
  waterlevelBufferIndex = (waterlevelBufferIndex + 1) % waterlevelBufferSize;
  return waterlevelVoltage;
}
float read_TDS_sensor(){
  float tdsVoltage = adc1.readVoltage();
  //float tdsVoltage = adc1.readRaw() * (3.3 / 1024.0); // Convert to voltage
  float compensationCoefficient = 1.0 + 0.015 * (temperature - 25.0); // Temp compensation
  float compensatedVoltage = tdsVoltage / compensationCoefficient;
  // TDS calculation based on SEN0244 Datasheet
  float tdsValue = (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage)
                       - (255.86 * compensatedVoltage * compensatedVoltage)
                       + (857.39 * compensatedVoltage);
  tdsValue *= 0.25; // Calibration adjustment
  tdsBuffer[tdsBufferIndex] = tdsValue;
  tdsBufferIndex = (tdsBufferIndex + 1) % tdsBufferSize;
  return tdsValue; //units ppm
}

float getMedian(float* buffer, int bufferSize) {
  // Create a copy of the buffer since sorting modifies the data
  float tempBuffer[bufferSize];
  for(int i = 0; i < bufferSize; i++) {tempBuffer[i] = buffer[i];}
  std::sort(tempBuffer, tempBuffer + bufferSize);
  // Calculate median
  if (bufferSize % 2 == 0){return (tempBuffer[bufferSize/2 - 1] + tempBuffer[bufferSize/2]) / 2.0f;
  }else{return tempBuffer[bufferSize/2];}
}

float getPhValue(){return getMedian(phBuffer, phBufferSize);}
float getTdsValue(){return getMedian(tdsBuffer, tdsBufferSize);}
float getWaterlevelValue(){return getMedian(waterlevelBuffer, waterlevelBufferSize);}
float getDhtTemperatureValue(){return getMedian(dhtTemperatureBuffer, dhtTemperatureBufferSize);}
float getDhtHumidityValue(){return getMedian(dhtHumidityBuffer, dhtHumidityBufferSize);}