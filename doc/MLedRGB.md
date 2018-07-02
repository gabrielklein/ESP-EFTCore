# MLedRGB

This module is a way to connect a simple 4 wires RGB led to module. It's a good way to get notifications or warning.

# Where to buy and how to install

You can buy RGB leds in many places for less than 1$. Please take care of the resistance!

As an example you can buy.
https://www.aliexpress.com/item/10PCS-Smart-Electronics-FZ0455-4pin-KEYES-KY-016-Three-Colors-3-Color-RGB-LED-Sensor-Module/32765591585.html?spm=a2g0s.9042311.0.0.OSt3fD




# Integration

You can include the library using

```
#include "module/MLedRGB.h"

[...]

hub.add("rgb", new eft::MLedRGB(D6, D7, D8));

```


# Alternate version

You can use the RGB led to get notifications during the setup phase.

```

#include "module/SetupRGB.h"

[...]

eft::SetupRGB setupRGB(D6, D7, D8);
hub.setup(&setupRGB);

```

## C++ integration

You can change the led using

rgb(bool r, bool g, bool b)

As a example to set the red to true:
rgbLed -> rgb(true, false, false)



# Core Web API

## /api/{name}?r=1&g=1&b=0
## /api/{name}?rgb=101


You have two ways of calling the api.

/api/{name}?r=1&g=1&b=0

Where
* r=0 or r=1 set the red on of off.
* g=0 or g=1 set the green on of off.
* b=0 or b=1 set the blue on of off.

Response is like
```
  {
    "r":1,
    "g":0,
    "b":0
  }
```

The other way is to set the rgb parameter
/api/{name}?rgb=101

rgb can be set to
* A value between 0 and 7. 0 means all off, seven all on, 4 is red, 2 is green, 1 is blue.
* r, g, b, means red, green, blue
* Value with 3 time 0 or 1. Example 100 (red), 001 (blue), 010 green
*
Example (to set it to red)
* /api/{name}?rgb=4
* /api/{name}?rgb=red
* /api/{name}?rgb=100




# MQTT

This module will subscribe to

/{baseTopic}/{nameFX} (Example: /mymodule/rgb)

You can then send to this channel

* A value between 0 and 7. 0 means all off, seven all on, 4 is red, 2 is green, 1 is blue.
* r, g, b, means red, green, blue
* Value with 3 time 0 or 1. Example 100 (red), 001 (blue), 010 green

Example (to set it to red)
* Send 4
* Send red
* Send 100


Status of the leds is sent to

/{baseTopic}/{nameFX}/Status (Example: /mymodule/rgb/status)

The status is a 3 digit number with only 1 and 0. Example: 101, 110, 000.

* 100: red
* 010: green
* 001: blue
