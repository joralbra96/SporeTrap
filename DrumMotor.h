#ifndef _DRUMMOTOR_H
#define _DRUMMOTOR_H
#include "Arduino.h"

class DrumMotor {
public:
  int stepPin = 2;
  const int DIR_PIN = 3;
  const int ENABLE_PIN = 4;
  const int MODE_0_PIN = 0;
  const int MODE_1_PIN = 0;
  const int MODE_2_PIN = 0;

  DrumMotor(int stepPin);

  void step();
  
};

#endif
