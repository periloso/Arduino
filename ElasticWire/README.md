ElasticWire Serial Library
========

## FEATURES
Allows an addressable serial communication.
It has been developed with RS485 usage in mind, but can be useful anytime you
need to access multiple peripherals from one or many masters.

The APIs are pretty straightforward, so you won't even need to think about "how
do I use this thing?!"

### Packet format


| Position  | Name       | Type   |
|-----------|------------|--------|
| ``0x00``  | src        | uint32 |
| ``0x04``  | dst        | uint32 |
| ``0x0a``  | type       | uint8  |
| ``0x08``  | id         | uint16 |
| ``0x0b``  | len        | uint8  |
| ``len-1`` | payload    | char[] |
| ``len``   | checksum   | uint8  |

### USAGE
```
// Stores the MAC address inside EEPROM 0x00
ElasticWire EW(Serial);

// Stores the MAC address inside EEPROM 0x04
ElasticWire EW(Serial, 0x04);

// Sends "ABC", payload length 0x03, destination BROADCAST, type INFO
EW.sendPacket("ABC", 0x03, BROADCAST, INFO);
```
