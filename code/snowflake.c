#include "xmas_boardspecific.h"
#include "xmas_common.h"
#include "charlieplex.h"
#include "debounce.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void snowflakeAlternate(uint16_t alternateInteval);
void snowflakeStarburst(uint16_t fillInterval);
void snowflakePinwheel(uint16_t marqueeInterval);
void snowflakeSpiralFill(uint16_t fillInterval);

void snowflakeAlternate(uint16_t alternateInteval) {
    cli();
    setAll(0);

    if(frameCount >= (2 * alternateInteval)) {
        frameCount = 0;
    } else if(frameCount >= (alternateInteval)) {
        charlieArray[1] = 1;
        charlieArray[4] = 1;
        charlieArray[7] = 1;
        charlieArray[10] = 1;
        charlieArray[13] = 1;
        charlieArray[16] = 1;
        charlieArray[18] = 1;
    } else  {
        charlieArray[0] = 1;
        charlieArray[2] = 1;
        charlieArray[3] = 1;
        charlieArray[5] = 1;
        charlieArray[6] = 1;
        charlieArray[8] = 1;
        charlieArray[9] = 1;
        charlieArray[11] = 1;
        charlieArray[12] = 1;
        charlieArray[14] = 1;
        charlieArray[15] = 1;
        charlieArray[17] = 1;
    }
    sei();
}

void snowflakeStarburst(uint16_t fillInterval) {
    cli();
    setAll(0);
    
    switch(frameCount / fillInterval) {
        case 7:
        case 6:
        case 5:
        case 4:
        case 3:
            charlieArray[0] = 1;
            charlieArray[3] = 1;
            charlieArray[6] = 1;
            charlieArray[9] = 1;
            charlieArray[12] = 1;
            charlieArray[15] = 1;
        case 2:
            charlieArray[1] = 1;
            charlieArray[4] = 1;
            charlieArray[7] = 1;
            charlieArray[10] = 1;
            charlieArray[13] = 1;
            charlieArray[16] = 1;
        case 1:
            charlieArray[2] = 1;
            charlieArray[5] = 1;
            charlieArray[8] = 1;
            charlieArray[11] = 1;
            charlieArray[14] = 1;
            charlieArray[17] = 1;
        case 0:
            charlieArray[18] = 1;
            break;
        default:
            frameCount = 0;
    }
    sei();
}

void snowflakePinwheel(uint16_t marqueeInterval) {
    cli();
    setAll(0);

    charlieArray[18] = 1;

    if(frameCount >= (3 * marqueeInterval)) {
        frameCount = 0;
    } else if(frameCount >= (2 * marqueeInterval)) {
        charlieArray[6] = 1;
        charlieArray[7] = 1;
        charlieArray[8] = 1;
        charlieArray[15] = 1;
        charlieArray[16] = 1;
        charlieArray[17] = 1;
    } else if(frameCount >= (marqueeInterval)) {
        charlieArray[3] = 1;
        charlieArray[4] = 1;
        charlieArray[5] = 1;
        charlieArray[12] = 1;
        charlieArray[13] = 1;
        charlieArray[14] = 1;
    } else  {
        charlieArray[0] = 1;
        charlieArray[1] = 1;
        charlieArray[2] = 1;
        charlieArray[9] = 1;
        charlieArray[10] = 1;
        charlieArray[11] = 1;
    }
    sei();
}

void snowflakeSpiralFill(uint16_t fillInterval) {
    cli();
    setAll(0);
    
    switch(frameCount / fillInterval) {
        case 11:
        case 10:
        case 9:
        case 8:
        case 7:
        case 6:
            charlieArray[15] = 1;
            charlieArray[16] = 1;
            charlieArray[17] = 1;
        case 5:
            charlieArray[12] = 1;
            charlieArray[13] = 1;
            charlieArray[14] = 1;
        case 4:
            charlieArray[9] = 1;
            charlieArray[10] = 1;
            charlieArray[11] = 1;
        case 3:
            charlieArray[6] = 1;
            charlieArray[7] = 1;
            charlieArray[8] = 1;
        case 2:
            charlieArray[3] = 1;
            charlieArray[4] = 1;
            charlieArray[5] = 1;
        case 1:
            charlieArray[0] = 1;
            charlieArray[1] = 1;
            charlieArray[2] = 1;
        case 0:
            charlieArray[18] = 1;
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
            snowflakeAlternate(750);
            break;
        case 4:
            snowflakeStarburst(500);
            break;
        case 5:
            snowflakeSpiralFill(500);
            break;
        case 6:
            snowflakePinwheel(500);
            break;
        case 7:
            allTwinkle(500, 40000);
            break;
        default:
            mode = 0;
            break;
    }
    return;
}