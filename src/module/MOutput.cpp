#include "MOutput.h"


namespace eft {

MOutput::MOutput(int pin, bool reverse) {

        this->pin = pin;
        this->reverse = reverse;

};

MOutput::~MOutput() {

};

void MOutput::setup(String nameAP, String nameFX, Config *config) {
        this->nameFX = nameFX;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, reverse ? HIGH : LOW);


};

String MOutput::commType() {
        return "MOutput";
};

void MOutput::commFx(JsonObject *o, JsonArray *ja) {


        (*o)["description"] = "Switch on or off an output";

        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX;
                x["desc"] = "Switch the output on or off";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "enable", "boolean", "Switch the output on or off");
        }


};


void MOutput::commWebServer(ESP8266WebServer *webServer, MQTT *mqtt) {

        this->webServer = webServer;
        this->mqtt = mqtt;

        webServer->on("/api/"+nameFX+"", HTTP_GET, [&] () {
                        this->servOnOff();
                });

        mqtt->subscribe(nameFX, "", [&] (String mess) {
                        this->mqttOnOff(mess);
                });


};

void MOutput::mqttOnOff(String mess) {
        bool enable = Config::isTrue(mess, false);
        switchOnOff(enable);
}

void MOutput::switchOnOff(bool enable) {
        if (enable)
                digitalWrite(pin, reverse ? LOW : HIGH);
        else
                digitalWrite(pin, reverse ? HIGH : LOW);

        mqtt->publish(nameFX,"status", enable ? "1" : "0");
}

void MOutput::servOnOff() {

        bool enable = Config::isTrue(webServer->arg("enable"), false);
        switchOnOff(enable);
        ServerUtil::sendSuccess(webServer);
}

void MOutput::loop() {



};



}
