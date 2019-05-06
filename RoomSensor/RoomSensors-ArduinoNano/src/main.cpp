/*
Compiled set of sensors to monitor the room.
Author: github.com/labrax
*/

#include "main.h"

Sensor<int> pir(2, SENSOR_DIGITAL, 0);
Sensor<int> mic(A0, SENSOR_ANALOG, 10, 50);
Sensor<int> phr(A1, SENSOR_ANALOG, 10);


BME280Interface bme;
BME280SensorTemperature temp(&bme);
BME280SensorPressure pressure(&bme);
BME280SensorHumidity humidity(&bme);

/*
CONTROL BLOCK
*/

#define PRINT_WINDOW 10000
unsigned long nextPrint = 0;


void setup() {
  Serial.begin(115200);

  pir.begin();
  mic.begin();
  phr.begin();

  bme.begin();
}

void loop(){
  pir.read();
  mic.read();
  phr.read();

  temp.read();
  pressure.read();
  humidity.read();
  print_all();
}

void print_all() {
  unsigned long curTime = millis();

  if(curTime >= nextPrint) {
    Serial.print(pir.get_max()); Serial.print(",");
    Serial.print(mic.get_max()); Serial.print(",");
    Serial.print(phr.get_max()); Serial.print(",");
    Serial.print(temp.get_max()); Serial.print(",");
    Serial.print(pressure.get_max()); Serial.print(",");
    Serial.print(humidity.get_max());
    Serial.println();


    pir.reset();
    mic.reset();
    phr.reset();
    temp.reset();
    pressure.reset();
    humidity.reset();

    nextPrint = nextPrint + PRINT_WINDOW;
  }
}
