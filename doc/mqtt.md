# MQTT support

Relevant information is sent using MQTT.

# Configuration

You have the following parameters:

* server: The server where your MQTT
* port: The port that will be used by your mqtt server

* user: The user of your mqtt server. You can leave it blank if you don't want to use it.
* password: The password of your mqtt server.

* basetopic: The "basetopic" will be added to all your message.

You can easily configure mqtt using a rest call.

http://192.168.0.123/api/mqtt/?server=iot.eclipse.org&port=1883&user=me&password=abc123&basetopic=mysensor/livingroom

# Integration with modules (Publish)

If a module is able to send information (like a temperature sensor), the information will be sent using mqtt.

All modules are added with a special name. As an example the MDS18xx module is added with the temp789 in this case.

```
  hub.add("temp789", new eft::MDS18xx(D1));
```

The basetopic is added at the begininning of the name (with a trailing /)

As an example is the basetopic is mysensor/livingroom

This module will send information on
/mysensor/livingroom/temp789/{something}

As an example MDS18xx will send information on

* /mysensor/livingroom/temp789/ : Onewire modules id with the current temperature will be send on this channel.
* /mysensor/livingroom/temp789/281234567891234: Temperature of this OneWire device will be sent to this channel

If you want to listen to information on this channel, just register to /mysensor/livingroom/temp789/ and /mysensor/livingroom/temp789/281234567891234.

# Integration with modules (Subscribe)

If a module is able to received information (Like a switch). The module will subscribe to a special channel.

If we have added the switch using
```
  hub.add("switch532", new eft::MSimpleSwitch(D4));
```

The the basetopic is added at the begininning of the name (with a trailing /)

As an example is the basetopic is mysensor/livingroom

This module will subscribe to
/mysensor/livingroom/switch532

If you send 1 to this channel using a third party application, the switch will be switched on.

If you send 0 to this channel using a third party application, the switch will be switched off.

# Debug

You can get some information using the following call: http://{moduleIP}/api/module

Please note that information is only available when a module send information.
