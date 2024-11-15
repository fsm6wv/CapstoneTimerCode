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

float read_PH_sensor(){
  float phVoltage = adc3.readVoltage();
  float phValue = 3.5 * phVoltage; // Placeholder for pH conversion
  return phValue;
}
float* read_DHT_sensor(){
  static float data[2]; // Static to ensure the data doesn't go out of scope
  data[0] = dht.readTemperature();
  data[1] = dht.readHumidity();
  return data;
}
float read_WATERLEVEL_sensor(){
  float waterLevelVoltage = adc2.readVoltage();
  return waterLevelVoltage;
}
float read_TDS_sensor(){
  static unsigned long sampleTime = millis();
  static unsigned long printTime = millis();
  for(;;){
    // TDS Sensor Sampling every 40 ms
    if (millis() - sampleTime > 40U) {
      sampleTime = millis();
      analogBuffer[analogBufferIndex] = adc1.readRaw(); // Read TDS sensor
      analogBufferIndex++;
      if (analogBufferIndex == SCOUNT) analogBufferIndex = 0; // Reset if buffer is full
    }

    // Print and calculate sensor values every 800 ms
    if (millis() - printTime > 800U) {
      printTime = millis();

      // Calculate TDS using the median filter
      int filteredAnalog = getMedianNum(analogBuffer, SCOUNT);
      float medianTdsVoltage = filteredAnalog * (3.3 / 1024.0); // Convert to voltage
      float compensationCoefficient = 1.0 + 0.015 * (temperature - 25.0); // Temp compensation
      float compensatedVoltage = medianTdsVoltage / compensationCoefficient;

      // TDS calculation based on SEN0244 Datasheet
      float tdsValue = (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage)
                       - (255.86 * compensatedVoltage * compensatedVoltage)
                       + (857.39 * compensatedVoltage);
      tdsValue *= 0.25; // Calibration adjustment

      return tdsValue;
    }
  }
}

// Median filter function
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (int i = 0; i < iFilterLen; i++) bTab[i] = bArray[i];
  for (int j = 0; j < iFilterLen - 1; j++) {
    for (int i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        int bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) return bTab[(iFilterLen - 1) / 2];
  return (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
}