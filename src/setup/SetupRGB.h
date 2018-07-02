#ifndef EFT_SETUPRGB_H
#define EFT_SETUPRGB_H

#include <Arduino.h>
#include <string>
#include "ISetupNotif.h"

namespace eft {

/**
 * Module used to notify during setup using a simple RGB (3 pins + 2) led.
 */
class SetupRGB : public ISetupNotif {

public:
SetupRGB(int pinR,int pinG,int pinB);
~SetupRGB();

/**
 * Called when we have an error
 */
void error();
/**
 * Called in case of success by giving the calling ip
 */
void client(String ip);
/**
 * Called if we have successfully connected to a network.
 */
void success();
/**
 * Called when we are connecting
 */
void inprogress();


private:
int pinR;
int pinG;
int pinB;

};



}

#endif
