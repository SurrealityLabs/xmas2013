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
#include "debounce.h"

volatile uint8_t debounceCount = 0;

uint8_t debounceState = DEBOUNCE_IDLE;

uint8_t debounceButton(void) {
    if(debounceState == DEBOUNCE_IDLE) {
        // If button is low, reset the debounceCount
        // When it overflows, we'll check if it's still low
        if(!(PINB & (1 << BUTTON))) {
            debounceCount = 0;
            debounceState = DEBOUNCE_WAITLOW;
        }
    } else if(debounceState == DEBOUNCE_WAITLOW) {
        // If we've overflowed, and the input is still low, now we look for it to go high
        if(debounceCount >= DEBOUNCE_THRESHOLD) {
            if(!(PINB & (1 << BUTTON))) {
                debounceCount = 0;
                debounceState = DEBOUNCE_WAITHIGH;
            }
        }
    } else if(debounceState == DEBOUNCE_WAITHIGH) {
        // Once it goes high, we wait a bit to see if it stays high
        if((PINB & (1 << BUTTON))) {
            debounceCount = 0;
            debounceState = DEBOUNCE_ENDHIGH;
        }
    } else if(debounceState == DEBOUNCE_ENDHIGH) {
        // If it's stayed high, we reset and register that the button was pushed
        if(debounceCount >= DEBOUNCE_THRESHOLD) {
            if((PINB & (1 << BUTTON))) {
                debounceCount = 0;
                debounceState = DEBOUNCE_IDLE;
                return 1;
            }
        }
    } else {
        debounceState = DEBOUNCE_IDLE;
    }
    return 0;
}

inline void registerDebounceCount(void) {
    debounceCount++;
    return;
}

inline uint8_t getDebounceState(void) {
    return debounceState;
}
