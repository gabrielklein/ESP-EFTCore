#ifndef EFT_COMMUTIL_H
#define EFT_COMMUTIL_H

#include <Arduino.h>
#include <ArduinoJson.h>

namespace eft {

class CommUtil
{
public:

/**
 * Add a parameters of a function to a json array.
 */
static void addFXParam(JsonArray *ja, String name, String type, String desc);

private:
CommUtil() {
}
};

}

#endif
