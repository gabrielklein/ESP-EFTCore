#include "Hub.h"

namespace eft {

Hub::Hub(String name) {

        uint8_t mac[WL_MAC_ADDR_LENGTH];
        WiFi.softAPmacAddress(mac);
        String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                       String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
        macID.toUpperCase();
        String apName = name +"_" + macID;

        this->nameAP = apName;
        fileServ.setup();
        config.setup(nameAP, "config", "config/mainconfig.txt", &fileServ);
        for (int i=0; i<EFT_MAX_MODULE; i++)
                modules[i] = NULL;
        modulesCount = 0;
};

Hub::~Hub() {
        for (int i=0; i<modulesCount; i++) {
                delete(modules[i]);
                modules[i] = NULL;
        }
};

/**
 * Called at setup time. Use this call to initialize some data.
 */
void Hub::setup(ISetupNotif *iSetupNotif) {

        Serial.println ("\nStarting Hub");

        if (iSetupNotif!=NULL)
                iSetupNotif->inprogress();

        WiFiManager wifiManager;

        Serial.print("\nAPName: ");
        Serial.println(nameAP);

        // Potenially reset settings
        //wifiManager.resetSettings();

        if(!wifiManager.autoConnect(nameAP.c_str())) {
                if (iSetupNotif!=NULL)
                        iSetupNotif->error();
                delay(3000);
                //reset and try again, or maybe put it to deep sleep
                ESP.reset();
                delay(5000);
        }

        IPAddress myIP = WiFi.localIP();
        Serial.println("Connected, ip: ");
        Serial.println(myIP);

        if (iSetupNotif!=NULL)
                iSetupNotif->success();


        // ---------------

        // Setup webserver
        this->fileServ.setup();
        webServer.setup(nameAP, "webserver");
        config.commWebServer(&webServer.ws);

        // Setup mqtt
        mqtt.setup(nameAP, "mqtt", &config);
        mqtt.commWebServer(&webServer.ws);

        // Add hub calls
        webServer.ws.on("/api/modules", HTTP_GET, [&] () {
                        this->servModuleList();
                });

        // Add hub calls
        webServer.ws.on("/api/reset", HTTP_GET, [&] () {
                        this->servResetAP();
                });

        webServer.ws.onNotFound([&] () {
                        this->servFiles();
                });
};

void Hub::servFiles() {

        //Serial.println("WebServerSN: Forbidden access to this file!");
        unsigned long time = millis();
        String s2 = "<html><head><title>No file</title></head><body>File not found. "+String(time)+"</body></html>";
        webServer.ws.send(404, "text/html", s2);
        return;

}


void Hub::commFx(JsonObject *o, JsonArray *ja) {
        (*o)["description"] = "Core functionnalities of the hub and server";
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = "modules";
                x["desc"] = "List all compiled modules";
        }
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = "scan";
                x["desc"] = "Scan the network";
        }
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = "reset";
                x["desc"] = "Reset AP settings of the access point (don't reset other settings)";
        }
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = "reboot";
                x["desc"] = "Reboot the module";
        }
        {
                JsonObject& x = (*ja).createNestedObject();
                x["api"] = "config";
                x["desc"] = "View all configuration settings.";
                JsonArray& t = x.createNestedArray("params");
                JsonObject& x2 = t.createNestedObject();
                x2["param"] = "{configurationVariable}";
                x2["type"] = "String";
                x2["desc"] = "Change a variable - change is effective at reboot";
        }

}

void Hub::servResetAP() {

        ServerUtil::sendSuccess(&(webServer.ws));
        delay(100);
        WiFiManager wifiManager;
        wifiManager.resetSettings();
        delay(100);
        ESP.reset();
        delay(1000);

}

void Hub::servModuleList() {

        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.createObject();
        root["time"] = millis();
        //JsonObject& t = root.createNestedObject("modules");
        JsonArray& t = root.createNestedArray("modules");

        // Core
        {
                JsonObject& x = t.createNestedObject();
                x["type"] = "core";
                x["name"] = "core";
                JsonArray& fx = x.createNestedArray("fx");
                commFx(&x, &fx);
        }

        // MQTT
        {
                JsonObject& x = t.createNestedObject();
                x["type"] = mqtt.commType();
                x["name"] = "mqtt";
                JsonArray& fx = x.createNestedArray("fx");
                mqtt.commFx(&x, &fx);
        }

        for (int i=0; i<modulesCount; i++) {
                JsonObject& x = t.createNestedObject();
                x["type"] = modules[i]->commType();
                x["name"] = modulesName[i];
                JsonArray& fx = x.createNestedArray("fx");
                modules[i]->commFx(&x, &fx);
        }

        JsonArray& t2 = root.createNestedArray("mqttSub");

        for (std::map<String,THandlerFunction1p>::iterator it=mqtt.subMap.begin(); it!=mqtt.subMap.end(); ++it) {
                JsonObject& x = t2.createNestedObject();
                x["topic"] = it->first.c_str();
        }

        JsonArray& t3 = root.createNestedArray("mqttPub");

        for (std::map<String,String>::iterator it=mqtt.pubMap.begin(); it!=mqtt.pubMap.end(); ++it) {
                JsonObject& x = t3.createNestedObject();
                x["topic"] = it->first.c_str();
                x["message"] = it->second.c_str();
        }


        String s;
        root.printTo(s);
        webServer.ws.send(200, "application/json", s);

}


void Hub::add(String nameFX, IComm *r) {
        Serial.print("Adding module: ");
        Serial.print(nameFX);
        Serial.print(", type: ");
        Serial.println(r->commType());

        modulesName[modulesCount] = nameFX;
        r->setup(nameAP, nameFX, &config);
        r->commWebServer(&webServer.ws, &mqtt);

        modules[modulesCount] = r;
        modulesCount++;
};


/**
 * Loop
 */
void Hub::loop() {

        webServer.loop();
        mqtt.loop();

        for (int i=0; i<modulesCount; i++) {
                modules[i]->loop();
        }

};

}
