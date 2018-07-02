//
// This class is used to get current time.
//
#ifndef EFT_MLEDRGB_H
#define EFT_MLEDRGB_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "../comm/IComm.h"
#include "../comm/ServerUtil.h"

namespace eft {

class MLedRGB : public IComm {

public:
MLedRGB(int pinR,int pinG,int pinB);
~MLedRGB();
void setup(String nameAP, String nameFX, Config *config);
String commType();
void commFx(JsonObject *o, JsonArray *ja);
void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
void loop();
void servRGB();
void mqttRGB(String mess);
void rgb(bool r, bool g, bool b);

private:
ESP8266WebServer *webServer;
MQTT *mqtt;
String nameFX;

int pinR;
int pinG;
int pinB;

int pinStatusR = 0;
int pinStatusG = 0;
int pinStatusB = 0;

};

}

#endif
