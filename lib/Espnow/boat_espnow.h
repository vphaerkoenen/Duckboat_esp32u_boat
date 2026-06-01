#ifndef BOATESPNOW_H
#define BOATESPNOW_H
#include "Arduino.h"
#include <WiFi.h>
#include <esp_now.h>
#include "DataStructs.h"
class BoatEspnow
{
public:
    BoatEspnow() {};
    void initMotorControlPins();
    void findOwnAddress();
    void initializeWireless();
    void registerPeer(const uint8_t *controllerMac);
    void sendData(const uint8_t *controllerMac, BoatDataStruct &dataToController);
    void emergencyStop();
    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len);
};
#endif