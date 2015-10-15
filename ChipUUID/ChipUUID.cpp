/*
  STM32_UUID - Library for getting the UUID of the STM32 chip
  Created by Alessio Periloso, October 14, 2015
  Released under GNU License v2
*/

#include <Arduino.h>
#include "STM32_UUID.h"
#include "ChipUUID.h"

ChipUUID::ChipUUID() {

  // Get the ID from the chip and save to int_id
  memcpy(&asIntArray, __STM32_UUID__, 12);

  // Rotate the int_id to make it little-endian
  for (char i=0; i<3; i++) {
    asChars[i*4] = (asIntArray[i] >> 24) & 0xff;
    asChars[i*4+1] = (asIntArray[i] >> 16) & 0xff;
    asChars[i*4+2] = (asIntArray[i] >> 8) & 0xff;
    asChars[i*4+3] = asIntArray[i] & 0xff;
  }

  // Create the HEX
  const static unsigned char hexcodes[] = "0123456789ABCDEF";
  for (char i=0; i<12; i++) {
    asHex[i*2] = hexcodes[(uint8_t)(asChars[i] >> 4)];
    asHex[i*2+1] = hexcodes[(uint8_t)(asChars[i] & 0x0f)];
  }
  asHex[24] = 0;
}