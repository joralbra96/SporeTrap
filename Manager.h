#ifndef _MANAGER_
#define _MANAGER_

#include "Configuration.h"
#include "esp32-hal-gpio.h"
#include "Definitions.h"
#include "EventLogger.h"
#include "BlowerFan.h"
#include "DrumMotor.h"
#include <RTClib.h>
#include "BatterySensor.h"


class Manager {
public:
  const uint8_t STOPPED_MODE = 0;
  const uint8_t RUNNING_MODE = 1;
  const uint8_t IDLE_MODE = 2;

  const unsigned long SAVE_TIME_PERIOD = 1000;
  unsigned long last_time_saved = 0;

  Configuration& config;
  EventLogger& eventLogger;
  BlowerFan& blowerFan;
  DrumMotor& drumMotor;
  RTC_DS3231& rtc;
  BatterySensor& batterySensor;

  Manager(Configuration& config_ref, EventLogger& eventlogger_ref, BlowerFan& blowerfan_ref, DrumMotor& drummotor_ref, RTC_DS3231& rtc_ref, BatterySensor& batterysensor_ref);

  void begin();
  bool isProgramInRunningMode();
  bool isProgramInTime();
  void verifyRestart();
  bool isInTime();
  void loop(unsigned long currentTime);
  int getPeriod();
  unsigned long getCurrentTimestamp();

  void startProgram();
  void endProgram();
  void stopProgram();

  String getDeviceName();

  uint8_t getProgramWorkingMode();
  bool setProgramWorkingMode(uint8_t workingMode);

  uint8_t getWorkingPeriod();
  bool setWorkingPeriod(uint8_t workingPeriod);

  float getFlowValue();

  float getFlowSetpoint();
  bool setFlowSetpoint(float setpoint);

  uint32_t getDateTimestamp();
  void setDateTimestamp(uint32_t timestamp);

  unsigned long getStartProgramDateTimestamp();
  void setStartProgramDateTimestamp(unsigned long timestamp);

  float getVoltageValue();
};

#endif
