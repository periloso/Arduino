#ifndef _ELASTICWIRE_CPP
#define _ELASTICWIRE_CPP
#include <stdint.h>

#define MAX_RETRIES    3
#define _MAX_PKT_SIZE  256
#define _PKT_SIZE      _HEADER_SIZE + data_len
#define _HEADER_SIZE   16
#define _PAYLOAD_SIZE  _MAX_PKT_SIZE - _HEADER_SIZE
#define _MAGIC         0x5745
#define BROADCAST      0xffffffff

#define LIST 0
#define INFO 1
#define DATA 2
#define REQ  3
#define RES  4
#define ACK  5

#include "Arduino.h"
#include <string.h>
#include <EEPROM.h>

struct Packet {
  uint16_t magic;
  uint32_t dst;
  uint32_t src;
  uint8_t  type;
  uint16_t id;
  uint8_t  seq;
  uint8_t  features;
  uint8_t  len;
  unsigned char payload[_PAYLOAD_SIZE];
  uint8_t  checksum;
};

class ElasticWire {
  private: Packet pkt;
  private: char rxbuf[_MAX_PKT_SIZE];
  private: HardwareSerial *refSer;
  private: uint32_t mac;
  private: uint32_t pkt_id;
  private: uint8_t  seq;
  public: Packet getData(char * buf, uint8_t len);
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
  void sendPacket(T data, uint8_t data_len, uint32_t dst, uint8_t type, uint8_t features) {
    uint8_t *ptr;
    this->pkt.checksum = 0;
    this->pkt.magic = _MAGIC;
    memcpy(&this->pkt.dst, &dst, 4);                       // dst
    memcpy(&this->pkt.src, &this->mac, 4);                 // src
    memcpy(&this->pkt.type, &type, 1);                     // pkt_type
    memcpy(&this->pkt.id, &pkt_id, 2);                     // pkt_id
    memcpy(&this->pkt.seq, &seq, 1);                       // pkt_seq
    memcpy(&this->pkt.features, &features, 1);             // features
    memcpy(&this->pkt.len, &data_len, 1);                  // payload_len
    memcpy(&this->pkt.payload, data, data_len);            // payload

    // Create checksum
    ptr = (uint8_t *)&this->pkt;
    for(int i = 0; i < _PKT_SIZE - 1; i++) {
      this->pkt.checksum ^= *ptr++;
    }

    // The pointer points to the struct address to set the checksum
    ptr = (uint8_t *)&this->pkt;
    memcpy(ptr + _PKT_SIZE - 1, &this->pkt.checksum, 1);

    // Send data
    this->refSer->write((char *) ptr, _PKT_SIZE);
    this->pkt_id++;
  }
};

#endif
