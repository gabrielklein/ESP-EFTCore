# Web Rest API

All calls are done on http:// port 80. Only "GET" verb is supported. Parameters are sent using the ?xx=yy approach.

# Module rest API

You can get information concerning the different modules using. The answer to these requests is JSON based.

Pattern: /api/{moduleName}/{subpath}

Example:

You have added the "MDS18xx" module like

```
  hub.add("tempABC", new eft::MDS18xx(D1));
```

The ip of your module is 192.168.0.123

To access the temperature you calls

http://192.168.0.123/api/tempABC/


## Module configuration

Many modules have an api to configure the module in a "config" path.

Example: http://192.168.0.123/api/tempABC/config

You can change a configuration by adding parameters

Example: http://192.168.0.123/api/tempABC/config?fahrenheit=1&updateEveryS=7
This call will update the temperature of this module every 7 seconds (and send the information to mqtt). The temperature will be in fahrenheit.

# Core rest API

## /api/scan

Scan network

```
{
   "ap":[
      {
         "ssid":"NetworkA",
         "enc":"4",
         "dbm":-66,
         "bssid":"12:AA:BB:CC:DD:EE",
         "channel":1,
         "isHidden":false
      },
      {
         "ssid":"NetworkB",
         "enc":"8",
         "dbm":-80,
         "bssid":"45:AA:BB:CC:DD:EE",
         "channel":11,
         "isHidden":false
      }
   ]
}
```

## /api/reboot

Reboot module

```
{
   "status":"ok",
   "time":657046
}
```

## /api/module

Display currently enabled modules and mqtt subscribed and published topic.

Please note that published topic are only displayed after having been sent.

```
{
  "time":19443,"modules":[
    {"type":"MTime","name":"time"}
  ],
  "mqttSub":[
    {"topic":"/sensor/time/someSubscribed"}
  ],
  "mqttPub":[
    {"topic":"/sensor/mqtt/setup","message":"MyEFT_1EA6,10.0.0.160"}
  ]
}
```



## /api/config

Get configuration variables (key, value)

```
{
  "time":96436,
  "path":"config/mainconfig.txt",
  "config":{
    "key1":"value1",
    "key2":"value2",
    "key3":"value3"
  }
}
```

Modules variable are stored using the "modulename.key".

As an example when you add the "MDS18xx" module
: hub.add("temp", new eft::MDS18xx(D1));

The "module" is named "temp". It means configuration will be "temp.fahrenheit" in this example.

You can update configuration variables by adding configuration parameters.

/api/config?key1=newvalue1&key2=newvalue2

Please avoid updating configuration through this interface, because the module may not be notified of the change! Do it through the module's API.

You have no checks on these values too! Use the components API instead!
