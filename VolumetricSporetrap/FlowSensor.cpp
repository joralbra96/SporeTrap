#include "esp32-hal-adc.h"
#include "FlowSensor.h"
#include "Arduino.h"



FlowSensor::FlowSensor(int sensorPin) {
  this->sensorPin = sensorPin;
}



void FlowSensor::begin() {
  pinMode(sensorPin, INPUT);
}



float FlowSensor::read() {
  float V1 = (analogRead(sensorPin) * ADC_MAX_VOLTAGE) / ADC_BITS;
  float Vs = ((R1 + R2) / R2) * V1;
  float flow = RANGE * ((Vs - SENSOR_ZERO_VOLTAGE) / (SENSOR_FULL_RANGE_VOLTAG - SENSOR_ZERO_VOLTAGE));
  return flow;
}
