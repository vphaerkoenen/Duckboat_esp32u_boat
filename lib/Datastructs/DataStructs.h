#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <Arduino.h>

struct BoatDataStruct
{
  uint8_t batteryStateBatteryLeft = 0;
  uint8_t batteryStateBatteryRight = 0;
  bool booleanEmergencyStop = false;
};

struct MotorControlStruct
{
  uint16_t leftMotorSignal;
  uint16_t rightMotorSignal;
};

struct ControlsUpdateStruct
{
  unsigned long clock_ControlDataUpdated = 0;
};

extern BoatDataStruct BoatData_Struct;
extern MotorControlStruct MotorControl_Struct;
extern ControlsUpdateStruct ControlsUpdate_Struct;
#endif
