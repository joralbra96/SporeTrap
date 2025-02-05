#include "BLEManager.h"
#include "BLECallbacks.h"
#include "HardwareVersion.h"

BLEManager::BLEManager(Manager& manager_ref)
  : manager(manager_ref) {
}

void BLEManager::notify_loop(unsigned long currentTime){
  if ((currentTime-notify_start_time) > notify_period){
    pVoltageCharacteristic->notify();
    pFlowValueCharacteristic->notify();
    pDateCharacteristic->notify();

    notify_start_time = currentTime;
  }
}

void BLEManager::ota_loop(unsigned long currentTime) {
  if (this->init_ota_flag == true) {
    if (this->ota_start_time == 0) {
      Serial.println("Ota start time");
      this->ota_start_time = currentTime;
    }
    if ((currentTime - this->ota_start_time) > this->OTA_TIMEOUT) {
      Serial.println("Timeout on OTA Firmware Update");

      Serial.print("CurrentTime: ");
      Serial.println(currentTime);
      Serial.print("ota_start_time: ");
      Serial.println(this->ota_start_time);
      Serial.print("OTA_TIMEOUT: ");
      Serial.println(this->OTA_TIMEOUT);
      Serial.println((currentTime - this->ota_start_time) > this->OTA_TIMEOUT);
      Serial.println((currentTime - this->ota_start_time));
      Serial.println(OTA_TIMEOUT);

      ESP.restart();
    }
  }
}


void BLEManager::begin(const char* server_name) {

  BLEDevice::init(server_name);

  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(BLEUUID(SERVICE_CHAR_UUID), 20);
  BLEService* pOtaService = pServer->createService(BLEUUID(OTA_SERVICE_CHAR_UUID), 20);

  pServer->setCallbacks(new ServerCallbacks());

  pFlowValueCharacteristic = pService->createCharacteristic(FLOW_VALUE_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pFlowValueCharacteristic->setCallbacks(new FlowValueCharacteristicCallback(manager));

  pNameCharacteristic = pService->createCharacteristic(NAME_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pNameCharacteristic->setCallbacks(new NameCharacteristicCallback(manager));

  pVoltageCharacteristic = pService->createCharacteristic(VOLTAGE_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pVoltageCharacteristic->setCallbacks(new VoltageCharacteristicCallback(manager));

  pStartProgramDateCharacteristic = pService->createCharacteristic(START_PROGRAM_DATE_CHAR_UUID, BLECharacteristic::PROPERTY_READ);
  pStartProgramDateCharacteristic->setCallbacks(new StartProgramDateCharacteristicCallback(manager));

  pDateCharacteristic = pService->createCharacteristic(DATE_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  pDateCharacteristic->setCallbacks(new DateCharacteristicCallback(manager));

  pFlowSetpointCharacteristic = pService->createCharacteristic(FLOW_SETPOINT_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pFlowSetpointCharacteristic->setCallbacks(new FlowSetpointCharacteristicCallback(manager));

  pWorkingModeCharacteristic = pService->createCharacteristic(WORKING_MODE_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pWorkingModeCharacteristic->setCallbacks(new WorkingModeCharacteristicCallback(manager));

  pWorkingPeriodCharacteristic = pService->createCharacteristic(WORKING_PERIOD_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pWorkingPeriodCharacteristic->setCallbacks(new WorkingPeriodCharacteristicCallback(manager));

  pLogsCharacteristic = pOtaService->createCharacteristic(LOGS_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pLogsCharacteristic->setCallbacks(new LogsCharacteristicCallback(manager));

  pOtaCharacteristic = pOtaService->createCharacteristic(OTA_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE);
  pOtaCharacteristic->setCallbacks(new OtaUpdateCharacteristicCallback(this));

  pHWVersionCharacteristic = pService->createCharacteristic(HW_VERSION_CHAR_UUID, BLECharacteristic::PROPERTY_READ);
  pHWVersionCharacteristic->setValue(HARDWARE_VERSION);
  Serial.print("Hardware version: ");
  Serial.println(HARDWARE_VERSION);


  // start advertising

  pService->start();
  pOtaService->start();
  pServer->getAdvertising()->start();
  Serial.println("BLE server started");
}
