#ifndef EFT_CONFIG_H
#define EFT_CONFIG_H

#include <Arduino.h>
#include <string>
#include <map>
#include "FileServ.h"
#include "../comm/ServerUtil.h"

namespace eft {

class Config {

public:
Config();
~Config();

void setup(String nameAP, String nameFX, String filename, FileServ* fileServ);

String getString(String key);
int getInt(String key, int def = 0);
float getFloat(String key, float def = 0);
bool getBool(String key, bool def = 0);

void setString(String key, String value);
void setInt(String key, int value);
void setFloat(String key, float value);
void setBool(String key, bool value);

void commWebServer(ESP8266WebServer *webServer);
void servConfigAll();
void servConfig(String k0, String k1="", String k2="", String k3="", String k4="", String k5="", String k6="", String k7="", String k8="", String k9="");

bool loadConfig();
bool saveConfig();

static bool isTrue(String m, bool def = true);

private:
String filename;
std::map<String,String> map;
ESP8266WebServer *webServer;
FileServ* fileServ;
String nameFX;


};



}

#endif
