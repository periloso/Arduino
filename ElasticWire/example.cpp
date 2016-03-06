#include "Arduino.h"
#include "elasticwire.h"

ElasticWire EW(Serial, 0x00);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  EW.sendPacket("ABC", 0x03, BROADCAST, INFO);
  delay(70);
}
