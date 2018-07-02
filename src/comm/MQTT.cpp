#include "MQTT.h"

namespace eft {

MQTT::MQTT() {
        timeLastReconnect = millis();
};
MQTT::~MQTT() {
        if (ethClient!=NULL)
                delete(ethClient);
};

void MQTT::setup(String nameAP, String nameFX, eft::Config *config) {

        this->nameAP = nameAP;
        this->nameFX = nameFX;
        this->config = config;

        ethClient = new WiFiClient();
        client.setClient(*ethClient);

        serverConfig = config->getString(nameFX + ".server");
        if (serverConfig.length() == 0)
                serverConfig = EFT_DEFAULT_MQTT;

        serverBaseTopic = config->getString(nameFX + ".basetopic");

        client.setServer(serverConfig.c_str(), config->getInt(nameFX + ".port"));

        client.setCallback([&] (char* topic, byte* payload, unsigned int length) {
                        this->callback(topic, payload, length);
                });

        timeLastReconnect = millis();
};

void MQTT::callback(char* topic, byte* payload, unsigned int length) {

        payload[length] = 0;
        char* st = (char*)payload;
        String s = String(st);

        THandlerFunction1p h = subMap[topic];
        if (h == NULL) {
                Serial.print("MQTT (no handler) [");
                Serial.print(topic);
                Serial.print("] ");
                Serial.print(s);
                Serial.println();
        }
        else {
                h(s);
        }

}

String MQTT::commType() {
        return "mqtt";
};

void MQTT::commWebServer(ESP8266WebServer *webServer) {
        this->webServer = webServer;
        webServer->on("/api/"+nameFX+"/config", HTTP_GET, [&] () {
                        this->servConfig();
                });


};

void MQTT::commFx(JsonObject *o, JsonArray *ja) {
        (*o)["description"] = "MQTT communication layer, communicate with a MQTT server (core)";

        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = nameFX+"/config";
                x["desc"] = "Configure the MQTT server";

                JsonArray& t = x.createNestedArray("params");
                CommUtil::addFXParam(&t, "server", "string", "Name of the server to connect to");
                CommUtil::addFXParam(&t, "port", "number", "Port to connect to (default 1883)");
                CommUtil::addFXParam(&t, "user", "string", "User of the mqtt server (empty = no user)");
                CommUtil::addFXParam(&t, "password", "string", "Password of the mqtt server (empty = no user)");
                CommUtil::addFXParam(&t, "basetopic", "string", "The basetopic will be added at the beginning of the string");


        }


};

void MQTT::loop() {
        if (!client.connected()) {
                conn();
        }
        client.loop();
};

void MQTT::conn(bool force) {

        if (client.connected()) {
                return;
        }

        unsigned long d = millis() - timeLastReconnect;

        // Don't do it too often (once every 5 seconds)
        if (!force && (d>=0 && d<5000) && timeLastReconnect!=0) {
                return;
        }

        timeLastReconnect = millis();

        // Loop until we're reconnected

        //Serial.print("Attempting MQTT connection...");

        String user = config->getString(nameFX + ".user");
        String password = config->getString(nameFX + ".password");


        bool res = false;
        bool withPasswd = false;
        if (user.length()>0)  {
                res = client.connect(nameAP.c_str(), user.c_str(), password.c_str());
                withPasswd = true;
        }
        else {
                res = client.connect(nameAP.c_str());
        }


        if (res) {
                Serial.println("MQTT Connected");

                pubMap.clear();

                IPAddress myIP = WiFi.localIP();
                publish(nameFX, "setup", nameAP + "," + myIP.toString());

                for (std::map<String,THandlerFunction1p>::iterator it=subMap.begin(); it!=subMap.end(); ++it) {
                        client.subscribe(it->first.c_str());
                }

        } else {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.print(", server=");
                Serial.print(config->getString(nameFX + ".server"));
                Serial.print(", port=");
                Serial.print(config->getInt(nameFX + ".port", 1883));
                if (withPasswd) {
                        Serial.print(", with password");
                }
                Serial.println("");
                return;
        }



};


void MQTT::servConfig() {


        bool save = false;

        if (webServer->hasArg("server")) {
                String server = webServer->arg("server");
                server.trim();
                if (server.length() == 0) {
                        server = EFT_DEFAULT_MQTT;
                }
                config->setString(nameFX + ".server", server);
                save = true;
        }

        if (webServer->hasArg("port")) {
                String port = webServer->arg("port");
                int portI = port.toInt();
                if (portI>0) {
                        config->setInt(nameFX + ".port", portI);
                }
                else {
                        config->setInt(nameFX + ".port", 1883);
                }
                save = true;
        }

        if (webServer->hasArg("user")) {
                String user = webServer->arg("user");
                config->setString(nameFX + ".user", user);
                save = true;
        }

        if (webServer->hasArg("password")) {
                String password = webServer->arg("password");
                config->setString(nameFX + ".password", password);
                save = true;
        }


        if (webServer->hasArg("basetopic")) {
                serverBaseTopic = webServer->arg("basetopic");
                if (serverBaseTopic.length() == 0) {
                        serverBaseTopic = nameAP;
                }
                config->setString(nameFX + ".basetopic", serverBaseTopic);
                save = true;
        }

        if (save) {
                serverConfig = config->getString(nameFX + ".server");
                client.setServer(serverConfig.c_str(), config->getInt(nameFX + ".port", 1883));
                client.disconnect();
                timeLastReconnect = 0;
        }


        if (save) {
                config->saveConfig();
        }

        config->servConfig(nameFX + ".server", nameFX + ".port", nameFX + ".user", nameFX + ".password", nameFX + ".basetopic");

}

void MQTT::subscribe(String nameFX, String topic, THandlerFunction1p handler) {
        String t = getTopic(nameFX, topic);
        subMap[t] = handler;
        client.subscribe(t.c_str());

};

void MQTT::publish(String nameFX, String topic, String message) {
        String t = getTopic(nameFX, topic);
        pubMap[t] = message;
        Serial.println("Post to MQTT ["+t+"] " + message);
        client.publish(t.c_str(),message.c_str());
};

String MQTT::getTopic(String nameFX, String topic) {
        String t = "/";
        if (serverBaseTopic.length() == 0) {
                t = t + nameAP;
        }
        else {
                t = t + serverBaseTopic;
        }

        if (nameFX.length() > 0) {
                t = t + "/" + nameFX;
        }

        if (topic.length() > 0) {
                t = t + "/" + topic;
        }

        return t;
}


}
