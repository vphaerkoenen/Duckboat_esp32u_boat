#ifndef MEASURE_H
#define MEASURE_H
#include <Arduino.h>

class Measure
{
public:
        Measure() {};
        byte measureBoatBatteryLeftVoltage();
        byte measureBoatBatteryRightVoltage();
        void initMeasurePin();
        const int batteryIsChargedAdcvalue = 3475; // charged=voltagedivider gives 2800mv / espmax_millivolts 3300 * 12 bit 4095 = 3474,5
        const int batteryIsEmptyAdcvalue = 2482;   // empty=voltagedivider gives 2000mv / espmax_millivolts 3300 * 12 bit 4095 = 2482
        const int timeToRechargeAdcvalue = 2817;   // timetorecharge=voltagedivider gives 2270mv / espmax_millivolts 3300 * 12 bit 4095 = 2817
        const int timeToRechargePercent = 33;      // calculated using map(timeToRechargeAdcvalue, batteryIsEmptyAdcvalue, batteryIsChargedAdcvalue, 0, 100)
};
#endif