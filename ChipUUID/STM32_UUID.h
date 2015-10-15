#include <Arduino.h>

#ifndef STM32_UUID_h
  #if defined(__STM32F1__)
    #define __STM32_UUID__ ((uint32_t *)0x1FFFF7E8)
  #elif (defined(__STM32F0__) || defined(__STM32F4__))
    #define __STM32_UUID__ ((uint32_t*) 0x1FFF7A10)
  #endif
#endif
