#include "esp32-hal.h"
#include <string>
#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "Manager.h"
#include "esp_ota_ops.h"

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    // do something when device connects
    Serial.println("A new connection is stablished");
  }

  void onDisconnect(BLEServer* pServer) {
    // do something when device disconnects
    Serial.println("A connection is finished");
    pServer->getAdvertising()->start();
  }
};

class LogsCharacteristicCallback : public BLECharacteristicCallbacks {
public:
  int initFlag = 0;
  int fileSize = 0;
  int requiredReads = 0;
  int actualReads = 0;
  const uint16_t OTA_FULL_PACKET = 512;
  Manager& manager;

  LogsCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};

  void onRead(BLECharacteristic* pChar) {
    // float flowValue = manager.getFlowValue();
    // pChar->setValue(flowValue);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("LogsCharacteristicCallback 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }

    if (initFlag == 0){

      fileSize = manager.eventLogger.getSyslogAsString().length();
      
      requiredReads = ceil(((float)fileSize/(float)OTA_FULL_PACKET)); 
      Serial.print("Tamaño del archivo: ");
      Serial.println(fileSize);
      Serial.print("Lecturas: ");
      Serial.println(requiredReads);
      initFlag = initFlag + 1;
      pChar->setValue(requiredReads);
    }else{
      int startIndex = actualReads * (int)OTA_FULL_PACKET;
      int endIndex = (actualReads * (int)OTA_FULL_PACKET) + ((int)OTA_FULL_PACKET);

      String sub = manager.eventLogger.getSyslogAsString().substring(startIndex, endIndex);
      Serial.print("start: ");
      Serial.println(startIndex);
      Serial.print("end: ");
      Serial.println(endIndex);
      Serial.print("Reading bytes from file: ");
      Serial.println(sub.length());
      Serial.println(sub.c_str());
      actualReads = actualReads + 1;
      pChar->setValue(sub.c_str());
      
    }

  }


  void onWrite(BLECharacteristic* pChar) {
    std::string rxData = pChar->getValue();

    if (rxData.length() > 0) {
      Serial.println("LogsCharacteristicCallback 1 write request received: ");
      initFlag = 0;
      actualReads = 0;
      fileSize = 0;
      requiredReads = 0;
    }
  }
};


class OtaUpdateCharacteristicCallback : public BLECharacteristicCallbacks {
public:
  esp_ota_handle_t otaHandler = 0;
  bool updateFlag = false;
  const uint16_t OTA_FULL_PACKET = 512;
  BLEManager* blemanager;

  OtaUpdateCharacteristicCallback(BLEManager* blemanager_ref)
    : BLECharacteristicCallbacks(), blemanager(blemanager_ref){};


  void onWrite(BLECharacteristic* pChar) {
    std::string rxData = pChar->getValue();
    if (!updateFlag) {  //If it's the first packet of OTA since bootup, begin OTA
      Serial.println("BeginOTA");
      esp_ota_begin(esp_ota_get_next_update_partition(NULL), OTA_SIZE_UNKNOWN, &otaHandler);
      updateFlag = true;
      blemanager->init_ota_flag = true;
    }
    if (rxData.length() > 0) {

      esp_ota_write(otaHandler, rxData.c_str(), rxData.length());
      if (rxData.length() != OTA_FULL_PACKET) {
        Serial.println("Termino");
        Serial.println("EndOTA");
        Serial.flush();

        uint8_t result = esp_ota_end(otaHandler);
        Serial.print("Result: ");
        Serial.println(result);
        if (ESP_OK == result) {
          if (ESP_OK == esp_ota_set_boot_partition(esp_ota_get_next_update_partition(NULL))) {
            Serial.println("Upload OK");
            Serial.flush();
            //delay(5000);
            esp_restart();
          } else {
            Serial.println("Upload Error");
            Serial.flush();
            esp_restart();
          }
        } else {
          Serial.println("Upload RECONTRA Error");
          Serial.flush();
          esp_restart();
        }
      }
    }
  }
};

class FlowValueCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  FlowValueCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    float flowValue = manager.getFlowValue();
    pChar->setValue(flowValue);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("FlowValueCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onNotify(BLECharacteristic* pChar) {
    // float value = map(random(10000), 0, 10000, 1, 10);
    float value = manager.getFlowValue();
    pChar->setValue(value);
  }
};


class NameCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  NameCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    String deviceName = manager.getDeviceName();
    std::string deviceNamestr = std::string(deviceName.c_str());
    pChar->setValue(deviceNamestr);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("NameCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onWrite(BLECharacteristic* pChar) {
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("NameCharacteristic 1 write request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }
};


class VoltageCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  VoltageCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    // float voltage = manager.getVoltageValue();
    // pChar->setValue(voltage);
    // float value1 = map(random(10000), 0, 10000, 1, 10);
    float value1 = manager.getVoltageValue();
    pChar->setValue(value1);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("VoltageCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onNotify(BLECharacteristic* pChar) {
    // float value = map(random(10000), 0, 10000, 1, 10);
    float value = manager.getVoltageValue();
    pChar->setValue(value);
  }
};


class DateCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  DateCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    unsigned long date = manager.getCurrentTimestamp();
    uint32_t dateFormated = (uint32_t)date;

    pChar->setValue(dateFormated);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("DateCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onNotify(BLECharacteristic* pChar) {
    unsigned long date = manager.getCurrentTimestamp();
    uint32_t dateFormated = (uint32_t)date;

    pChar->setValue(dateFormated);
  }

  void onWrite(BLECharacteristic* pChar) {
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("DateCharacteristic 1 write request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");

      uint32_t timestamp;

      // Copy the bytes to the value variable
      memcpy(&timestamp, pChar->getValue().c_str(), sizeof(timestamp));

      // Convert the byte order if necessary (assuming little-endian byte order)

      manager.setDateTimestamp(timestamp);
    }
  }
};

class StartProgramDateCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  StartProgramDateCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    unsigned long utime = manager.getStartProgramDateTimestamp();
    uint32_t time = (uint32_t)utime;
    pChar->setValue(time);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("DateCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println(time);
      Serial.println("*********");
    }
  }
};


class FlowSetpointCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  FlowSetpointCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    float flowSetpointValue = manager.getFlowSetpoint();
    pChar->setValue(flowSetpointValue);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("FlowSetpointCharacteristic 1 read request received: ");
    Serial.print("Value in float: ");
    Serial.println(String(pChar->getValue().c_str()).toFloat());
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onWrite(BLECharacteristic* pChar) {
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("FlowSetpointCharacteristic 1 write request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");

      // float newSetpoint = String(pChar->getValue().c_str()).toFloat();
      float newSetpoint;
      newSetpoint = *(reinterpret_cast<float*>(pChar->getData()));
      Serial.print("New setpoint: ");
      Serial.println(newSetpoint);

      manager.setFlowSetpoint(newSetpoint);
    }
  }
};

class WorkingModeCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  WorkingModeCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    uint8_t workingMode = manager.getProgramWorkingMode();
    uint8_t workingModeBytes[1] = { workingMode };

    pChar->setValue(workingModeBytes, 1);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("WorkingModeCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onWrite(BLECharacteristic* pChar) {
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("WorkingModeCharacteristic 1 write request received: ");
    uint8_t* byteArray = pChar->getData();
    int intValue = (byteArray[0] << 24) | (byteArray[1] << 16) | (byteArray[2] << 8) | byteArray[3];
    Serial.println(intValue);

    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");

      manager.setProgramWorkingMode((uint8_t)intValue);
    }
  }
};


class WorkingPeriodCharacteristicCallback : public BLECharacteristicCallbacks {
private:
  Manager& manager;
public:
  WorkingPeriodCharacteristicCallback(Manager& manager_ref)
    : BLECharacteristicCallbacks(), manager(manager_ref){};
  void onRead(BLECharacteristic* pChar) {
    uint8_t workingPeriod = manager.getWorkingPeriod();
    uint8_t workingPeriodBytes[1] = { workingPeriod };
    pChar->setValue(workingPeriodBytes, 1);
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("WorkingPeriodCharacteristic 1 read request received: ");
    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");
    }
  }

  void onWrite(BLECharacteristic* pChar) {
    std::string value = pChar->getValue();
    Serial.println("*********");
    Serial.println("WorkingPeriodCharacteristic 1 write request received: ");

    uint8_t* byteArray = pChar->getData();
    int intValue = (byteArray[0] << 24) | (byteArray[1] << 16) | (byteArray[2] << 8) | byteArray[3];
    Serial.println(intValue);

    if (value.length() > 0) {
      char buf[200];
      for (int i = 0; i < value.length(); i++) {
        sprintf(buf, "%02X", value[i]);
        Serial.print(buf);
      }
      Serial.println();
      Serial.println("*********");

      manager.setWorkingPeriod((uint8_t)intValue);
    }
  }
};
