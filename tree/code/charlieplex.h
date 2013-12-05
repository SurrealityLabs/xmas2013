#ifndef RBG_XMAS_CHARLIEPLEX
#define RBG_XMAS_CHARLIEPLEX

#include <stdint.h>
#include "hardware_setup.h"

inline void writeCharlieplexLED(void);
extern volatile uint8_t charlieArray[20];
extern volatile uint16_t frameCount;

#endif
