#include "DrumMotor.h";


DrumMotor::DrumMotor(int stepPin) {
  // set the pins as outputs
  this->stepPin = stepPin;
  pinMode(this->stepPin, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  // set the enable pin low to enable the motor driver
  digitalWrite(ENABLE_PIN, LOW);
}

void DrumMotor::step() {
  Serial.println("DrumMotor::step()");
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(stepPin, LOW);
}