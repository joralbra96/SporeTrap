#ifndef _BLOWERFAN_
#define _BLOWERFAN_
#include "FlowSensor.h"

class BlowerFan {
private:
  int pwmPin;
  const int freq = 5000;
  const int ledChannel = 0;
  const int resolution = 8;



  bool pidStatus = false;
  float pv;
  float cv;
  float cv_1;
  float error;
  float error1;
  float error2;
  float kp=1;
  float ki=1;
  float kd=0.01;
  float Tm = 0.1;
  float Tm_interval = Tm * 1000;
  float sp = 10;
  unsigned long startTimePID = 0;

public:
  FlowSensor& flowSensor;

  BlowerFan(int pwmPin, FlowSensor& flowsensor_ref);

  void pidLoop(unsigned long currentTime);

  void runPWM(int pwmValue);

  void setSetpoint(float sp);

  void startPID();
  void finishPID();
  void resumePID();
  void stopPID();
};

#endif