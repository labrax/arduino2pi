
#ifndef SENSORSET_H
#define SENSORSET_H

#include "Sensor.h"
#include <vector>

class SensorSet {
private:
  vector<String> names;
  vector<Sensor> sensors;
public:
  void add_sensor(String &name, Sensor &sensor) {
    names.insert(names.end(), name);
    sensors.insert(sensors.end(), sensor);
    sensor.begin();
  }

  void read_all() {
    for(Sensor & s : sensors)
      s.read();
  }
};

#endif
