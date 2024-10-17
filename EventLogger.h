#ifndef _EVENTLOGGER_H
#define _EVENTLOGGER_H
#include "SPIFFS.h"

class EventLogger {

public:
  char fileName[8] = "/syslog";
  File eventFile;

  EventLogger(){};

  void begin();

  void initEventLoggerFile();

  String getSyslogAsString();

  void resetEventLoggerFile();

  void logOnRestartEvent(unsigned long shutdownTimestamp, unsigned long startupTimestamp);
  void logStartTime(unsigned long timestamp);
  void logEndTime(unsigned long timestamp);
  void logStringEvent(char* stringEvent);

};

#endif