#include <Arduino.h>
#include <ChipUUID.h>
#define LED PC13

ChipUUID chipUUID;

void setup() {
  // initialize digital pin PB1 as an output.
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  digitalWrite(LED, LOW);
}

void loop() {
  digitalWrite(LED, HIGH);    // turn the LED on by making the voltage LOW
  delay(100);
  
  ChipUUID UUID;
  
  Serial.print("From DeviceID in hex: ");
  Serial.println((char *) UUID.asHex);
  
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage HIGH
  delay(100);
}
