//
// This class is used to get current time.
//
#ifndef EFT_MINTERRUPT_H
#define EFT_MINTERRUPT_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include "../comm/IComm.h"
#include "../comm/MQTT.h"
#include "../comm/ServerUtil.h"

namespace eft {

#define EFT_MINTERRUPT_BIN 6


typedef std::function<void (int countRising, int countFalling, int countTotalRising, int countTotalFalling)> EFTInterruptCallback;

struct mInterruptSettings {

        EFTInterruptCallback intCallback = NULL;

        bool callbackInLoop = true;

        // We notify each action. Notification is done in the main loop
        bool mqttNotifyEachActions = false;

        // We notify "stats". Stats are the number of interrupts per periode of time
        bool mqttNotifyStats = false;

        // Should we notify falling
        // Please note that statistics count the total of falling if enabled + rising if enabled
        bool notifyFalling = true;

        // Should we notify "rising" (low to high)
        // Please note that statistics count the total of falling if enabled + rising if enabled
        bool notifyRising = false;

        // Multiplier
        float scale = 1;

        // Units
        String unit = "";
};

class MInterruptStat {

public:
MInterruptStat(unsigned long durationMS);
~MInterruptStat();
void servStats(JsonObject *o, float scale, String unit);
int sum = 0;
float freq = 0;
void add(unsigned long time, int deltaR, int deltaF);
int added=0;

private:
unsigned long durationMS;
unsigned long durationPerBin;
int count[EFT_MINTERRUPT_BIN];
unsigned long time[EFT_MINTERRUPT_BIN];
int currentBin = 0;
unsigned long prevTimeAdd = -1;
int sumTime = 0;
int sumCount = 0;
int sumOtherBin = 0;
unsigned long sumTimeOtherBin = 0;
};


class MInterrupt : public IComm {

public:
MInterrupt(int pin, mInterruptSettings settings);
~MInterrupt();
void setup(String nameAP, String nameFX, Config *config);
String commType();
void commFx(JsonObject *o, JsonArray *ja);
void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
void loop();
void interrupt(int mode);
void servStats();
void servConfig();

MInterruptStat stat1Second;
MInterruptStat stat15Seconds;
MInterruptStat stat1Minute;
MInterruptStat stat5Minutes;
MInterruptStat stat1Hour;
MInterruptStat stat24Hours;

private:
ESP8266WebServer *webServer;
MQTT *mqtt;
String nameFX;
Config *config;
int countInterruptR = 0;
int countInterruptPrevR = 0;
int countInterruptF = 0;
int countInterruptPrevF = 0;
bool nofifyInterrupt = false;
int pin;
mInterruptSettings settings;

unsigned long timeLastUpdate = 0;
int updateEveryS = 15;

};

}

#endif
