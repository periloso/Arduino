/*
  STM32_UUID - Library for getting the UUID of the STM32 chip
  Created by Alessio Periloso, October 14, 2015
  Released under GNU License v2
*/

#ifndef ChipUUID.h
  #include <Arduino.h>
  #include "STM32_UUID.h"

  class ChipUUID {
    public:
      uint32_t asIntArray[3];
      char     asChars[12];
      char     asHex[25];

      ChipUUID();
    private:
  };
#endif