/*
This file is part of Xmas2013.

Xmas2013 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Xmas2013 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Xmas2013.  If not, see <http://www.gnu.org/licenses/>.
*/
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
