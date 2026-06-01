//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// OPERATION DUCK BOAT - BOAT CODE /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Author: Velipekka Härkönen 2026
*/
#include <Arduino.h>
#include "boat_espnow.h"
#include "measure.h"
#include "DataStructs.h"
#include "Addresses.h"
// #include "Addresses.example.h"   // use this for setting MAC addresses (remember to change the file name to Addresses.h after editing)
BoatEspnow boatEspnow;
Measure measure;
BoatDataStruct BoatData_Struct;
MotorControlStruct MotorControl_Struct;
ControlsUpdateStruct ControlsUpdate_Struct;

unsigned long clockStart;
unsigned long clockCurrent;
int measureInterval = 3000;        // interval for measuring battery voltage and sending data to controller in ms, default is 3 seconds
int controlInstructionsInterruptLimit = 100; // if no control signal in time limit -> emergency stop is activated

void setup()
{
  Serial.begin(115200);
  delay(1000);
  // espnow.findOwnAddress(); //   use this for finding own MAC-address (prints in serial monitor)
  boatEspnow.initializeWireless();
  boatEspnow.registerPeer(MAC_CONTROLLER);
  boatEspnow.initMotorControlPins();
  measure.initMeasurePin();
  clockStart = millis();
}

void loop()
{
  clockCurrent = millis();
  if (clockCurrent - clockStart > measureInterval) // time to measure battery voltage and send data to controller
  {
    BoatData_Struct.batteryStateBatteryLeft = measure.measureBoatBatteryLeftVoltage();
    BoatData_Struct.batteryStateBatteryRight = measure.measureBoatBatteryRightVoltage();
    boatEspnow.sendData(MAC_CONTROLLER, BoatData_Struct);
    clockStart = clockCurrent; // update clockStart for next measure
  }

  if (clockCurrent - ControlsUpdate_Struct.clock_ControlDataUpdated > controlInstructionsInterruptLimit) // if no control data received within the limit, activate emergency stop
  {
    if (!BoatData_Struct.booleanEmergencyStop) // stop the motors if emergency stop is not already active
    {
      boatEspnow.emergencyStop();
      BoatData_Struct.booleanEmergencyStop = true; // Update the boat data struct to indicate emergency stop
    }
  }
  else
  {
    BoatData_Struct.booleanEmergencyStop = false; // Reset emergency stop flag if control data is received within the limit
  }
}
