#ifndef RBG_XMAS_COMMON
#define RBG_XMAS_COMMON

#include <stdint.h>
#include "hardware_setup.h"
#include "charlieplex.h"

void doSleep(void);
void setAll(uint8_t state);
void allOff(void);
void allOn(void);
void allBlink(uint16_t blinkInterval);
void allTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold);
uint16_t getRandom(void);

#endif
