#include "Configuration.h"

Configuration::Configuration(Preferences& preferences_ref)
  : preferences(preferences_ref) {
}

void Configuration::load() {
  preferences.begin("config", false);
  this->device_id = preferences.getString("id", "UNKNOWN_ID");
  this->device_name = preferences.getString("name", "UNKNOWN_NAME");
  this->working_mode = preferences.getUChar("w_mode", 0);
  this->working_period = preferences.getUChar("w_period", 168);
  this->start_timestamp = preferences.getULong("start_time", 0);
  this->end_timestamp = preferences.getULong("end_time", 0);
  this->last_timestamp = preferences.getULong("last_time", 0);
  this->last_step_timestamp = preferences.getULong("last_step", 0);
  this->flow_setpoint = preferences.getUInt("flow_setpoint", 10.0);


  Serial.println("SporeTrap Configuration:");
  Serial.printf("ID: %s\n", this->device_id);
  Serial.printf("NAME: %s\n", this->device_name);
  Serial.printf("WORKING MODE: %u\n", this->working_mode);
  Serial.printf("WORKING PERIOD: %u\n", this->working_period);
  Serial.printf("FLOW SETPOINT: %f\n", this->flow_setpoint);
  Serial.printf("START TIME: %u\n", this->start_timestamp);
  Serial.printf("END TIME: %u\n", this->end_timestamp);
  Serial.printf("LAST TIME SAVED: %u\n", this->last_timestamp);
  Serial.printf("LAST STEP TIME SAVED: %u\n", this->last_step_timestamp);
}


void Configuration::reload() {
  preferences.begin("config", false);
  this->device_id = preferences.getString("id", "UNKNOWN_ID");
  this->device_name = preferences.getString("name", "UNKNOWN_NAME");
  this->flow_setpoint = preferences.getUInt("flow_setpoint", 10.0);
  this->working_mode = preferences.getUChar("w_mode", 0);
  this->working_period = preferences.getUChar("w_period", 168);
  this->start_timestamp = preferences.getULong("start_time", 0);
  this->end_timestamp = preferences.getULong("end_time", 0);
  this->last_timestamp = preferences.getULong("last_time", 0);
  this->last_step_timestamp = preferences.getULong("last_step", 0);
}


String Configuration::getDeviceName() {
  this->device_name = preferences.getString("name", "UNKNOWN_NAME");
  return this->device_name;
}

float Configuration::getFlowSetpoint() {
  this->flow_setpoint = preferences.getFloat("flow_setpoint", 10.0);
  return this->flow_setpoint;
}

uint8_t Configuration::getWorkingPeriod() {
  this->working_period = preferences.getUChar("w_period", 7);
  return this->working_period;
}

uint8_t Configuration::getWorkingMode() {
  this->working_mode = preferences.getUChar("w_mode", 0);
  return this->working_mode;
}


unsigned long Configuration::getStartTimestamp() {
  this->start_timestamp = preferences.getULong("start_time", 0);
  return this->start_timestamp;
}

unsigned long Configuration::getEndTimestamp() {
  this->end_timestamp = preferences.getULong("end_time", 0);
  return this->end_timestamp;
}

unsigned long Configuration::getLastTimestamp() {
  this->last_timestamp = preferences.getULong("last_time", 0);
  return this->last_timestamp;
}

unsigned long Configuration::getLastStepTimestamp() {
  this->last_step_timestamp = preferences.getULong("last_step", 0);
  // Serial.print("Configuration::getLastStepTimestamp(): ");
  // Serial.println(this->last_step_timestamp);
  return this->last_step_timestamp;
}


void Configuration::setDeviceName(const char* deviceName) {
  preferences.putString("name", deviceName);
  this->getDeviceName();
}

void Configuration::setFlowSetpoint(float flow) {
  preferences.putFloat("flow_setpoint", flow);
  this->getFlowSetpoint();
}

void Configuration::setWorkingPeriod(uint8_t workingPeriod) {
  preferences.putUChar("w_period", workingPeriod);
  this->getWorkingPeriod();
}

void Configuration::setWorkingMode(uint8_t workingMode) {
  preferences.putUChar("w_mode", workingMode);
  this->getWorkingMode();
}

void Configuration::setStartTimestamp(unsigned long timestamp) {
  preferences.putULong("start_time", timestamp);
  this->getStartTimestamp();
}
void Configuration::setEndTimestamp(unsigned long timestamp) {
  preferences.putULong("end_time", timestamp);
  this->getEndTimestamp();
}

void Configuration::setLastTimestamp(unsigned long timestamp) {
  preferences.putULong("last_time", timestamp);
  this->getLastTimestamp();
}


void Configuration::setLastStepTimestamp(unsigned long timestamp) {
  preferences.putULong("last_step", timestamp);
  this->getLastStepTimestamp();
}
