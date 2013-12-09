#include "xmas_boardspecific.h"
#include "xmas_common.h"
#include "charlieplex.h"
#include "debounce.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void treeTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold);
void treeMarquee(uint16_t marqueeInterval);
void treeRightFill(uint16_t fillInterval);
void treeBottomFill(uint16_t fillInterval);

void treeTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold) {
    cli();
    allTwinkle(twinkleInterval, randomThreshold);
    charlieArray[0] = 1;
    sei();
}

void treeMarquee(uint16_t marqueeInterval) {
    cli();
    setAll(0);

    charlieArray[0] = 1;

    if(frameCount >= (3 * marqueeInterval)) {
        frameCount = 0;
    } else if(frameCount >= (2 * marqueeInterval)) {
        charlieArray[3] = 1;
        charlieArray[6] = 1;
        charlieArray[9] = 1;
        charlieArray[12] = 1;
        charlieArray[15] = 1;
    } else if(frameCount >= (marqueeInterval)) {
        charlieArray[2] = 1;
        charlieArray[5] = 1;
        charlieArray[8] = 1;
        charlieArray[11] = 1;
        charlieArray[14] = 1;
    } else  {
        charlieArray[1] = 1;
        charlieArray[4] = 1;
        charlieArray[7] = 1;
        charlieArray[10] = 1;
        charlieArray[13] = 1;
    }
    sei();
}

void treeRightFill(uint16_t fillInterval) {
    cli();
    setAll(0);
    
    charlieArray[0] = 1;

    switch(frameCount / fillInterval) {
        case 10:
        case 9:
        case 8:
        case 7:
        case 6:
            charlieArray[15] = 1;
        case 5:
            charlieArray[14] = 1;
            charlieArray[9] = 1;
        case 4:
            charlieArray[13] = 1;
            charlieArray[8] = 1;
            charlieArray[4] = 1;
        case 3:
            charlieArray[12] = 1;
            charlieArray[7] = 1;
            charlieArray[3] = 1;
        case 2:
            charlieArray[11] = 1;
            charlieArray[6] = 1;
            charlieArray[2] = 1;
        case 1:
            charlieArray[10] = 1;
            charlieArray[5] = 1;
            charlieArray[1] = 1;
        case 0:
            break;
        default:
            frameCount = 0;
    }
    sei();
}

void treeBottomFill(uint16_t fillInterval) {
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
            treeMarquee(500);
            break;
        case 4:
            treeRightFill(500);
            break;
        case 5:
            treeBottomFill(300);
            break;
        case 6:
            allTwinkle(500, 40000);
            break;
        case 7:
            treeTwinkle(500, 40000);
            break;
        default:
            mode = 0;
            break;
    }
    return;
}