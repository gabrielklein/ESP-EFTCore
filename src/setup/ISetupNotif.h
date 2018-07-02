#ifndef EFT_SETUP_NOTIF_H
#define EFT_SETUP_NOTIF_H

#include <string>

namespace eft {

/**
 * Generic interface to notify wlan connexion phases.
 */
class ISetupNotif {

public:

/**
 * We cannot connect to the network
 */
virtual void error();

/**
 * Give the IP of the ESP
 */
virtual void client(String ip);

/**
 * We have successfully connected to the network
 */
virtual void success();

/**
 * We are in progress of connecting to the network.
 */
virtual void inprogress();


};
}



#endif
