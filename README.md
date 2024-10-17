# VolumetricSporeTrap

# SETUP

![image](https://user-images.githubusercontent.com/52977088/235458783-895cbc4e-fe83-4446-9df0-ea4eb7c56aab.png)

# PREFERENCES
```cpp
#include "Preferences.h"

void setup() {
  // put your setup code here, to run once:
  Preferences preferences;
  preferences.begin("config", false);
  preferences.putString("id", "00001");
  preferences.putString("name", "SPORETRAP_X");
  preferences.putUChar("w_mode", 1);
  preferences.putUChar("w_period", 7);
  preferences.putULong("start_time", 0);
  preferences.putULong("end_time", 0);
  preferences.putULong("last_time", 0);
  preferences.putULong("last_step", 0);
  preferences.putUInt("flow_setpoint", 10.0);
  preferences.end();
}

void loop() {
  // put your main code here, to run repeatedly:
}

```
