"use strict"

module.exports = {
  validateChecksum: function validateChecksum(packet, checksum) {
    let _checksum = 0;
    for(let i=0; i<packet.length; i++)
    {
      _checksum ^= packet[i];
    }
    return _checksum === checksum;
  },
  handle: function handle(buffer, magic_string) {
    const _HEADERS_LEN    = 0x0f;
    const _MAX_BUFFER_LEN = 512;
    magic_string = magic_string || 'EW'; // If it's undefined, then it's 'EW'

    let buf = buffer;
    if (buffer.length > _HEADERS_LEN) {
      let pos = buf.indexOf(magic_string)
      if (pos === -1) {
        console.error('Cannot find the magic_string.\n');
        return {
          error: 'no_magic_string',
          buffer: new Buffer(0)
        };
      }
      buf = buf.slice(pos);

      try {
        this.packet.magic_string = this.parse(buf, 0x00, 'string', 2);
        this.packet.dst = this.parse(buf, 0x02, 'uint32bu');
        this.packet.src = this.parse(buf, 0x06, 'uint32bu');
        this.packet.type = this.parse(buf, 0x0a, 'uint8');
        this.packet.id = this.parse(buf, 0x0b, 'uint16lu');
        this.packet.seq = this.parse(buf, 0x0d, 'uint8');
        this.packet.features = this.parse(buf, 0x0e, 'uint8');
        this.packet.payload_length = this.parse(buf, 0x0f, 'uint8') - _HEADERS_LEN - 1;
        if (buf.length > (this.packet.payload_length + _HEADERS_LEN)) {
          this.packet.payload = this.parse(buf, 0x10, 'string', 0x10 + this.packet.payload_length - 1);
          this.packet.checksum = this.parse(buf, 0x10 + this.packet.payload_length - 1, 'uint8');
          this.packet.validated = this.validateChecksum(buf.slice(0, 0x10 + this.packet.payload_length - 1), this.packet.checksum);
          return {
            error: false,
            packet: this.packet,
            buffer: buf.slice(0x10 + this.packet.payload_length)
          };
        } else {
          return {
            error: 'incomplete',
            buffer: buf
          };
        }
      } catch(e) {
        console.log('There was an error: ', e);
        return {
          error: 'error',
          buffer: new Buffer(0) //buf.slice(2)
        }
      }
    } else {
      return {
        error: 'incomplete',
        buffer: buffer
      };
    }
  },
  parse: function parse(bytes, offset, type, strlen) {
      switch(type) {
        case 'string':
          return bytes.toString('utf8', offset, strlen);
        case 'uint8lu':
        case 'uint8':
          return bytes.readUIntLE(offset, 1);
        case 'uint16lu':
        case 'uint16':
          return bytes.readUIntLE(offset, 2);
        case 'uint32lu':
        case 'uint32':
          return bytes.readUIntLE(offset, 4);
        case 'uint8bu':
          return bytes.readUIntBE(offset, 1);
        case 'uint16bu':
          return bytes.readUIntBE(offset, 2);
        case 'uint32bu':
          return bytes.readUIntBE(offset, 4);
        default:
          throw Error('Parse failed. Offset: ', offset, ' Bytes: ', bytes, ' Type: ', type);
      }
    },
    pad: function(string) {
      return '0x' + ((Array(32).join('0') + string).slice(-32)).toUpper();
    },
    packet: {
      magic_string: '',
      dst: 0,
      src: 0,
      type: 0,
      id: 0,
      seq: 0,
      features: 0,
      payload_length: 0,
      payload: '',
      checksum: 0,
      validated: false
    }
}
