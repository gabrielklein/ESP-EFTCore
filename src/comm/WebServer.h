#ifndef EFT_WEBSERVER_H
#define EFT_WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "ServerUtil.h"

namespace eft {

/**
 * Webserver abstraction
 */
class WebServer
{
public:
WebServer();
~WebServer();

/**
 * Setup the webserver part
 */
void setup(String nameAP, String nameFX);

/**
 * Main loop
 */
void loop();

ESP8266WebServer ws;


private:
/**
 * Do a scan of the wireless network
 */
void apiScan();
/**
 * Do a reboot of the module
 */
void apiReboot();
String nameAP;

};

}

#endif
