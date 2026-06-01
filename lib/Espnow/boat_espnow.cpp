#include "boat_espnow.h"
#include <WiFi.h>
#include <esp_now.h>
#include "DataStructs.h"
#include "Arduino.h"
#include "Servo.h"

#define leftMotorPin 33
#define rightMotorPin 32

Servo leftMotor;
Servo rightMotor;

void BoatEspnow::initMotorControlPins()
{
    leftMotor.attach(leftMotorPin);
    rightMotor.attach(rightMotorPin);
}

void BoatEspnow::findOwnAddress()
{
    uint8_t baseMac[6];

    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    Serial.print("Station MAC: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.printf("%02X:", baseMac[i]);
    }
    Serial.printf("%02X\n", baseMac[5]);
}

void BoatEspnow::initializeWireless()
{

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
}
void BoatEspnow::registerPeer(const uint8_t *controllerMac)
{
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, controllerMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
    }
    else
    {
        Serial.print("Registered peer ");
    }

    esp_now_register_send_cb((esp_now_send_cb_t)BoatEspnow::onDataSent);
    esp_now_register_recv_cb((esp_now_recv_cb_t)BoatEspnow::onDataReceived);
}
void BoatEspnow::sendData(const uint8_t *controllerMac, BoatDataStruct &dataToController)
{

    // Lähetetään data suoraan osoittimen avulla
    esp_err_t result = esp_now_send(controllerMac, (uint8_t *)&dataToController, sizeof(dataToController));

    if (result == ESP_OK)
    {
        Serial.print("Sent with success batteryState values of left: ");
        Serial.print(dataToController.batteryStateBatteryLeft);
        Serial.print(" and right: ");
        Serial.print(dataToController.batteryStateBatteryRight);
        Serial.println(" and emergencyStop value of: ");
        Serial.println(dataToController.booleanEmergencyStop);
    }
    else
    {
        Serial.print("Error sending the data. Error code: ");
        Serial.println(result);
    }
}

void BoatEspnow::onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len)
{
    Serial.println("Controller -> Boat packet received");
    if (len == sizeof(MotorControlStruct))
    {
        memcpy(&MotorControl_Struct, data, sizeof(MotorControl_Struct));
        Serial.print("Received motor control values: Left = ");
        Serial.print(MotorControl_Struct.leftMotorSignal);
        Serial.print(", Right = ");
        Serial.println(MotorControl_Struct.rightMotorSignal);

        leftMotor.writeMicroseconds(MotorControl_Struct.leftMotorSignal);
        rightMotor.writeMicroseconds(MotorControl_Struct.rightMotorSignal);
    }
    else
    {
        Serial.println("Received data of unexpected size");
    }
    ControlsUpdate_Struct.clock_ControlDataUpdated = millis();
    BoatData_Struct.booleanEmergencyStop = false;
}

void BoatEspnow::onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("Boat says: data sent to: ");

    for (int i = 0; i < 6; i++)
    {
        Serial.printf("%02X", mac_addr[i]);
        if (i < 5)
            Serial.print(":");
    }

    if (status == ESP_NOW_SEND_SUCCESS)
    {
        Serial.println(" -> Boat says: Delivery Success");
    }
    else
    {
        Serial.println(" -> Boat says: Delivery Fail");
    }
}
void BoatEspnow::emergencyStop()
{
    leftMotor.writeMicroseconds(1500);           // Stop left motor
    rightMotor.writeMicroseconds(1500);          // Stop right motor
    MotorControl_Struct.leftMotorSignal = 1500;  // Update motor control struct for consistency
    MotorControl_Struct.rightMotorSignal = 1500; // Update motor control struct for consistency
    Serial.println("Weak signal. Motors stopped.");
}
