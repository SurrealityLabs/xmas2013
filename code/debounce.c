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
