
/*
Compiled set of sensors to monitor the room.
Author: github.com/labrax
*/

#include "main.h"

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
//


template <class T>
class Measurement{
private:
  T minV, maxV, sumV;
  short int N;
  boolean any_value = false;
public:
  Measurement() {
    reset();
  }
  void reset() {
    any_value = false;
    N = 0;
    sumV = 0;
  }
  void add_measurement(T value) {
    if(not any_value) {
      minV = value;
      maxV = value;
    }
    if(value < minV)
      minV = value;
    if(value > maxV)
      maxV = value;
    N++;
    sumV += value;
  }
  T get_min() {
    return this->minV;
  }
  T get_max() {
    return this->maxV;
  }
  T get_sum() {
    return this->sumV;
  }
  T get_N() {
    return this->N;
  }
};


Measurement<double> mic;
Measurement<double> phr;
Measurement<double> temp;

#define MIC_SENSOR_ANALOG A2
#define PHR_SENSOR_ANALOG A1
#define TEMP_SENSOR_DIGITAL 2

/*
DS18B20 BLOCK
*/
OneWire oneWire(TEMP_SENSOR_DIGITAL);
DallasTemperature sensors(&oneWire);


/*
CONTROL BLOCK
*/
#define SAMPLE_WINDOW 50 //in ms
#define PRINT_WINDOW 1000
unsigned long nextPrint = 0;


void setup() {
  pinMode(TEMP_SENSOR_DIGITAL, INPUT);
  pinMode(MIC_SENSOR_ANALOG, INPUT);
  pinMode(PHR_SENSOR_ANALOG, INPUT);

  //analogReference(DEFAULT);

  Serial.begin(115200);

  Serial.println(PRINT_WINDOW);
  Serial.println(SAMPLE_WINDOW);
  Serial.println("TIME,MIC,PHR,TEMP");
}

void loop(){
  mic_op();
  phr_op();
  temp_op();

  print_all();
}


void mic_op() {
  //extracted from: https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  unsigned int sample;

  //sink previous values
  analogRead(MIC_SENSOR_ANALOG);
  delay(10);


  // collect data for 50 mS
  while (millis() - startMillis < SAMPLE_WINDOW)
  {
     sample = analogRead(MIC_SENSOR_ANALOG);
     if (sample < 1024)  // toss out spurious readings
     {
        if (sample > signalMax)
        {
           signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
           signalMin = sample;  // save just the min levels
        }
     }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 3.3) / 1024;  // convert to volts

  mic.add_measurement(volts);

  //mic.add_measurement(analogRead(ANALOG_INPUT));
}

void phr_op() {
  analogRead(PHR_SENSOR_ANALOG);
  //this will sink the previous analog read and provide enough time for it to be determined for next read
  delay(10);
  //
  phr.add_measurement(analogRead(PHR_SENSOR_ANALOG));
}

void temp_op() {
  sensors.requestTemperatures();
  temp.add_measurement(sensors.getTempCByIndex(0));
}


void print_all() {
  unsigned long curTime = millis();
  if(curTime >= nextPrint) {
    Serial.print(curTime); Serial.print(",");
    Serial.print(mic.get_max()); Serial.print(",");
    Serial.print(phr.get_max()); Serial.print(",");
    Serial.print(temp.get_max());
    Serial.println();

    phr.reset();
    mic.reset();
    temp.reset();

    nextPrint = nextPrint + PRINT_WINDOW;
  }
}
