#include "FlowSensor.h"
#include "BatterySensor.h"
#include "BlowerFan.h"
#include "DrumMotor.h"
#include <Wire.h>
#include <RTClib.h>
#include "Configuration.h"
#include "Preferences.h"
#include "EventLogger.h"
#include "Manager.h"
#include "BLEManager.h"


const uint8_t PWM_BLOWER_FAN_PIN = 26;
const uint8_t ADC_FLOW_SENSOR_PIN = 32;

const uint8_t ADC_BATTERY_SENSOR_PIN = 33;

const uint8_t STEPPER_STEP_PIN = 19;
const uint8_t STEPPER_DIR_PIN = 18;
const uint8_t STEPPER_MODE_0_PIN = 5;
const uint8_t STEPPER_MODE_1_PIN = 17;
const uint8_t STEPPER_MODE_2_PIN = 16;



RTC_DS3231 rtc;

FlowSensor flowSensor(ADC_FLOW_SENSOR_PIN);
BlowerFan blowerFan(PWM_BLOWER_FAN_PIN, flowSensor);
DrumMotor drumMotor(STEPPER_STEP_PIN);
BatterySensor batterySensor(ADC_BATTERY_SENSOR_PIN);

Preferences preferences;
Configuration config(preferences);
EventLogger eventLogger;
Manager manager(config, eventLogger, blowerFan, drumMotor, rtc, batterySensor);
BLEManager blemanager(manager);

void rtcBegin() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  config.load();
  Serial.println("Config loaded...");
  rtcBegin();
  Serial.println("RTC loaded...");
  batterySensor.begin();
  Serial.println("BatterySensor loaded...");
  eventLogger.begin();
  Serial.println("EventLogger loaded...");
  flowSensor.begin();
  Serial.println("FlowSensor loaded...");
  manager.begin();
  Serial.println("Manager loaded...");
  blemanager.begin(manager.config.getDeviceName().c_str());
  Serial.println("BLE Manager loaded...");
  Serial.println("ENd of begin");
  //manager.verifyRestart();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

  manager.loop(currentTime);
  blemanager.notify_loop(currentTime);
  blemanager.ota_loop(currentTime);
}
