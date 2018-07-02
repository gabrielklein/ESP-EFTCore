//
// This class is used for "notifications" on a RGB PIN
//
#ifndef EFT_HUB_H
#define EFT_HUB_H

#include <Arduino.h>

#include "FileServ.h"
#include "Config.h"
#include "../comm/IComm.h"
#include "../comm/ServerUtil.h"
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include "../setup/ISetupNotif.h"
#include "../comm/WebServer.h"
#include "../comm/MQTT.h"
#include <list>

#define EFT_MAX_MODULE 50

namespace eft {

class Hub {

public:
Hub(String name);
~Hub();
void setup(ISetupNotif *iSetupNotif = NULL);
void add(String name, IComm *r);
void loop();
void servModuleList();
void servResetAP();
void servFiles();
void commFx(JsonObject *o, JsonArray *ja);

private:
FileServ fileServ;
Config config;
WebServer webServer;
IComm *modules[EFT_MAX_MODULE];
String modulesName[EFT_MAX_MODULE];
int modulesCount = 0;
MQTT mqtt;
String nameAP;

};

}

#endif
