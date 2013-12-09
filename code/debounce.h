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
