#include "WebServer.h"

namespace eft {

WebServer::WebServer() {

}

WebServer::~WebServer() {
        ws.stop();
}


void WebServer::setup(String nameAP, String nameFX) {

        this->nameAP = nameAP;

        Serial.println("WebServer: Setup");

        ws.on("/api/scan", HTTP_GET, [&] () {
                        this->apiScan();
                });
        ws.on("/api/reboot", HTTP_GET, [&] () {
                        this->apiReboot();
                });

        ws.begin();
}

void WebServer::loop() {
        unsigned long now = millis();
        ws.handleClient();
        unsigned long duration = millis() - now;
        if (duration > 100) {
                Serial.print("WebServer: is a bit long: ");
                Serial.println(duration);
        }
}

/**
 * Scan wireless network
 */
void WebServer::apiScan() {
        int n = WiFi.scanNetworks();
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        JsonArray& networks = json.createNestedArray("ap");
        for(int i=0; i<n; i++) {
                JsonObject& data = networks.createNestedObject();
                data["ssid"] = WiFi.SSID(i);
                String e = "";
                e = e + WiFi.encryptionType(i);
                data["enc"] = e;
                data["dbm"] = WiFi.RSSI(i);
                data["bssid"] = WiFi.BSSIDstr(i);
                data["channel"] = WiFi.channel(i);
                data["isHidden"] = WiFi.isHidden(i);
        }
        //String root;
        String s;
        json.printTo(s);
        ws.send(200, "application/json", s);

};


/**
 * Just reboot
 */
void WebServer::apiReboot() {
        ServerUtil::sendSuccess(&ws);
        delay(100);
        ESP.reset();
};

}
