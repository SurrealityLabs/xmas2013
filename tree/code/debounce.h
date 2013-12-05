#ifndef RBG_XMAS_DEBOUNCE
#define RBG_XMAS_DEBOUNCE

#include <avr/io.h>
#include <stdint.h>
#include "hardware_setup.h"

#define DEBOUNCE_IDLE 0
#define DEBOUNCE_WAITLOW 1
#define DEBOUNCE_WAITHIGH 2
#define DEBOUNCE_ENDHIGH 3

#define DEBOUNCE_THRESHOLD 5

uint8_t debounceButton(void);
inline void registerDebounceCount(void);
inline uint8_t getDebounceState(void);

#endif
