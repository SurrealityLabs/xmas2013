#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define OUTPUT_0 2
#define OUTPUT_1 3
#define OUTPUT_2 4
#define OUTPUT_3 1
#define OUTPUT_4 0
#define BUTTON 5

volatile uint8_t charlieArray[20];
volatile uint16_t frameCount = 0;

inline void writeCharlieplexLED(void);

int main(void) {
    DDRB = 0b00111111;
    PORTB = 0b00000000;

    //Timer0 set for 60x20 = 1200 Hz for charlieplex refresh
    TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (1 << WGM01) | (0 << WGM00);
    TCCR0B = (0 << FOC0A) | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (1 << CS01) | (1 << CS00);
    OCR0A = 0x48;
    TCNT0 = 0;

    TCCR1 = (1 << CTC1) | (1 << PWM1A) | (0 << COM1A1) | (0 << COM1A0) | (0 << CS13) | (1 << CS12) | (1 << CS11) | (1 << CS10);
    GTCCR = (0 << PWM1B) | (0 << COM1B1) | (0 << COM1B0) | (0 << FOC1B) | (0 << FOC1A) | (0 << PSR1);
    OCR1C = 0x7D;
    TCNT1 = 0;

    TIMSK = (0 << OCIE1A) | (0 << OCIE1B) | (1 << OCIE0A) | (0 << OCIE0B) | (1 << TOIE1) | (0 << TOIE0);
    sei();

    uint8_t i = 0;

    for(i=0; i<20; i++) {
        charlieArray[i] = 0;
    }
    
    while(1) {
        for(i=0; i<20; i++) {
            charlieArray[i] = 1;
            frameCount = 0;
            TCNT1 = 0;
            while(frameCount < 1000);
        }

        for(i=20; i>0; i--) {
            charlieArray[20-i] = 0;
            frameCount = 0;
            TCNT1 = 0;
            while(frameCount < 1000);
        }
    }

    return 0;
}

ISR(TIM0_COMPA_vect) {
    writeCharlieplexLED();
}

ISR(TIM1_OVF_vect) {
    frameCount++;
}

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