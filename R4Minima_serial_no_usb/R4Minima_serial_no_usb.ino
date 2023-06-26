/*
 * Purpose: Try routing Serial to pins 0 & 1.
 * Board: UNO R4
 * IDE: 1.8.19
 *
 * Usage:
 *
 * By: Clemens Valens
 * Date: 26/6/2023
 */

// Uncomment to route Serial to pins 0 & 1.
#define NO_USB
#include <Arduino.h>

void setup(void)
{
  Serial.begin(9600);
#ifndef NO_USB
  Serial1.begin(9600);
#endif
}

void loop(void)
{
  Serial.write('$'); // TX on pin 1 if NO_USB is defined
#ifndef NO_USB
  Serial1.write('<'); // TX on pin 1 if NO_USB is not defined
#endif
  delay(20);
}
