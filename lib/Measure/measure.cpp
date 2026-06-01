#include <Arduino.h>
#include "measure.h"
#define measureVoltagePinBatteryLeft 34
#define measureVoltagePinBatteryRight 35

void Measure::initMeasurePin()
{
    pinMode(measureVoltagePinBatteryLeft, INPUT);
    pinMode(measureVoltagePinBatteryRight, INPUT);
}

byte Measure::measureBoatBatteryLeftVoltage()
{
    int adcValueBatteryLeft = analogRead(measureVoltagePinBatteryLeft);
    if (adcValueBatteryLeft <= batteryIsEmptyAdcvalue)
    {
        adcValueBatteryLeft = batteryIsEmptyAdcvalue;
    }
    else if (adcValueBatteryLeft >= batteryIsChargedAdcvalue)
    {
        adcValueBatteryLeft = batteryIsChargedAdcvalue;
    }

    byte boatVoltagePercentBatteryLeft = map(adcValueBatteryLeft, batteryIsEmptyAdcvalue, batteryIsChargedAdcvalue, 0, 100);
    Serial.print("Boat battery_left voltagepercent:");
    Serial.println(boatVoltagePercentBatteryLeft);
    return boatVoltagePercentBatteryLeft;
}
byte Measure::measureBoatBatteryRightVoltage()
{
    int adcValueBatteryRight = analogRead(measureVoltagePinBatteryRight);

    if (adcValueBatteryRight <= batteryIsEmptyAdcvalue)
    {
        adcValueBatteryRight = batteryIsEmptyAdcvalue;
    }
    else if (adcValueBatteryRight >= batteryIsChargedAdcvalue)
    {
        adcValueBatteryRight = batteryIsChargedAdcvalue;
    }
    byte boatVoltagePercentBatteryRight = map(adcValueBatteryRight, batteryIsEmptyAdcvalue, batteryIsChargedAdcvalue, 0, 100);
    Serial.print("Boat battery_right voltagepercent:");
    Serial.println(boatVoltagePercentBatteryRight);
    return boatVoltagePercentBatteryRight;
}