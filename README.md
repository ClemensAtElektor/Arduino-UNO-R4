# Arduino-UNO-R4
Exploring UNO R4 Minima &amp; WiFi features.

On the UNO R4, Serial is the USB port, Serial1 is on pins 0 & 1.

Put
```
#define NO_USB
#include <Arduino.h>
```
at the top of the sketch to route Serial to pins 0 and 1.

On the WiFi Serial2 is connected to the Wi-Fi module.
