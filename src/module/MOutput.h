//
// This class is used to get current time.
//
#ifndef EFT_MOUTPUT_H
#define EFT_MOUTPUT_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "../comm/IComm.h"
#include "../comm/ServerUtil.h"

namespace eft {

/**
 * Drive an output pin
 */
class MOutput : public IComm {

public:
/**
 * Initialize this module by giving the pin and if we reverse the on-off functionnalities.
 */
MOutput(int pin, bool reverse = false);
~MOutput();
/**
 * Setup this module
 */
void setup(String nameAP, String nameFX, Config *config);
/**
 * Type of this module
 */
String commType();
/**
 * Describe the module functionnalities
 */
void commFx(JsonObject *o, JsonArray *ja);
/**
 * Register web and mqtt server api
 */
void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
/**
 * When we receive a mqtt signal on the subscribed channel
 */
void mqttOnOff(String mess);
/**
 * Web api when we call the server url
 */
void servOnOff();
/**
 * Main loop to dispatch actions
 */
void loop();
/**
 * Switch this output on and off
 */
void switchOnOff(bool enable);

private:
ESP8266WebServer *webServer;
MQTT *mqtt;
String nameFX;

int pin;
bool reverse;

};

}

#endif
