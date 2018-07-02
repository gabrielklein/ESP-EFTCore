#include "MInterrupt.h"

namespace eft {

// Need to make it pseudo object oriented friendly
// -----------------

  #ifndef EFT_FX_INTERRUPT_DEFINE
  #define EFT_FX_INTERRUPT_DEFINE

int eftFxInterruptCount = 0;

MInterrupt* eftFxInterruptClass[10];

void eftFxInterrupt(int id, int mode) {
        eftFxInterruptClass[id]->interrupt(mode);
};

void eftFxInterrupt0R() {
        eftFxInterrupt(0, RISING);
};
void eftFxInterrupt1R() {
        eftFxInterrupt(1, RISING);
};
void eftFxInterrupt2R() {
        eftFxInterrupt(2, RISING);
};
void eftFxInterrupt3R() {
        eftFxInterrupt(3, RISING);
};
void eftFxInterrupt4R() {
        eftFxInterrupt(4, RISING);
};
void eftFxInterrupt5R() {
        eftFxInterrupt(5, RISING);
};
void eftFxInterrupt6R() {
        eftFxInterrupt(6, RISING);
};
void eftFxInterrupt7R() {
        eftFxInterrupt(7, RISING);
};
void eftFxInterrupt8R() {
        eftFxInterrupt(8, RISING);
};
void eftFxInterrupt9R() {
        eftFxInterrupt(9, RISING);
};
void eftFxInterrupt0F() {
        eftFxInterrupt(0,FALLING);
};
void eftFxInterrupt1F() {
        eftFxInterrupt(1,FALLING);
};
void eftFxInterrupt2F() {
        eftFxInterrupt(2,FALLING);
};
void eftFxInterrupt3F() {
        eftFxInterrupt(3,FALLING);
};
void eftFxInterrupt4F() {
        eftFxInterrupt(4,FALLING);
};
void eftFxInterrupt5F() {
        eftFxInterrupt(5,FALLING);
};
void eftFxInterrupt6F() {
        eftFxInterrupt(6,FALLING);
};
void eftFxInterrupt7F() {
        eftFxInterrupt(7,FALLING);
};
void eftFxInterrupt8F() {
        eftFxInterrupt(8,FALLING);
};
void eftFxInterrupt9F() {
        eftFxInterrupt(9,FALLING);
};



void eftFxInterruptRegister(MInterrupt *m, int pin, struct mInterruptSettings *settings) {

        eftFxInterruptClass[eftFxInterruptCount] = m;

        switch(eftFxInterruptCount) {
        case 0:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt0F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt0R, RISING);
                break;
        case 1:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt1F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt1R, RISING);
                break;
        case 2:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt2F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt2R, RISING);
                break;                break;
        case 3:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt3F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt3R, RISING);
                break;                break;
        case 4:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt4F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt4R, RISING);
                break;                break;
        case 5:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt5F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt5R, RISING);
                break;                break;
        case 6:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt6F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt6R, RISING);
                break;                break;
        case 7:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt7F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt7R, RISING);
                break;                break;
        case 8:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt8F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt8R, RISING);
                break;                break;
        case 9:
                if (settings->notifyFalling)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt9F, FALLING);
                if (settings->notifyRising)
                        attachInterrupt(digitalPinToInterrupt(pin),  eftFxInterrupt9R, RISING);
                break;                break;
        default:
                Serial.println("You can only register a maximum of 10 interrupts through these class.");
        }

        eftFxInterruptCount++;

}

#endif

// -----------------

MInterruptStat::MInterruptStat(unsigned long durationMS) {
        this->durationMS = durationMS;
        this->durationPerBin = durationMS / (float)EFT_MINTERRUPT_BIN;
};

MInterruptStat::~MInterruptStat() {

};

void MInterruptStat::servStats(JsonObject *o, float scale, String unit) {
        (*o)["value"] = freq*scale;
        (*o)["freq"] = unit;
        (*o)["duration"] = durationMS/1000;
        (*o)["sum"] = sum;
        (*o)["freq"] = freq;
        (*o)["realDuration"] = ((float)sumTime)/1000.0;
        (*o)["realCount"] = sumCount;

        //for (int i=0; i<EFT_MINTERRUPT_BIN; i++) {
        //        (*o)["time"+String(i)] = this->time[i];
        //        (*o)["count"+String(i)] = this->count[i];
        //}
}

void MInterruptStat::add(unsigned long time, int deltaR, int deltaF) {

        int delta = deltaR + deltaF;

        added+=delta;

        // Init the system or "reset" (if we overlap, every ~50 days)
        if (this->time[0] == 0 || time < prevTimeAdd) {
                currentBin = 0;
                for (int i=0; i<EFT_MINTERRUPT_BIN; i++) {
                        count[i] = 0;
                        this->time[i] = 0;
                }
                prevTimeAdd = time;
                this->time[0] = time;
                //Serial.println("RESET");
                return;
        }

        prevTimeAdd = time;

        unsigned long currentST = this->time[currentBin];
        unsigned long dtime = time - currentST;

        bool needRecalculate = false;
        while (dtime>durationPerBin) {
                currentBin = (currentBin+1)%EFT_MINTERRUPT_BIN;
                count[currentBin] = 0;
                this->time[currentBin] = currentST+durationPerBin;
                dtime -= durationPerBin;
                needRecalculate = true;
        };

        // Add delta to the bin
        if (delta>0) {
                count[currentBin] += delta;
        }


        // We need to recalculate the sum of the bins
        if (needRecalculate == true) {
                sumOtherBin = 0;
                sumTimeOtherBin = 0;
                for (int i=0; i<EFT_MINTERRUPT_BIN; i++) {
                        unsigned long tt1 = this->time[i];
                        unsigned long tt2 = this->time[(i+1)%EFT_MINTERRUPT_BIN];
                        if (i!=currentBin && tt2>tt1 && tt1!=0) {
                                unsigned long dt = tt2-tt1;
                                sumOtherBin += count[i];
                                sumTimeOtherBin += dt;
                        }
                }
                //needRecalculate = false;
        }


        uint64_t t1 = sumOtherBin;
        t1+=count[currentBin];
        sumCount = t1;
        uint64_t t2 = sumTimeOtherBin;
        unsigned long dt = time - this->time[currentBin];
        t2 += dt;
        uint64_t t3 = durationMS;
        sumTime = t2;

        // Normalize on the total time
        t1*=t3;
        if (t2>0)
                t1/=t2;

        sum = (int)t1;
        if (durationMS>0)
                freq = 1000.0*((float)sum) / ((float)durationMS);
        else
                freq = 0;
}

// -----------------

MInterrupt::MInterrupt(int pin, mInterruptSettings settings) :
        stat1Second(1000),
        stat15Seconds(15*1000),
        stat1Minute(60*1000),
        stat5Minutes(5*60*1000),
        stat1Hour(60*60*1000),
        stat24Hours(24*60*60*1000)

{
        this->pin = pin;
        this->settings = settings;

};

MInterrupt::~MInterrupt() {
};

void MInterrupt::commFx(JsonObject *o, JsonArray *ja) {

        (*o)["description"] = "Support for DHTxx temperature and humidity modules (DS18B20, DS1822, DS1820, MAX31820)";
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX;
                x["desc"] = "View statistics of the number of time the module was interupted the last x period of time";
        }

        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX+"/config";
                x["desc"] = "Configure the interruptions";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "updateEveryS", "number", "Upload every x seconds");

        }



};

void MInterrupt::setup(String nameAP, String nameFX, Config *config) {
        this->nameFX = nameFX;
        this->config = config;

        pinMode(pin, INPUT);

        // Enable interrupts (in case they were previously disabled)
        interrupts();

        //auto fx = [&]() -> void {
        //                  this->interrupt();
        //          };

        eftFxInterruptRegister(this, pin, &settings);

        updateEveryS = config->getInt(nameFX + ".updateEveryMS", 15);
        if (updateEveryS<1) {
                updateEveryS = 1;
        }

};

String MInterrupt::commType() {
        return "MInterrupt";
};

void MInterrupt::interrupt(int mode) {

        // We are in the interrupt call of the code.
        if (mode == RISING)
                countInterruptR++;
        else if (mode == FALLING)
                countInterruptF++;

        nofifyInterrupt = true;

        if (!settings.callbackInLoop && settings.intCallback!=NULL) {
                settings.intCallback(mode == RISING ? 1 : 0, mode == FALLING ? 1 : 0, countInterruptR, countInterruptF);
        }

}

void MInterrupt::commWebServer(ESP8266WebServer *webServer, MQTT *mqtt) {

        this->webServer = webServer;

        webServer->on("/api/"+nameFX+"", HTTP_GET, [&] () {
                        this->servStats();
                });

        webServer->on("/api/"+nameFX+"/config", HTTP_GET, [&] () {
                        this->servConfig();
                });


        this->mqtt = mqtt;
};

/**
 * Get list of supported timezones.
 */
void MInterrupt::servConfig() {

        bool doSave = false;

        if (webServer->hasArg("updateEveryS")) {

                String v = webServer->arg("updateEveryS");
                updateEveryS = v.toInt();
                if (updateEveryS<2) {
                        updateEveryS = 2;
                }

                config->setInt(nameFX + ".updateEveryS", updateEveryS);
                doSave = true;
        }

        if (doSave) {
                config->saveConfig();
        }

        config->servConfig(nameFX + ".updateEveryS");

};


void MInterrupt::servStats() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        //JsonObject& t = root.createNestedObject("modules");
        JsonArray& t = root.createNestedArray("interrupt");

        stat1Second.servStats(&t.createNestedObject(), settings.scale, settings.unit);
        stat15Seconds.servStats(&t.createNestedObject(), settings.scale, settings.unit);
        stat1Minute.servStats(&t.createNestedObject(), settings.scale, settings.unit);
        stat5Minutes.servStats(&t.createNestedObject(), settings.scale, settings.unit);
        stat1Hour.servStats(&t.createNestedObject(), settings.scale, settings.unit);
        stat24Hours.servStats(&t.createNestedObject(), settings.scale, settings.unit);

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

}


void MInterrupt::loop() {

        // Only call once out of 10 if no update (avoid using too much cpu)
        //if (countInterruptPrev!=countInterrupt || countInterruptNb0++>10) {
        // I would love to use a semaphone here :)
        int deltaR = countInterruptR-countInterruptPrevR;
        int deltaF = countInterruptF-countInterruptPrevF;
        countInterruptPrevR = countInterruptR;
        countInterruptPrevF = countInterruptF;
        //countInterruptNb0 = 0;
        // --
        unsigned long t = millis();
        stat1Second.add(t, deltaR, deltaF);
        stat15Seconds.add(t, deltaR, deltaF);

        stat1Minute.add(t, deltaR, deltaF);
        stat5Minutes.add(t, deltaR, deltaF);
        stat1Hour.add(t, deltaR, deltaF);
        stat24Hours.add(t, deltaR, deltaF);

        //}
        //Serial.print(stat1Second.sum);
        //erial.print("  ");
        //Serial.print(stat1Second.freq);
        //Serial.print("  ");
        //Serial.print(stat1Minute.sum);
        //Serial.print("  ");
        //Serial.print(stat1Minute.freq);
        //Serial.print("  ");
        //Serial.println();
        if (settings.mqttNotifyEachActions && (deltaR>0 || deltaF>0)) {
                this->mqtt->publish(nameFX, "",
                                    String(deltaR)+","+ String(deltaF)+","+ String(stat15Seconds.freq)+","+ String(stat1Minute.freq)+","+  String(stat5Minutes.freq)+","+ String(stat1Hour.freq)+","+ String(stat24Hours.freq));
        }


        if (nofifyInterrupt) {
                nofifyInterrupt = false;
                if (settings.callbackInLoop && settings.intCallback!=NULL) {
                        settings.intCallback(deltaR, deltaF, countInterruptR, countInterruptF);
                }
        }

        // Sleep
        long d = millis()-timeLastUpdate;
        if (d>=0 && d<updateEveryS*1000) {
                return;
        }

        // We should have the new temperature after 1 secondo
        timeLastUpdate = millis();

        if (settings.mqttNotifyStats) {
                this->mqtt->publish(nameFX, "15s", String(stat15Seconds.freq)+","+ String(stat15Seconds.sum)+","+  String(stat15Seconds.freq*settings.scale)+","+  settings.unit);
                this->mqtt->publish(nameFX, "1", String(stat1Minute.freq)+","+ String(stat1Minute.sum)+","+  String(stat1Minute.freq*settings.scale)+","+  settings.unit);
                this->mqtt->publish(nameFX, "5", String(stat5Minutes.freq)+","+ String(stat5Minutes.sum)+","+  String(stat5Minutes.freq*settings.scale)+","+  settings.unit);
                this->mqtt->publish(nameFX, "60", String(stat1Hour.freq)+","+ String(stat1Hour.sum)+","+  String(stat1Hour.freq*settings.scale)+","+  settings.unit);
        }



};



}
