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

    uint8_t maxCount, i;
    uint16_t temp;

    temp = frameCount / fillInterval;
    if(temp > 31) {
        frameCount = 0;
        maxCount = 16;
    } else if(temp > 15) {
        maxCount = 16;
    } else {
        maxCount = temp;
    }

    for(i = 0; i < maxCount; i++) {
        charlieArray[i] = 1;
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
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
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
            candycaneFillShort(250);
            break;
        case 5:
            candycaneMarquee(500);
            break;
        case 6:
            allTwinkle(300, 40000);
            break;
        case 7:
            candycaneFillLong(250);
            break;
        default:
            mode = 0;
            break;
    }
    return;
}
