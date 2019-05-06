
#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Sensor.h"

class BME280Interface {
private:
  boolean bme_is_valid = false;
public:
  Adafruit_BME280 bme; // I2C

  boolean is_valid() {
    return bme_is_valid;
  }

  BME280Interface() {

  }

  void begin() {
    if(bme.begin()) {
      bme_is_valid = true;
    }
  }
};


class BME280SensorTemperature : public Sensor<double> {
private:
  BME280Interface * bme;
protected:
  void internal_read() {
    if(bme->is_valid())
      last_read = bme->bme.readTemperature();
    else
      last_read = 0;
  }

public:
  BME280SensorTemperature(BME280Interface *bme280i) : Sensor(0, false, 0, 0), bme(bme280i) {}

  void begin() {}
};


class BME280SensorPressure : public Sensor<double> {
private:
  BME280Interface * bme;
protected:
  void internal_read() {
    if(bme->is_valid())
      last_read = bme->bme.readPressure();
    else
      last_read = 0;
  }

public:
  BME280SensorPressure(BME280Interface *bme280i) : Sensor(0, false, 0, 0), bme(bme280i) {}

  void begin() {}
};


class BME280SensorHumidity : public Sensor<double> {
private:
  BME280Interface * bme;
protected:
  void internal_read() {
    if(bme->is_valid())
      last_read = bme->bme.readHumidity();
    else
      last_read = 0;
  }

public:
  BME280SensorHumidity(BME280Interface *bme280i) : Sensor(0, false, 0, 0), bme(bme280i) {}

  void begin() {}
};


#endif
