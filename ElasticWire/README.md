ElasticWire Serial Library
========

## FEATURES
ElasticWire allows an addressable serial communication.
It has been developed with RS485 usage in mind, but can be useful anytime you need to access multiple peripherals from one or many masters.

The APIs are pretty straightforward, so you won't even need to think about "how do I use this thing?!"


### Packet logging
To explore and experiment with the protocol, a lightweight in-browser protocol logger has been developed.
To enable it, you can issue:

```
cd node.js
npm install
node .
```

The daemon will then say it is running on localhost:3000... so let's just connect to it: [http://localhost:3000](http://localhost:3000)!! ;-)
To make it easier to read, the protocol logger serves a html page made with bootstrap, jquery and socket.io to allow a great responsiveness.

### Packet format

| Position   | Name           | Type    |
|------------|----------------|---------|
| ``0x00``   | magic_code     | char[2] |
| ``0x02``   | destination    | uint32  |
| ``0x06``   | source         | uint32  |
| ``0x0a``   | type           | uint8   |
| ``0x0b``   | id             | uint16  |
| ``0x0d``   | sequence_id    | uint8   |
| ``0x0e``   | features       | uint8   |
| ``0x0f``   | payload_length | uint8   |
| ``0x10``   | payload        | char[]  |
| ``-len-``  | checksum       | uint8   |

### Packet format description
The ElasticWire protocol allows reliable and unreliable transmissions using both packet ACKs and addressable communications.
The packets are automatically fragmented if they are bigger than 240 bytes.

In case of fragmentation, the ``sequence id`` is incremented for each fragment, and the ``fragment_continues`` flag inside the ``features`` byte is enabled.
When the last fragment is sent, the flag ``fragment_continues`` inside the ``features`` byte is disabled.

If a reliable transmission is requested, the ``reliable`` bit inside the ``features`` is enabled;
in such case, retransmissions will occur automatically for three times before the transmission is discarded and an error is returned by the function ``sendPacket``.
In case you need to adapt the retransmissions, you can change their value inside ``elasticwire.h``, the value is called ``MAX_RETRIES``. In case of transmission failure (the ACK is not received), the function ``sendPacket`` returns a non-zero value.

To confirm a transmission, an ``ACK`` typed packet will be sent, with ``destination``, ``source``, ``sequence_id`` and ``type`` set accordingly (everything is done automatically).

The ``magic_code`` is ``EW`` by default and it is used to detect the point where a transmission starts.
In case you need it, you can change it editing ``elasticwire.h`` and changing the ``_MAGIC`` value.


### USAGE
```
// Stores the MAC address inside EEPROM 0x00
ElasticWire EW(Serial);

// Stores the MAC address inside EEPROM 0x04
ElasticWire EW(Serial, 0x04);

// Sends "ABC", payload length 0x03, destination BROADCAST, type INFO
EW.sendPacket("ABC", 0x03, BROADCAST, INFO, FEATURES);
```
