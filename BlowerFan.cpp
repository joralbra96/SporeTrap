#include "esp32-hal-ledc.h"
#include "BlowerFan.h"
#include "Arduino.h"

BlowerFan::BlowerFan(int pwmPin, FlowSensor& flowsensor_ref)
  : flowSensor(flowsensor_ref) {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(pwmPin, ledChannel);
}

void BlowerFan::runPWM(int pwmValue) {
  ledcWrite(ledChannel, pwmValue);
}

void BlowerFan::setSetpoint(float sp) {
  this->sp = sp;
}

void BlowerFan::startPID() {
  pidStatus = true;
  startTimePID = millis();
}

void BlowerFan::finishPID() {
  pv = 0;
  cv = 0;
  cv_1 = 0;
  error = 0;
  error1 = 0;
  error2 = 0;
  this->runPWM(0);
  pidStatus = false;
}

void BlowerFan::stopPID() {
  this->runPWM(0);
  pidStatus = false;
}

void BlowerFan::resumePID() {
  pidStatus = true;
  startTimePID = millis();
}

void BlowerFan::pidLoop(unsigned long currentTime) {


  if (pidStatus) {
    if ((currentTime - startTimePID) > Tm_interval) {
      pv = flowSensor.read();

      error = sp - pv;

      cv = cv_1 + (kp + kd / Tm) * error + (-kp + ki * Tm - 2 * kd / Tm) * error1 + (kd / Tm) * error2;
      cv_1 = cv;
      error2 = error1;
      error1 = error;

      if (cv > 500) {
        cv = 500;
      }

      if (cv < 0) {
        cv = 0;
      }

      this->runPWM(cv * (255 / 500));

      startTimePID = millis();
    }
  }
}