#include "MTime.h"

namespace eft {

MTime::MTime() {
        this->timeClient = new NTPClient(this->ntpUDP, this->timeHost, 0, 60*60*1000 /*Once an hour*/);
};

MTime::~MTime() {
        delete(this->timeClient);
};

void MTime::setup(String nameAP, String nameFX, Config *config) {
        this->nameFX = nameFX;
        this->config = config;
        timeClient->begin();
};

String MTime::commType() {
        return "MTime";
}

void MTime::commWebServer(ESP8266WebServer *webServer, MQTT *mqtt) {
        this->webServer = webServer;
        webServer->on("/api/"+nameFX+"", HTTP_GET, [&] () {
                        this->servTime();
                });
        webServer->on("/api/"+nameFX+"/config", HTTP_GET, [&] () {
                        this->servConfig();
                });

        webServer->on("/api/"+nameFX+"/timezone", HTTP_GET, [&] () {
                        this->servTimezone();
                });

};

void MTime::commFx(JsonObject *o, JsonArray *ja) {
        (*o)["description"] = "Configure the time service";
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX;
                x["desc"] = "Get the current time";
        }

        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX+"/timezone";
                x["desc"] = "Display supported timezones";
        }


        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX+"/config";
                x["desc"] = "Configure the timezone";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "tz", "string", "Timezone to be displayed");

        }

};


/**
 * Get current time
 */
void MTime::servTime() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();

        JsonObject& utc = root.createNestedObject("utc");

        time_t t = timeClient->getEpochTime();

        root["timeUnix"] = t;

        utc["timezone"] =  "UTC";
        utc["epochTime"] =  timeClient->getEpochTime();
        utc["hms"] =  getFormattedTime(t);
        //utc["date"] =  timeClient->getDay();
        utc["hour"] =  hour(t);
        utc["minute"] =  minute(t);
        utc["second"] =  second(t);
        utc["day"] =  day(t);
        utc["weekday"] =  weekday(t);
        utc["month"] =  month(t);
        utc["year"] =  year(t);


        JsonObject& local = root.createNestedObject("local");
        int index = getTimezoneIndex();
        Timezone *tzz = tz.tz[index];
        String name = tz.name[index];

        TimeChangeRule *tcr;
        time_t t2 = tzz->toLocal(t, &tcr);
        local["timezone"] =  name;
        local["hms"] =  getFormattedTime(t2);
        //local["date"] =  timeClient->getDay();
        local["hour"] =  hour(t2);
        local["minute"] =  minute(t2);
        local["second"] =  second(t2);
        local["day"] =  day(t2);
        local["weekday"] =  weekday(t2);
        local["month"] =  month(t2);
        local["year"] =  year(t2);
        local["tcr"] = tcr->abbrev;


        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

};


localtime MTime::getLocalTime() {

        localtime x;

        int index = getTimezoneIndex();
        Timezone *tzz = tz.tz[index];
        time_t t = timeClient->getEpochTime();
        TimeChangeRule *tcr;
        time_t localTime = tzz->toLocal(t, &tcr);

        x.localTime = localTime;
        x.hour = hour(localTime);
        x.minute = minute(localTime);
        x.second = second(localTime);
        x.year = year(localTime);
        x.month = month(localTime);
        x.weekday =  weekday(localTime);

        return x;
}

/**
 * Get list of supported timezones.
 */
void MTime::servTimezone() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        JsonArray& tzo = root.createNestedArray("tz");

        time_t utc = timeClient->getEpochTime();
        TimeChangeRule *tcr;
        for (int i=0; i<this->tz.tzCount; i++) {
                JsonObject& x = tzo.createNestedObject();
                time_t t = tz.tz[i]->toLocal(utc, &tcr);
                x["id"] = tz.name[i];
                x["label"] = tz.label[i];
                x["tcr"] = tcr->abbrev;

                x["hms"] =  getFormattedTime(t);
                x["hour"] =  hour(t);
                x["minute"] =  minute(t);
                x["second"] =  second(t);
        }

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);
};

/**
 * Get list of supported timezones.
 */
void MTime::servConfig() {

        if (webServer->hasArg("timezone")) {

                String v = webServer->arg("timezone");
                bool success = false;
                for (int i=0; i<this->tz.tzCount; i++) {
                        if (tz.name[i].equals(v)) {
                                success = true;
                        }
                }
                if (!success) {
                        ServerUtil::sendFail(webServer, 10001, "Timezone was not found: "+v);
                        return;
                }

                config->setString(nameFX + ".timezone", v);
                config->saveConfig();
        }

        config->servConfig(nameFX + ".timezone");

};


/**
 * Get current timezone
 */
int MTime::getTimezoneIndex() {

        if (tzCurrentIndex>=0)
                return tzCurrentIndex;

        String tzK = config->getString(nameFX + ".timezone");
        if (tzK.length() == 0) {
                tzCurrentIndex=0;
                return 0;
        }
        for (int i=0; i<this->tz.tzCount; i++) {
                if (tz.name[i].equals(tzK)) {
                        tzCurrentIndex=i;
                        return i;
                }
        }
        // Return default one
        tzCurrentIndex=0;
        return 0;
}

/**
 * Get formatted time
 */
String MTime::getFormattedTime(unsigned long rawTime) {
        unsigned long hours = (rawTime % 86400L) / 3600;
        String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

        unsigned long minutes = (rawTime % 3600) / 60;
        String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

        unsigned long seconds = rawTime % 60;
        String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

        return hoursStr + ":" + minuteStr + ":" + secondStr;
}

void MTime::loop() {
        timeClient->update();
};


}
