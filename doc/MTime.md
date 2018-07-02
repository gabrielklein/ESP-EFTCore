# MTime

This module will synchronize the time of your ESP with the network. By setting the timezone, you will be able to get the local time.

This module is useful if you want to do actions at specific time.

# Where to buy and how to install

Nothing to be bought :)

# Integration

You can include the library using
```
#include "module/MTime.h"

....

Then add the library to the hub

hub.add(new eft::MTime());

```

or

```
#include "module/MTime.h"
eft::MTime mTime;

....

Then add the library to the hub

hub.add(&mTime);

```

And add the following libraries (tip: in the lib_deps section of your platformio.ini)

*  NTPClient
*  Time
*  Timezone

## C++ integration

### getLocalTime();

You can get the local time using

....

  eft::localtime ymhms = mTime.getLocalTime();

  Then you can have access to the different parameters of the time
  ymhms.year;
  ymhms.month;
  ymhms.hour;
  ymhms.minute;
  ymhms.second;
  ymhms.weekday;
  ymhms.localTime;

....

Then you can write rules like

Switch on between 8am to 5pm

....

if (loop())

if (ymhms.hour>=8 && ymhms.hour<17) {
  if (!switch.isEnabled())
    switch.enable();
}
else {
  if (switch.isEnabled())
    switch.disable();  
}

....


# Core Web API

## /api/{name}

Get the current time.

The time is your local time, and the UTC time.

```
{
  "utc":{"timezone":"UTC","epochTime":1515455309,"hms":"23:48:29","hour":23,"minute":48,"second":29,"day":8,"weekday":2,"month":1,"year":2018},
  "timeUnix":1515455309,"local":{"timezone":"CE","hms":"00:48:29","hour":0,"minute":48,"second":29,"day":9,"weekday":3,"month":1,"year":2018,"tcr":"CET"}
}
```

## /api/{name}/config

Example: /api/time/config?timezone=CE

This API will change the timezone. You should get the following message.

```
{"time":194064,"config":{"time.timezone":"CE"}}
```

You can get an error if the timezone is incorrect

```
{"status":"error","id":11001,"desc": "Timezone was not found: xxx","time":590112}
```

## /api/{name}/timezone

Get supported timezones

```
{
  "time": 2872139,
  "tz": [
    {
      "id": "UTC",
      "label": "Coordinated Universal Time (GMT)",
      "tcr": "GMT",
      "hms": "22:04:54",
      "hour": 22,
      "minute": 4,
      "second": 54
    },
    {
      "id": "ausET",
      "label": "Australia Eastern Time Zone (Sydney, Melbourne)",
      "tcr": "AEDT",
      "hms": "09:04:54",
      "hour": 9,
      "minute": 4,
      "second": 54
    },
    [ ... ]
  ]
}
```

# MQTT

No message published or taken from MQTT.
