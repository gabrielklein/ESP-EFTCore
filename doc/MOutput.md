# MOutput

This module simply switch on or off one of the output.

It could be used to drive a relay.

# Where to buy and how to install

Many different types of modules are working with this pin. Leds, switch and many other things.


As an example, if you want to control 110 - 220 volts, you can use

5V relay module KY-019 (<1$) (~0.250 Watt)
https://www.aliexpress.com/store/product/10PCS-LOT-1-Channel-5V-Relay-Module-for-SCM-Household-Appliance-Control/110055_752210006.html

You may need to use a resistor.

Found on internet the following equation:
(3.3V - 0.7V)/35mA = 75 ohm - that could be used to protect the output pin.


You can otherwise use a Solid State Relay Module ~3$ (highly recommended! And easier to use. < ~ 0.100 Watt)
https://www.aliexpress.com/item/1pcs-SSR-10DA-10A-Solid-State-Relay-Module-3-32V-DC-Input-24-380VAC/32700727110.html





# Integration

You can include the library using

```
#include "module/MOutput.h"

[...]

// The first parameter is the pin to be controlled
// If the second parameter is set to true, HIGH / LOW will be reversed
hub.add("relayx", new eft::MOutput(D3, false));

```


# Alternate version

You can use the RGB led to get notifications during the setup phase.

```

#include "module/MOutput.h"

[...]

eft::MOutput mOutput(D3, false);
hub.setup(&mOutput);

```

## C++ integration

You can switch on or off the pin (set it to LOW or HIGH) using

mOutput->switchOnOff(true)
mOutput->switchOnOff(false)



# Core Web API

## /api/{name}?enable=0


You can switch on or off the pin calling

Switch on:
/api/{name}?enable=1

Switch off:
/api/{name}?enable=0


Example:
http://192.168.0.1/api/relay?enable=0

Response is like
```
{"status":"ok","time":2561554}
```

# MQTT

This module will subscribe to

/{baseTopic}/{nameFX}/status (Example: /mymodule/switch/status)

You can then send to this channel

* 1 to switch it on (set it to HIGH)
* 0 to switch it off (set it to LOW)

When the status is changed, 1 or 0 is sent to the MQTT channel.
