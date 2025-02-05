#include "BatterySensor.h"
#include "Arduino.h"



BatterySensor::BatterySensor(int sensorPin) {
  this->sensorPin = sensorPin;
}



void BatterySensor::begin() {
  pinMode(sensorPin, INPUT);

  analogReadResolution(12);
}



float BatterySensor::read() {
  return analogRead(sensorPin) * sensorCte;
}
