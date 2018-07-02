//
// This class is used to share and distribute files
//
#ifndef EFT_FILESERV_H
#define EFT_FILESERV_H

#include <Arduino.h>
#include "FS.h"


namespace eft {

/**
 * File services
 */
class FileServ {

public:
FileServ();
~FileServ();
void setup();
File openR(const String &string);
File openW(const String &string);
};

}
#endif
