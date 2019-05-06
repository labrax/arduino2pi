
#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "Measurement.h"

#define SENSOR_ANALOG true
#define SENSOR_DIGITAL false


template <class T>
class Sensor : public Measurement<T>{
private:
  byte port;
  boolean analog;
  int delay_time;
  int sample_window;

  T signal_query() {
    if(analog)
      return analogRead(port);
    return digitalRead(port);
  }

protected:
  T last_read;

  virtual void internal_read() {
    if(delay_time > 0)
      delay(delay_time);
    if(sample_window > 0) {
      unsigned int signalMax = 0;
      unsigned int signalMin = 1024;
      unsigned int sample;
      unsigned long startMillis = millis();
      while (millis() - startMillis < sample_window) {
         sample = signal_query();
         if (sample < 1024) {
           signalMax = sample > signalMax ? sample : signalMax;
           signalMin = sample < signalMin ? sample : signalMin;
         }
      }
      last_read = signalMax - signalMin;
    } else {
      last_read = signal_query();
    }
  }

public:
  Sensor(byte port, boolean analog=true, int delay_time=10, int sample_window=0):port(port),analog(analog), last_read(0), delay_time(delay_time), sample_window(sample_window) {

  }

  void begin() {
    pinMode(port, INPUT);
  }

  void read() {
    internal_read();
    add_measurement(last_read);
  }
};

#endif
