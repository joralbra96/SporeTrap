#include "FS.h"
#include "EventLogger.h"
#include "Arduino.h"


void EventLogger::begin() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  } else {
    initEventLoggerFile();
  }
}

void EventLogger::initEventLoggerFile() {
  Serial.println("EventLogger::initEventLoggerFile()");

  eventFile = SPIFFS.open(fileName, "a+");

  if (!eventFile) {
    Serial.println("There was an error opening the file for writing");
    return;
  }

  eventFile.close();
}

String EventLogger::getSyslogAsString() {
  Serial.println("EventLogger::getSyslogAsString()");

  eventFile = SPIFFS.open(fileName, FILE_READ);

  if (!eventFile) {
    Serial.println("There was an error opening the file for writing");
    return "";
  }

  String result = eventFile.readString();
  Serial.println("File:");
  Serial.println(result);
  Serial.println("EndFile");

  Serial.print("Bytes: ");
  Serial.println(result.length());

  eventFile.close();
  return result;
}

void EventLogger::resetEventLoggerFile() {
  SPIFFS.remove(fileName);
}

void EventLogger::logOnRestartEvent(unsigned long shutdownTimestamp, unsigned long startupTimestamp) {
  Serial.println("EventLogger::logOnRestartEvent() start");
  eventFile = SPIFFS.open(fileName, "a+");
  if (!eventFile) {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  eventFile.print("SD_");
  eventFile.print(shutdownTimestamp);
  eventFile.println();
  eventFile.print("SU_");
  eventFile.print(startupTimestamp);
  eventFile.println();

  eventFile.close();
  Serial.println("EventLogger::logOnRestartEvent() end");
}

void EventLogger::logStartTime(unsigned long timestamp) {
  eventFile = SPIFFS.open(fileName, "a+");

  if (!eventFile) {
    Serial.println("There was an error opening the file for writing");
    return;
  }

  eventFile.print("ST_");
  eventFile.print(timestamp);
  eventFile.println();


  eventFile.close();
}
void EventLogger::logEndTime(unsigned long timestamp) {
  eventFile = SPIFFS.open(fileName, "a+");

  if (!eventFile) {
    Serial.println("There was an error opening the file for writing");
    return;
  }

  eventFile.printf("ET_%d\n", timestamp);

  eventFile.close();
}
void EventLogger::logStringEvent(char* stringEvent) {
  eventFile = SPIFFS.open(fileName, "a+");

  if (!eventFile) {
    Serial.println("There was an error opening the file for writing");
    return;
  }

  eventFile.printf("SE_%s\n", stringEvent);

  eventFile.close();
}
