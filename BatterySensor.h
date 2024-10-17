#ifndef _BATTERYSENSOR_
#define _BATTERYSENSOR_


class BatterySensor {
private:
  double measured_battery;
  int sensorPin;
  float sensorCte;

public:
  BatterySensor(int sensorPin);
  void begin();
  float read();
};

#endif