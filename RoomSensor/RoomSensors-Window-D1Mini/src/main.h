
#ifndef __RSMAIN_H_
#define __RSMAIN_H_

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Measurement.h>
#include <Sensor.h>


void print_all();
int send_data(String sensor_name, String value, String unit);
int send_all();
#endif
