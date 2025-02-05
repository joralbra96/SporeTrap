#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <Preferences.h>

class Configuration {

public:
  String device_id;
  String device_name;
  uint8_t working_mode;
  uint8_t working_period;
  unsigned long start_timestamp;
  unsigned long end_timestamp;
  unsigned long last_timestamp;
  unsigned long last_step_timestamp;
  float flow_setpoint;

  Preferences& preferences;
  Configuration(Preferences& preferences_ref);
  void load();
  void reload();

  void setDeviceName(const char* deviceName);
  void setFlowSetpoint(float flow);
  void setStartTimestamp(unsigned long timestamp);
  void setEndTimestamp(unsigned long timestamp);
  void setLastTimestamp(unsigned long timestamp);
  void setLastStepTimestamp(unsigned long timestamp);
  void setWorkingPeriod(uint8_t workingPeriod);
  void setWorkingMode(uint8_t workingMode);

  String getDeviceName();
  float getFlowSetpoint();
  unsigned long getStartTimestamp();
  unsigned long getEndTimestamp();
  unsigned long getLastTimestamp();
  unsigned long getLastStepTimestamp();
  uint8_t getWorkingPeriod();
  uint8_t getWorkingMode();
};

#endif
