# ESP-EFTCore

ESP-EFTCore is an easy way to control sensor on an ESP8266.

# Example

```c
void setup() {

        // Use serial
        Serial.begin ( 115200 );

        // First setup the hub
        // It will first be opened as an an access point.
        // Connect to this access point and set your network preferences
        eft::SetupRGB setupRGB(D6, D7, D8);
        // hub.setup() No visual information
        // hub.setup(eft::ISetupNotif) Send some message to the ISetupNotif class.
        // This class can be deleted after this call!
        eft::ISetupNotif *isn = &setupRGB;
        hub.setup(isn);

        // Then add the module your need. They need to implement IComm
        // Don't delete them.....
        // Support time, timezone
        hub.add("time", &mTime);

		// Add a neopixel (third pary lib)
        hub.add("neo", new eft::MNeoPixel(D4, 16));

		// Add a DHT22 sensor
        hub.add("dht", new eft::MDht(D3, DHT22));


        //eft::localtime ymhms = mTime.getLocalTime();


}

void loop() {
        hub.loop();
}
```


More information later! Initial release.
