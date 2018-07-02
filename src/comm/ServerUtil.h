#ifndef EFT_SERVERUTIL_H
#define EFT_SERVERUTIL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

namespace eft {

/**
 * Static utils for the server part
 */
class ServerUtil
{
public:

/**
 * Get the mime type of an extension
 */
static String getMime(String name);
/**
 * Send a json success message
 */
static void sendSuccess(ESP8266WebServer *webServer);
/**
 * Send a json fail message
 */
static void sendFail(ESP8266WebServer *webServer, int errorId, String errorDesc);

private:
ServerUtil() {
}
};

}

#endif
