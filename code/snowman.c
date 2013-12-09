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

void snowmanTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold);
void snowmanMarquee(uint16_t marqueeInterval);
void snowmanMarquee2(uint16_t marqueeInterval);
void snowmanFill(uint16_t fillInterval);

void snowmanTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold) {
    cli();
    allTwinkle(twinkleInterval, randomThreshold);
    charlieArray[0] = 1;
    sei();
}

void snowmanMarquee(uint16_t marqueeInterval) {
    cli();
    setAll(0);

    charlieArray[0] = 1;

    if(frameCount >= (3 * marqueeInterval)) {
        frameCount = 0;
    } else if(frameCount >= (2 * marqueeInterval)) {
        charlieArray[3] = 1;
        charlieArray[6] = 1;
        charlieArray[9] = 1;
    } else if(frameCount >= (marqueeInterval)) {
        charlieArray[2] = 1;
        charlieArray[5] = 1;
        charlieArray[8] = 1;
    } else  {
        charlieArray[1] = 1;
        charlieArray[4] = 1;
        charlieArray[7] = 1;
    }
    sei();
}

void snowmanMarquee2(uint16_t marqueeInterval) {
    cli();
    setAll(0);

    charlieArray[0] = 1;
    charlieArray[7] = 1;
    charlieArray[8] = 1;
    charlieArray[9] = 1;

    if(frameCount >= (3 * marqueeInterval)) {
        frameCount = 0;
    } else if(frameCount >= (2 * marqueeInterval)) {
        charlieArray[3] = 1;
        charlieArray[6] = 1;
    } else if(frameCount >= (marqueeInterval)) {
        charlieArray[2] = 1;
        charlieArray[5] = 1;
    } else  {
        charlieArray[1] = 1;
        charlieArray[4] = 1;
    }
    sei();
}

void snowmanFill(uint16_t fillInterval) {
    cli();
    setAll(0);
    
    charlieArray[0] = 1;
    charlieArray[7] = 1;
    charlieArray[8] = 1;
    charlieArray[9] = 1;

    switch(frameCount / fillInterval) {
        case 10:
        case 9:
        case 8:
        case 7:
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
            snowmanMarquee(500);
            break;
        case 4:
            snowmanMarquee2(500);
            break;
        case 5:
            allTwinkle(500, 40000);
            break;
        case 6:
            snowmanTwinkle(500, 40000);
            break;
        case 7:
            snowmanFill(500);
            break;
        default:
            mode = 0;
            break;
    }
    return;
}
