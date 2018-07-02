#ifndef EFT_ICOMM_H
#define EFT_ICOMM_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "../core/Config.h"
#include "CommUtil.h"
#include "MQTT.h"

namespace eft {

/**
 * This class is implemented by all modules. It's the set of function that should be supported to describe the functionlities of a module.
 */
class IComm {

public:

/**
 * Return the name of this class.
 */
virtual String commType();
/**
 * Add the description of the API that interact with this module.
 */
virtual void commFx(JsonObject *o, JsonArray *);
/**
 * Setup this module.
 */
virtual void setup(String nameAP, String nameFX, eft::Config *config);
/**
 * Register the web and mqtt part of this module.
 */
virtual void commWebServer(ESP8266WebServer *webServer, MQTT *mqtt);
/**
 * Main loop called every time the "loop()" in the hub is called.
 */
virtual void loop();
};

}

#endif
