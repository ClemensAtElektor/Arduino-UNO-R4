/*
 * Purpose: 
 * Board: 
 * IDE: 1.8.19
 *
 * Usage:
 *
 * By: Clemens Valens
 * Date: //202
 */

void setup(void)
{
  Serial.begin(115200); // USB serial
  Serial1.begin(9600); // Pins 0 & 1
}

void loop(void)
{
  Serial.write('0');
  Serial1.write(0x55);
  delay(15);
}
