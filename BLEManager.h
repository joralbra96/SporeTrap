#ifndef _BLEMANAGER_H
#define _BLEMANAGER_H
#include "Manager.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>


class BLEManager {
private:
  Manager& manager;
  const char* SERVICE_CHAR_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  const char* OTA_SERVICE_CHAR_UUID = "c8659210-af91-4ad3-a995-a58d6fd26145";

  const char* NAME_CHAR_UUID = "72d50e32-c810-11ed-afa1-0242ac120002";
  const char* VOLTAGE_CHAR_UUID = "72d51102-c810-11ed-afa1-0242ac120002";
  const char* DATE_CHAR_UUID = "72d514d6-c810-11ed-afa1-0242ac120002";
  const char* START_PROGRAM_DATE_CHAR_UUID = "72d514d6-c810-11ed-afa1-0242ac120003";
  const char* FLOW_SETPOINT_CHAR_UUID = "72d51670-c810-11ed-afa1-0242ac120002";
  const char* FLOW_VALUE_CHAR_UUID = "72d51800-c810-11ed-afa1-0242ac120002";
  const char* WORKING_PERIOD_CHAR_UUID = "72d5197c-c810-11ed-afa1-0242ac120002";
  const char* WORKING_MODE_CHAR_UUID = "72d5197c-c810-11ed-afa1-0242ac120003";
  const char* LOGS_CHAR_UUID = "c8659211-af91-4ad3-a995-b58d6fd26145";
  const char* OTA_CHAR_UUID = "c8659211-af91-4ad3-a995-a58d6fd26145";
  const char* HW_VERSION_CHAR_UUID = "c8659212-af91-4ad3-a995-a58d6fd26145";





public:
  BLECharacteristic* pFlowValueCharacteristic;
  BLECharacteristic* pNameCharacteristic;
  BLECharacteristic* pVoltageCharacteristic;
  BLECharacteristic* pStartProgramDateCharacteristic;
  BLECharacteristic* pDateCharacteristic;
  BLECharacteristic* pFlowSetpointCharacteristic;
  BLECharacteristic* pWorkingModeCharacteristic;
  BLECharacteristic* pWorkingPeriodCharacteristic;
  BLECharacteristic* pLogsCharacteristic;
  BLECharacteristic* pOtaCharacteristic;
  BLECharacteristic* pHWVersionCharacteristic;
  bool init_ota_flag = false;
  unsigned long ota_start_time = 0L;
  unsigned long OTA_TIMEOUT = 60000L;

  unsigned long notify_start_time = 0L;
  unsigned long notify_period = 1000L;




  BLEManager(Manager& manager_ref);
  void begin(const char* server_name);
  void ota_loop(unsigned long currentTime);
  void notify_loop(unsigned long currentTime);
};

#endif
