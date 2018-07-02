
#include "Config.h"

namespace eft {

Config::Config() {
        this->filename = filename;
}

Config::~Config() {

}

void Config::setup(String nameAP, String nameFX, String filename, FileServ* fileServ) {
        this->nameFX = nameFX;
        this->filename = filename;
        this->fileServ = fileServ;
        this->loadConfig();
}


String Config::getString(String key) {

        String m = map[key];
        if (m == NULL || m.length() == 0) {
                return "";
        }
        return m;
}

int Config::getInt(String key, int def) {
        String m = map[key];
        if (m == NULL || m.length() == 0) {
                return def;
        }
        return m.toInt();
}

float Config::getFloat(String key, float def) {
        String m = map[key];
        if (m == NULL || m.length() == 0) {
                return (float)def;
        }
        return m.toFloat();
}


bool Config::getBool(String key, bool def) {
        String m = map[key];
        return Config::isTrue(m);
}

bool Config::isTrue(String m, bool def) {
        if (m == NULL || m.length() == 0) {
                return def;
        }

        char c = m.charAt(0);

        if (c == '1' || c == 't' || c == 'T' || c == 'y' || c == 'Y')
                return true;

        if (c == '0' || c == 'f' || c == 'F' || c == 'n' || c == 'N')
                return false;

        return def;
}

void Config::setString(String key, String value) {
        map[key] = value;
}

void Config::setInt(String key, int value) {
        map[key] = String(value);
}

void Config::setFloat(String key, float value) {
        map[key] = String(value);
}

void Config::setBool(String key, bool value) {
        if (value)
                map[key] = "1";
        else
                map[key] = "0";
}

void Config::commWebServer(ESP8266WebServer *webServer) {

        this->webServer = webServer;

        webServer->on("/api/"+nameFX, HTTP_GET, [&] () {
                        this->servConfigAll();
                });

}

void Config::servConfigAll() {

        int currentArgCount = webServer->args();

        if (currentArgCount>0) {
                for (int i = 0; i < currentArgCount; ++i) {
                        String k = webServer->argName(i);
                        String v = webServer->arg(i);
                        if (v == NULL || v.length() == 0) {
                                map[k] = "";
                        }
                        else {
                                map[k] = v;
                        }
                }
        }

        // Serv values in config

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        root["path"] = this->filename;
        JsonObject& t = root.createNestedObject("config");

        for (std::map<String,String>::iterator it=map.begin(); it!=map.end(); ++it) {
                t[it->first] = it->second;
        }

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

        if (currentArgCount>0) {
                saveConfig();
        }
}

void Config::servConfig(String k0, String k1, String k2, String k3, String k4, String k5, String k6, String k7, String k8, String k9) {


        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonObject& t = root.createNestedObject("config");

        if (k0!=NULL && k0.length() > 0)
                t[k0] = map[k0];
        if (k1!=NULL && k1.length() > 0)
                t[k1] = map[k1];
        if (k2!=NULL && k2.length() > 0)
                t[k2] = map[k2];
        if (k3!=NULL && k3.length() > 0)
                t[k3] = map[k3];
        if (k4!=NULL && k4.length() > 0)
                t[k4] = map[k4];
        if (k5!=NULL && k5.length() > 0)
                t[k5] = map[k5];
        if (k6!=NULL && k6.length() > 0)
                t[k6] = map[k6];
        if (k7!=NULL && k7.length() > 0)
                t[k7] = map[k7];
        if (k8!=NULL && k8.length() > 0)
                t[k8] = map[k8];
        if (k9!=NULL && k9.length() > 0)
                t[k9] = map[k9];

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);


}

/**
 * Save keystore
 */
bool Config::loadConfig() {

        File f = fileServ->openR(this->filename);
        if (f) {
                String key = "";
                String value = "";
                bool cont;
                char c;

                while(f.available()>0) {

                        key = "";
                        value = "";

                        if (f.available() > 0) {
                                cont = true;
                                while (f.available()>0 && cont) {
                                        c = (char)f.read();
                                        if (c == 0) {
                                                cont = false;
                                        }
                                        else {
                                                key+=c;
                                        }
                                }
                        }


                        if (f.available() > 0) {
                                cont = true;
                                while (f.available()>0 && cont) {
                                        c = (char)f.read();
                                        if (c == 0) {
                                                cont = false;
                                        }
                                        else {
                                                value+=c;
                                        }
                                }
                        }

                        if (value.length()>0) {
                                map[key] = value;
                        }
                }
                f.close();
                return true;
        }
        else {
                Serial.print("KeyStore: File not found: ");
                Serial.println(this->filename);
                return false;
        }
}

/**
 * Save keystore
 */
bool Config::saveConfig() {

        File f = fileServ->openW(this->filename);
        if (!f) {
                Serial.println("KeyStore: Failed to open config file for writing: "+this->filename);
                return false;
        }

        for (std::map<String,String>::iterator it=map.begin(); it!=map.end(); ++it) {
                f.print(it->first);
                f.write(0);
                f.print(it->second);
                f.write(0);
        }

        f.close();

        return true;
}


}
