#include "analogWave.h"

analogWave wave(DAC);

int freq = 1000;  // in hertz, change accordingly
const int freq_ctrl_pin_vcc = A3;
const int freq_ctrl_pin = A4;
const int freq_ctrl_pin_gnd = A5;

void setup(void)
{
  Serial.begin(115200);
  pinMode(freq_ctrl_pin_gnd, OUTPUT);
  digitalWrite(freq_ctrl_pin_gnd,LOW);
  pinMode(freq_ctrl_pin_vcc, OUTPUT);
  digitalWrite(freq_ctrl_pin_vcc,HIGH);
  wave.sine(freq);
}

void loop(void)
{
  freq = map(analogRead(freq_ctrl_pin), 0, 1024, 0, 10000);
  Serial.println("Frequency is now " + String(freq) + " Hz");
  wave.freq(freq);
  delay(100);
}
