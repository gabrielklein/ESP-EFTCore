//
// This class is used for "notifications" on a RGB PIN
//
#ifndef EFT_MQTT_H
#define EFT_MQTT_H

#include <Arduino.h>

#include <functional>
#include <memory>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "../core/Config.h"
#include "CommUtil.h"

#define EFT_DEFAULT_MQTT "iot.eclipse.org"

namespace eft {

typedef std::function<void (String string)> THandlerFunction1p;

/**
 * This class is the mqtt interface.
 * It's a way to send and receive mqtt message. You can register MQTT too.
 */
class MQTT {


public:
MQTT();
~MQTT();

/**
 * Setup this module
 */
void setup(String nameAP, String nameFX, eft::Config *config);

/**
 * Name of this module (mqtt)
 */
String commType();

/**
 * Describe the mqtt module in a json message.
 */
void commFx(JsonObject *o, JsonArray *ja);

/**
 * Function to register the webserver part.
 */
void commWebServer(ESP8266WebServer *webServer);

/**
 * Main loop to receive and send messages.
 */
void loop();

/**
 * Subscribe on a channel and receive messages in a callback function
 */
void subscribe(String nameFX, String topic, THandlerFunction1p handler);
/**
 * Publish a message to the mqtt channel. You need to give the name of the calling module, a topic and the message to be send.
 */
void publish(String nameFX, String topic, String message);


/**
 * Function called by the web server to serv config.
 */
void servConfig();
std::map<String,THandlerFunction1p> subMap;
std::map<String,String> pubMap;

private:
String getTopic(String nameFX, String topic);
ESP8266WebServer *webServer;
//EthernetClient ethClient;
PubSubClient client;
WiFiClient *ethClient = NULL;
void conn(bool force = false);
unsigned long timeLastReconnect;
void callback(char* topic, byte* payload, unsigned int length);
String nameAP;
String nameFX;
eft::Config *config;

// We need to keep it locally
String serverConfig = "";
String serverBaseTopic = "";
};

}

#endif
