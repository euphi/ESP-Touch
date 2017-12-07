# ESP-Touch
Touchcontroller for ESP8266 and MPR121


To compile, use platformio.

Unfortunately the MPR121 is not available in a platformio-compatible format for direct dowwnload by platformio.
So you have to clone it manually into the lib subdirectoy, e.g by.

```
cd lib
git clone https://github.com/BareConductive/mpr121
cd ..
platformio run
``` 