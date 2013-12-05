#include "xmas_boardspecific.h"
#include "xmas_common.h"
#include "charlieplex.h"
#include "debounce.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void candycaneAlternate(uint16_t alternateInteval);
void candycaneMarquee(uint16_t marqueeInterval);
void candycaneFillShort(uint16_t fillInterval);
void candycaneFillLong(uint16_t fillInterval);

void candycaneAlternate(uint16_t alternateInteval) {
    cli();
    setAll(0);

    if(frameCount >= (2 * alternateInteval)) {
        frameCount = 0;
    } else if(frameCount >= (alternateInteval)) {
        charlieArray[1] = 1;
        charlieArray[3] = 1;
        charlieArray[5] = 1;
        charlieArray[7] = 1;
        charlieArray[9] = 1;
        charlieArray[11] = 1;
        charlieArray[13] = 1;
        charlieArray[15] = 1;
    } else  {
        charlieArray[0] = 1;
        charlieArray[2] = 1;
        charlieArray[4] = 1;
        charlieArray[6] = 1;
        charlieArray[8] = 1;
        charlieArray[10] = 1;
        charlieArray[12] = 1;
        charlieArray[14] = 1;
    }
    sei();
}

void candycaneMarquee(uint16_t marqueeInterval) {
    cli();
    setAll(0);

    if(frameCount >= (4 * marqueeInterval)) {
        frameCount = 0;
    } else if(frameCount >= (3 * marqueeInterval)) {
        charlieArray[3] = 1;
        charlieArray[7] = 1;
        charlieArray[11] = 1;
        charlieArray[15] = 1;
    } else if(frameCount >= (2 * marqueeInterval)) {
        charlieArray[2] = 1;
        charlieArray[6] = 1;
        charlieArray[10] = 1;
        charlieArray[14] = 1;
    } else if(frameCount >= (marqueeInterval)) {
        charlieArray[1] = 1;
        charlieArray[5] = 1;
        charlieArray[9] = 1;
        charlieArray[13] = 1;
    } else  {
        charlieArray[0] = 1;
        charlieArray[4] = 1;
        charlieArray[8] = 1;
        charlieArray[12] = 1;
    }
    sei();
}

void candycaneFillShort(uint16_t fillInterval) {
    cli();
    setAll(0);
    
    switch(frameCount / fillInterval) {
        case 23:
        case 22:
        case 21:
        case 20:
        case 19:
        case 18:
        case 17:
        case 16:
        case 15:
            charlieArray[15] = 1;
        case 14:
            charlieArray[14] = 1;
        case 13:
            charlieArray[13] = 1;
        case 12:
            charlieArray[12] = 1;
        case 11:
            charlieArray[11] = 1;
        case 10:
            charlieArray[10] = 1;
        case 9:
            charlieArray[9] = 1;
        case 8:
            charlieArray[8] = 1;
        case 7:
            charlieArray[7] = 1;
        case 6:
            charlieArray[6] = 1;
        case 5:
            charlieArray[5] = 1;
        case 4:
            charlieArray[4] = 1;
        case 3:
            charlieArray[3] = 1;
        case 2:
            charlieArray[2] = 1;
        case 1:
            charlieArray[1] = 1;
        case 0:
            charlieArray[0] = 1;
            break;
        default:
            frameCount = 0;
    }
    sei();
}

void candycaneFillLong(uint16_t fillInterval) {
    cli();
    setAll(1);

    switch(frameCount / fillInterval) {
        case 0:
            charlieArray[15] = 0;
        case 1:
            charlieArray[14] = 0;
        case 2:
            charlieArray[13] = 0;
        case 3:
            charlieArray[12] = 0;
        case 4:
            charlieArray[11] = 0;
        case 5:
            charlieArray[10] = 0;
        case 6:
            charlieArray[9] = 0;
        case 7:
            charlieArray[8] = 0;
        case 8:
            charlieArray[7] = 0;
        case 9:
            charlieArray[6] = 0;
        case 10:
            charlieArray[5] = 0;
        case 11:
            charlieArray[4] = 0;
        case 12:
            charlieArray[3] = 0;
        case 13:
            charlieArray[2] = 0;
        case 14:
            charlieArray[1] = 0;
        case 15:
            charlieArray[0] = 0;
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            break;
        default:
            frameCount = 0;
    }
    sei();
}


void xmasModeSwitcher(uint8_t mode) {
    switch(mode) {
        case 0:
            allOff();
            PORTB &= 0b11100000;
            if(getDebounceState() == DEBOUNCE_IDLE) doSleep();
            break;
        case 1:
            allOn();
            break;
        case 2:
            allBlink(750);
            break;
        case 3:
            candycaneAlternate(750);
            break;
        case 4:
            candycaneFillShort(500);
            break;
        case 5:
            candycaneMarquee(500);
            break;
        case 6:
            allTwinkle(500, 40000);
            break;
        case 7:
            candycaneFillLong(500);
            break;
        default:
            mode = 0;
            break;
    }
    return;
}