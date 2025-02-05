#include "Manager.h"
#include "Configuration.h"
#include "BlowerFan.h"
#include "DrumMotor.h"


Manager::Manager(Configuration& config_ref, EventLogger& eventlogger_ref, BlowerFan& blowerfan_ref, DrumMotor& drummotor_ref, RTC_DS3231& rtc_ref, BatterySensor& batterysensor_ref)
  : config(config_ref), eventLogger(eventlogger_ref), blowerFan(blowerfan_ref), drumMotor(drummotor_ref), rtc(rtc_ref), batterySensor(batterysensor_ref) {
}

int Manager::getPeriod() {
  return (config.getWorkingPeriod() * 1.0) * 4.5;
}

// int Manager::getMaxSteps() {
//   return 1600;
// }

unsigned long Manager::getCurrentTimestamp() {
  uint32_t current_timestamp = rtc.now().unixtime();
  // unsigned long current_timestamp = 1681860900 + (millis() / 1000);
  // unsigned long current_timestamp = 1681857334 + (millis() / 1000);
  // Serial.print("Currenttimestamp: ");
  // Serial.println(current_timestamp);
  // Serial.print("Current timestamp: ");
  // Serial.println(current_timestamp);
  return current_timestamp;
}

bool Manager::isProgramInRunningMode() {
  // Serial.print("Manager::isProgramInRunningMode(): ");
  bool result = config.getWorkingMode() == RUNNING_MODE;
  // Serial.println(result);
  return result;
}


bool Manager::isProgramInTime() {
  bool result = getCurrentTimestamp() < config.getEndTimestamp();
  // Serial.print("getCurrentTimestamp() < config.getEndTimestamp() = ");
  // Serial.println(result);
  return result;
}

void Manager::verifyRestart() {
  eventLogger.getSyslogAsString();
  Serial.print("Verify restart: ");
  Serial.println(millis());

  if (isProgramInRunningMode()) {
    Serial.println("Restart under running mode");
    eventLogger.logOnRestartEvent(config.getLastTimestamp(), getCurrentTimestamp());
    eventLogger.getSyslogAsString();
  }
}

void Manager::begin() {
  verifyRestart();
  //blowerFan.startPID();
}

void Manager::loop(unsigned long currentTime) {

  if (isProgramInRunningMode()) {
    if (isProgramInTime()) {

      if ((currentTime - last_time_saved)>SAVE_TIME_PERIOD){
        config.setLastTimestamp(rtc.now().unixtime());
        last_time_saved = currentTime;
        

      }


      blowerFan.pidLoop(currentTime);

      if (getCurrentTimestamp() >= (config.getLastStepTimestamp() + getPeriod())) {
        Serial.print("Current timestamp: ");
        Serial.println(getCurrentTimestamp());
        Serial.print("Last step timestamp");
        Serial.println(config.getLastStepTimestamp());
        Serial.print("Period: ");
        Serial.println(getPeriod());
        drumMotor.step();
        config.setLastStepTimestamp(getCurrentTimestamp());
      }

    } else {
      Serial.println("false isProgramInTime() stopping mode on");
      // config.setWorkingMode(STOPPED_MODE);
      endProgram();
      blowerFan.finishPID();
    }
  }
}


void Manager::startProgram() {
  eventLogger.resetEventLoggerFile();
  eventLogger.logStartTime(rtc.now().unixtime());
  config.setStartTimestamp(rtc.now().unixtime());
  config.setEndTimestamp(rtc.now().unixtime()+ ((uint32_t) (config.getWorkingPeriod()*60*60)));
  config.setLastTimestamp(rtc.now().unixtime());
  config.setWorkingMode(RUNNING_MODE);

  // set working mode on running
  // set init date
  // clear syslog
  // set startProgram in file
}

void Manager::endProgram() {
  eventLogger.logEndTime(rtc.now().unixtime());
  config.setWorkingMode(IDLE_MODE);
  // set working mode on idle
  // set endprogram in file
}

void Manager::stopProgram() {
  // set working mode on stopped
  //
  config.setWorkingMode(STOPPED_MODE);
  eventLogger.resetEventLoggerFile();
}




uint8_t Manager::getProgramWorkingMode() {
  return config.getWorkingMode();
}


bool Manager::setProgramWorkingMode(uint8_t workingMode) {
  uint8_t actualWorkingMode = config.getWorkingMode();
  if (actualWorkingMode != workingMode) {
    if (workingMode == STOPPED_MODE) {
      if (actualWorkingMode == IDLE_MODE) {
        // config.setWorkingMode(workingMode);
        Serial.println("Stopping program...");
        stopProgram();
        Serial.println("Program stopped...");
        return true;
      }
    }

    if (workingMode == RUNNING_MODE) {
      if (actualWorkingMode == STOPPED_MODE) {
        // config.setWorkingMode(workingMode);
        Serial.println("Running program...");
        startProgram();
        Serial.println("Program set to running...");

        return true;
      }
    }

    if (workingMode == IDLE_MODE) {
      if (actualWorkingMode == RUNNING_MODE) {
        // config.setWorkingMode(workingMode);
        Serial.println("Terminating program...");
        endProgram();
        Serial.println("Program terminated...");

        return true;
      }
    }
  }
  return false;
}




uint8_t Manager::getWorkingPeriod() {
  return config.getWorkingPeriod();
}

bool Manager::setWorkingPeriod(uint8_t workingPeriod) {
  uint8_t actualWorkingPeriod = config.getWorkingPeriod();
  if (actualWorkingPeriod != workingPeriod) {
    config.setWorkingPeriod(workingPeriod);
    return true;
  }
  return false;
}


float Manager::getFlowValue() {
  return blowerFan.flowSensor.read();
}


float Manager::getFlowSetpoint() {
  return config.getFlowSetpoint();
}

bool Manager::setFlowSetpoint(float setpoint) {
  config.setFlowSetpoint(setpoint);
  if (config.getFlowSetpoint() == setpoint) {
    return true;
  }
  return false;
}

uint32_t Manager::getDateTimestamp() {
  return rtc.now().unixtime();
}

void Manager::setDateTimestamp(uint32_t timestamp) {
  rtc.adjust(DateTime(timestamp));
  Serial.print("Setting time ");
  Serial.println(timestamp);
}

unsigned long Manager::getStartProgramDateTimestamp() {
  return config.getStartTimestamp();
}

void Manager::setStartProgramDateTimestamp(unsigned long timestamp) {
  config.setStartTimestamp(timestamp);
}



float Manager::getVoltageValue() {
  return batterySensor.read();
}


String Manager::getDeviceName() {
  return config.getDeviceName();
}
