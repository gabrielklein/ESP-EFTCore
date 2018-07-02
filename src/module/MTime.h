//
// This class is used to get current time.
//
#ifndef EFT_MTIME_H
#define EFT_MTIME_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <Time.h>
#include <Timezone.h>
#include "../comm/IComm.h"
#include "../comm/ServerUtil.h"
#include "TZTime.h"


namespace eft {

struct localtime {
        word year;
        byte month;
        byte hour;
        byte minute;
        byte second;
        byte weekday;
        time_t localTime;
};

/**
 * Module to deal with timezones and time.
 */
class MTime : public IComm {

public:
MTime();
~MTime();

/**
 * Called by the hub to setup this object
 */
void setup(String nameAP, String nameFX, Config *config);
/**
 * Return the name of this class
 */
String commType();
/**
 * Return the description of this module in a json object
 */
void commFx(JsonObject *o, JsonArray *ja);
/**
 * Register web and mqtt calls.
 */
void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
/**
 * API to serv time
 */
void servTime();
/**
 * API to serv config
 */
void servConfig();
/**
 * API to serv zimeezones
 */
void servTimezone();
/**
 * Called by the hub
 */
void loop();

/**
 * Get the current local time object
 */
localtime getLocalTime();

private:
int tzCurrentIndex = -1;
ESP8266WebServer *webServer;
const char* timeHost = "time.nist.gov";
WiFiUDP ntpUDP;
NTPClient *timeClient;
TZTime tz;
String getFormattedTime(unsigned long rawTime);
int getTimezoneIndex();
Config *config;
String nameFX;
};

}

#endif
