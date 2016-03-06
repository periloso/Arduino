#ifndef _ELASTICWIRE_CPP
#define _ELASTICWIRE_CPP
#include <stdint.h>

#define _PKTSIZE   256
#define BROADCAST  0xffffffff

#define LIST 0
#define INFO 1
#define DATA 2
#define REQ  3
#define RES  4
#define ACK  5

#include "Arduino.h"
#include <string.h>
#include <EEPROM.h>

class ElasticWire {
  private: char msg[_PKTSIZE];
  private: HardwareSerial *refSer;
  private: uint32_t mac;
  private: uint32_t pkt_id;
  public: template <typename T>
    ElasticWire(T &refSer, uint32_t mac) {
      this->refSer = &refSer;
      this->pkt_id = 0;

      this->mac = EEPROM.read(mac+00);
      this->mac = (this->mac << 8) | EEPROM.read(mac+01);
      this->mac = (this->mac << 8) | EEPROM.read(mac+02);
      this->mac = (this->mac << 8) | EEPROM.read(mac+03);

      if (this->mac == 0xFFFFFFFF) {
        randomSeed(analogRead(5));
        this->mac = random(0, 65535);
        this->mac = (this->mac << 16) | (random(0, 65535));
        EEPROM.write(mac+00, this->mac >> 24 & 0xff);
        EEPROM.write(mac+01, this->mac >> 16 & 0xff);
        EEPROM.write(mac+02, this->mac >> 8 & 0xff);
        EEPROM.write(mac+03, this->mac & 0xff);
      }
    }
  public:  template <typename T>
  void sendPacket(T data, uint8_t len, uint32_t dst, uint8_t type) {
    uint8_t checksum = 0;

    memcpy(this->msg+0x00, &this->mac, 4);                // src
    memcpy(this->msg+0x04, &dst, 4);                      // dst
    memcpy(this->msg+0x08, &type, 1);                     // pkt_type
    memcpy(this->msg+0x09, &pkt_id, 2);                   // pkt_id
    memcpy(this->msg+0x0b, &len, 1);                      // payload_len
    memcpy(this->msg+0x0c, data, len);                    // payload

    // Create checksum
    for(int i=0; i<0x0d+len; i++) {
      checksum ^= this->msg[i];
    }
    memcpy(this->msg+0x0c+len, &checksum, 1);             // checksum

    this->pkt_id++;
    this->refSer->write(this->msg, 0x0d+len);
  }
};

#endif
