#include "CommUtil.h"

namespace eft {

void CommUtil::addFXParam(JsonArray *ja, String name, String type, String desc) {
        JsonObject& x2 = (*ja).createNestedObject();
        x2["param"] = name;
        x2["type"] = type;
        x2["desc"] = desc;
};


}
