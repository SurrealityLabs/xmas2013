#include "charlieplex.h"
#include <avr/io.h>

volatile uint8_t charlieArray[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile uint16_t frameCount = 0;

inline void writeCharlieplexLED(void) {
    static uint8_t currentLED = 0;

    DDRB = 0b00000000;
    PORTB = 0b00000000;
    switch(currentLED) {
        case 0:
        case 1:
        case 2:
        case 3:
            DDRB |= ((1 << OUTPUT_0));
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            DDRB |= ((1 << OUTPUT_1));
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            DDRB |= ((1 << OUTPUT_2));
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            DDRB |= ((1 << OUTPUT_3));
            break;
        case 16:
        case 17:
        case 18:
        case 19:
            DDRB |= ((1 << OUTPUT_4));
            break;
    }

    if(charlieArray[currentLED] == 1) {
        switch(currentLED) {
            case 4:
            case 8:
            case 12:
            case 16:
                DDRB |= ((1 << OUTPUT_0));
                PORTB = ((1 << OUTPUT_0));
                break;
            case 0:
            case 9:
            case 13:
            case 17:
                DDRB |= ((1 << OUTPUT_1));
                PORTB = ((1 << OUTPUT_1));
                break;
            case 1:
            case 5:
            case 14:
            case 18:
                DDRB |= ((1 << OUTPUT_2));
                PORTB = ((1 << OUTPUT_2));
                break;
            case 2:
            case 6:
            case 10:
            case 19:
                DDRB |= ((1 << OUTPUT_3));
                PORTB = ((1 << OUTPUT_3));
                break;
            case 3:
            case 7:
            case 11:
            case 15:
                DDRB |= ((1 << OUTPUT_4));
                PORTB = ((1 << OUTPUT_4));
                break;
        }
    }

    currentLED++;
    if(currentLED > 19) currentLED = 0;
}
