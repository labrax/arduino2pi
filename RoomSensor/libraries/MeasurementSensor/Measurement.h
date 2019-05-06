
#ifndef MEASUREMENT_H
#define MEASUREMENT_H

template <class T>
class Measurement {
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

#endif
