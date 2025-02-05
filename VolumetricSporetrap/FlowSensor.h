#ifndef _FLOWSENSOR_
#define _FLOWSENSOR_


class FlowSensor {
private:
  double measured_flow;
  int sensorPin;
  const int RANGE = 150;
  const float SENSOR_ZERO_VOLTAGE = 0.5;
  const float SENSOR_FULL_RANGE_VOLTAG = 4.5;
  const float SENSOR_VREF = 5;
  const float R1 = 10000;
  const float R2 = 10000;
  const float ADC_BITS = 4095;
  const float ADC_MAX_VOLTAGE = 3.3;


public:
  FlowSensor(int sensorPin);
  void begin();
  float read();
};

#endif