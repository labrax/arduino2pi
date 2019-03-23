
/*
Compiled set of sensors to monitor the room.
Author: github.com/labrax
*/

#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Wire.h>
// 

template <class T>
class Measurement{
private:
  T minV, maxV, sumV;
  short int N;
public:
  Measurement() {
    reset();
  }
  void reset() {
    this->minV = 99999999;
    this->maxV = -99999999;
    this->N = 0;
    this->sumV = 0;
  }
  void add_measurement(T value) {
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


Measurement<double> pir;
Measurement<double> mic;
Measurement<double> phr;
Measurement<float> tmp;
Measurement<float> pressure;

/*
PIR BLOCK
*/
#define PIR_SENSOR_DIGITAL 2

/*
MICROPHONE BLOCK
*/
#define MIC_SENSOR_ANALOG A0

/*
PHOTORESISTOR BLOCK
*/
#define PHR_SENSOR_ANALOG A1

/*
BMP280 BLOCK
*/
Adafruit_BMP280 bmp; // I2C
boolean bmp_is_valid = 0;


/*
CONTROL BLOCK
*/
#define SAMPLE_WINDOW 50 //in ms
#define PRINT_WINDOW 1000
unsigned long nextPrint = 0;


void setup() {
  pinMode(PIR_SENSOR_DIGITAL, INPUT);
  pinMode(MIC_SENSOR_ANALOG, INPUT);
  pinMode(PHR_SENSOR_ANALOG, INPUT);
  
  if(!bmp.begin()) {
    Serial.println("BMP280 NOT FOUND!");
  } else {
    bmp_is_valid = 1;
  }
  
  analogReference(DEFAULT);

  Serial.begin(9600);

  Serial.println(PRINT_WINDOW);
  Serial.println(SAMPLE_WINDOW);
  Serial.println("TIME,PIR,MIC,PHR,TEMP,PRESSURE");
}

void loop(){
  pir.add_measurement(digitalRead(PIR_SENSOR_DIGITAL));
  
  mic_op();
  phr_op();
  if(bmp_is_valid) {
    tmp.add_measurement(bmp.readTemperature());
    pressure.add_measurement(bmp.readPressure());
  }
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
  sample = analogRead(MIC_SENSOR_ANALOG);
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
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  mic.add_measurement(volts);
}

void phr_op() {
  //this will sink the previous analog read and provide enough time for it to be determined for next read
  analogRead(PHR_SENSOR_ANALOG);
  delay(10);
  //
  phr.add_measurement(analogRead(PHR_SENSOR_ANALOG));
}


void print_all() {
  unsigned long curTime = millis();
  if(curTime >= nextPrint) {
    Serial.print(curTime); Serial.print(",");
    Serial.print(pir.get_max()); Serial.print(",");
    Serial.print(mic.get_max()); Serial.print(",");
    Serial.print(""); Serial.print(","); //mic_cic removed
    Serial.print(phr.get_max()); Serial.print(",");
    Serial.print(tmp.get_max()); Serial.print(",");
    Serial.print(pressure.get_max());
    Serial.println();

    pir.reset();
    mic.reset();
    tmp.reset();
    pressure.reset();
    phr.reset();

    nextPrint = nextPrint + PRINT_WINDOW;
  }
}
