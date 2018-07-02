#include "MLedRGB.h"


namespace eft {

MLedRGB::MLedRGB(int pinR,int pinG,int pinB) {

        this->pinR = pinR;
        this->pinG = pinG;
        this->pinB = pinB;
};

MLedRGB::~MLedRGB() {

};

void MLedRGB::setup(String nameAP, String nameFX, Config *config) {
        this->nameFX = nameFX;


        pinMode(pinR, OUTPUT);
        pinMode(pinG, OUTPUT);
        pinMode(pinB, OUTPUT);

        digitalWrite(pinR, LOW);
        digitalWrite(pinG, LOW);
        digitalWrite(pinB, LOW);

};

String MLedRGB::commType() {
        return "MLedRGB";
};

void MLedRGB::commFx(JsonObject *o, JsonArray *ja) {

        (*o)["description"] = "RGB led (on/off)";
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX;
                x["desc"] = "Configure a RGB simple led";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "r", "boolean", "Do we switch red on or off");
                CommUtil::addFXParam(&t, "g", "boolean", "Do we switch green on or off");
                CommUtil::addFXParam(&t, "b", "boolean", "Do we switch blue on or off");

        }


};


void MLedRGB::commWebServer(ESP8266WebServer *webServer, MQTT *mqtt) {

        this->webServer = webServer;
        this->mqtt = mqtt;

        webServer->on("/api/"+nameFX+"", HTTP_GET, [&] () {
                        this->servRGB();
                });

        mqtt->subscribe(nameFX, "", [&] (String mess) {
                        this->mqttRGB(mess);
                });


};

void MLedRGB::rgb(bool r, bool g, bool b) {
        digitalWrite(pinR, r ? HIGH : LOW);
        digitalWrite(pinG, g ? HIGH : LOW);
        digitalWrite(pinB, b ? HIGH : LOW);

        pinStatusR = r;
        pinStatusG = g;
        pinStatusB = b;

        String s = pinStatusR ? "1" : "0";
        s = s + (pinStatusG ? "1" : "0");
        s = s + (pinStatusB ? "1" : "0");
        mqtt->publish(nameFX,"status", s);

}

void MLedRGB::mqttRGB(String mess) {

        mess.trim();
        mess.toLowerCase();

        if (mess.length() == 0) {
                rgb(false, false, false);
                return;
        }

        char c = mess.charAt(0);

        // red
        if (c == 'r') {
                rgb(true, false, false);
                return;
        }

        // green
        if (c == 'g') {
                rgb(false, true, false);
                return;
        }

        // blue
        if (c == 'b') {
                rgb(false, false, true);
                return;
        }

        // 101 or 110 or 001
        // rgb as {red enambed, green eabled, blue enabled}
        if (mess.length() == 3) {
                char c1 = mess.charAt(0);
                char c2 = mess.charAt(1);
                char c3 = mess.charAt(2);
                rgb(c1=='1', c2=='1', c3=='1');
                return;
        }

        // A number between 0 and 7
        if (c >= '0' && c <= '7') {
                int n = c-'0';
                rgb(n&0b100, n&0b010, n&0b001);
                return;
        }

        rgb(false, false, false);
}

void MLedRGB::servRGB() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();

        String r = webServer->arg("r");
        String g = webServer->arg("g");
        String b = webServer->arg("b");

        bool update = false;
        if (webServer->hasArg("r")) {
                pinStatusR = Config::isTrue(webServer->arg("r"), pinStatusR);
                update = true;
        }
        if (webServer->hasArg("g")) {
                pinStatusG = Config::isTrue(webServer->arg("g"), pinStatusG);
        }
        if (webServer->hasArg("b")) {
                pinStatusB = Config::isTrue(webServer->arg("b"), pinStatusB);
                update = true;
        }

        if (update) {
                rgb(pinStatusR, pinStatusG, pinStatusB);
        }

        // quick hack to support the "rgb=xxx"
        // Pin status is updated!
        String rgb = webServer->arg("rgb");
        if (rgb.length()>0) {
                mqttRGB(rgb);
        }

        root["r"] = pinStatusR;
        root["g"] = pinStatusG;
        root["b"] = pinStatusB;

        String s;
        root.printTo(s);
        webServer->send(200, "application/json", s);

};

void MLedRGB::loop() {



};



}
