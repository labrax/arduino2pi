/*
Compiled set of sensors to monitor the room.
Author: github.com/labrax
*/

#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Wire.h>
// 

/*
PIR BLOCK
*/
#define PIR_LED 11
#define PIR_SENSOR_DIGITAL 2
int pir_val = LOW;
int pir_state = LOW;

/*
MICROPHONE BLOCK
*/
#define MIC_LED 12
#define MIC_LED_ACTIVATION 0.02
#define MIC_SENSOR_ANALOG A0

unsigned int mic_sample;
double mic_volts;

#define MIC_CYCLE_LENGTH 10000
unsigned int mic_cycle = 0; //timer for the led

unsigned long startMillis = 0;  // Start of sample window
unsigned int peakToPeak = 0;   // peak-to-peak level
unsigned int signalMax = 0;
unsigned int signalMin = 1024;

/*
PHOTORESISTOR BLOCK
*/
#define PHR_LED 10
#define PHR_SENSOR_ANALOG A1
unsigned int phr_sample;

/*
BMP280 BLOCK
*/
Adafruit_BMP280 bmp; // I2C
float bmp_temp=-99;
float bmp_pressure=-99;
boolean is_valid = 0;


/*
CONTROL BLOCK
*/
#define SAMPLE_WINDOW 50 //in ms
#define PRINT_WINDOW 50
unsigned int nextPrint = 0;


void setup() {
  pinMode(PIR_LED, OUTPUT);      // initalize LED as an output
  pinMode(PIR_SENSOR_DIGITAL, INPUT);    // initialize sensor as an input

  pinMode(MIC_LED, OUTPUT);
  pinMode(MIC_SENSOR_ANALOG, INPUT);
  
  pinMode(PHR_LED, OUTPUT);
  pinMode(PHR_SENSOR_ANALOG, INPUT);
  
  if(!bmp.begin()) {
    Serial.println("BMP280 NOT FOUND!");
  }
  
  analogReference(DEFAULT);

  Serial.begin(9600);        // initialize serial

  //  if (!bme.begin()) {  
  //    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  //    while (1);
  //  }
  Serial.println(PRINT_WINDOW);
  Serial.println(SAMPLE_WINDOW);
  Serial.println("TIME,PIR,MIC,CYC,PHR,TEMP,PRESSURE");
}

void loop(){
  pir_op();
  mic_op();
  phr_op();
  bmp280();
  print_all();
}

void pir_op() {
  pir_val = digitalRead(PIR_SENSOR_DIGITAL);   // read sensor value
  if (pir_val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(PIR_LED, HIGH);   // turn LED ON
    pir_state = HIGH;       // update variable state to HIGH
  } 
  else {
    digitalWrite(PIR_LED, LOW); // turn LED OFF
    pir_state = LOW;       // update variable state to LOW
  }
  return;
}
  

void mic_op() {
  if (millis() - startMillis < SAMPLE_WINDOW)
  {
    mic_sample = analogRead(MIC_SENSOR_ANALOG);
    if (mic_sample < 1024)  // toss out spurious readings
    {
      if (mic_sample > signalMax)
        signalMax = mic_sample;  // save just the max levels
      else if (mic_sample < signalMin)
        signalMin = mic_sample;  // save just the min levels
    }
  } else {
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    mic_volts = (peakToPeak * 5.0) / 1024;  // convert to volts
    
    startMillis = millis(); //restart settings
    signalMax = 0;
    signalMin = 1024;
  
    if(mic_volts >= MIC_LED_ACTIVATION) {
      digitalWrite(MIC_LED, HIGH);
      mic_cycle = MIC_CYCLE_LENGTH;
    }
  }
  
  if(mic_cycle == 0)
    digitalWrite(MIC_LED, LOW);

  if(mic_cycle <= MIC_CYCLE_LENGTH && mic_cycle > 0)
    mic_cycle -= 1;
  return;  
}

void phr_op() {
  phr_sample = analogRead(PHR_SENSOR_ANALOG);
  return;
}

void bmp280() {
  bmp_temp = bmp.readTemperature();
  bmp_pressure = bmp.readPressure();
  return;
}

void print_all() {
  unsigned int curTime = millis();
  if(curTime >= nextPrint) {
    Serial.print(curTime); Serial.print(",");
    Serial.print(pir_val); Serial.print(",");
    Serial.print(mic_volts); Serial.print(",");
    Serial.print(mic_cycle); Serial.print(",");
    Serial.print(phr_sample); Serial.print(",");
    Serial.print(bmp_temp); Serial.print(",");
    Serial.print(bmp_pressure);
    Serial.println();
    
    nextPrint = nextPrint + PRINT_WINDOW;
  }
  return;
}
