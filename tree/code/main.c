#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define OUTPUT_0 2
#define OUTPUT_1 3
#define OUTPUT_2 4
#define OUTPUT_3 1
#define OUTPUT_4 0
#define BUTTON 5

volatile uint8_t charlieArray[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile uint16_t frameCount = 0;

inline void writeCharlieplexLED(void);

volatile uint8_t debounceCount = 0;

#define DEBOUNCE_IDLE 0
#define DEBOUNCE_WAITLOW 1
#define DEBOUNCE_WAITHIGH 2
#define DEBOUNCE_ENDHIGH 3

#define DEBOUNCE_THRESHOLD 5

uint8_t debounceState = DEBOUNCE_IDLE;

uint8_t debounceButton() {
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

void doSleep() {
    TIMSK = (0 << OCIE1A) | (0 << OCIE1B) | (0 << OCIE0A) | (0 << OCIE0B) | (0 << TOIE1) | (0 << TOIE0);
    PCMSK = (1 << BUTTON);
    GIMSK = (1 << PCIE);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    uint8_t mcucr1 = MCUCR | (1 << BODS) | (1 << BODSE);
    uint8_t mcucr2 = mcucr1 & ~(1 << BODSE);

    cli();
    MCUCR = mcucr1;
    MCUCR = mcucr2;

    sei();
    sleep_cpu();
    cli();

    sleep_disable();
    TIMSK = (0 << OCIE1A) | (0 << OCIE1B) | (1 << OCIE0A) | (0 << OCIE0B) | (1 << TOIE1) | (0 << TOIE0);
    GIMSK = (0 << PCIE);
    sei();
}


void setAll(uint8_t state);
void allOff(void);
void allOn(void);
void allBlink(uint16_t blinkInterval);
void allTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold);
uint16_t getRandom(void);

void treeTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold);
void treeMarquee(uint16_t marqueeInterval);
void treeRightFill(uint16_t fillInterval);
void treeBottomFill(uint16_t fillInterval);

void setAll(uint8_t state) {
    uint8_t i = 0;
    for(i=0; i<20; i++) {
        charlieArray[i] = state;
    }
}

void allOff(void) {
    cli();
    setAll(0);
    frameCount = 0;
    sei();
}

void allOn(void) {
    cli();
    setAll(1);
    frameCount = 0;
    sei();
}

void allBlink(uint16_t blinkInterval) {
    cli();
    if(frameCount >= (2 * blinkInterval)) {
        frameCount = 0;
    } else if(frameCount >= blinkInterval) {
        setAll(1);
    } else {
        setAll(0);
    }
    sei();
}

void allTwinkle(uint16_t twinkleInterval, uint16_t randomThreshold) {
    uint8_t i = 0;
    cli();
    if(frameCount >= twinkleInterval) {
        frameCount = 0;

        for(i=0; i<20; i++) {
            if(getRandom() > randomThreshold) {
                if(charlieArray[i] == 1) {
                    charlieArray[i] = 0;
                } else {
                    charlieArray[i] = 1;
                }
            }
        }
    }
    sei();
}

uint16_t getRandom() {
    static uint16_t LFSR = 0x1234u;

    uint16_t bit = ((LFSR>>0) ^ (LFSR>>2) ^ (LFSR>>3) ^ (LFSR>>5)) & 1;
    LFSR = (bit << 15) | (LFSR >> 1);
    return LFSR;
}

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

int main(void) {
    DDRB = 0b00011111;
    PORTB = 0b00000000;
    DIDR0 = 0;
    ACSR |= (1 << ACD);
    ADCSRA &= ~(1 << ADEN);

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

    uint8_t mode = 1;
    uint16_t lastFrameCount = 0;

    setAll(0);
    
    while(1) {
        if(lastFrameCount != frameCount) {
            switch(mode) {
                case 0:
                    allOff();
                    PORTB &= 0b11100000;
                    if(debounceState == DEBOUNCE_IDLE) doSleep();
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
            lastFrameCount = frameCount;
            if(debounceButton()) {
                mode++;
                mode %= 8;
                frameCount = 0;
            }
        }
    }

    return 0;
}

ISR(TIM0_COMPA_vect) {
    writeCharlieplexLED();
}

ISR(TIM1_OVF_vect) {
    frameCount++;
    debounceCount++;
}

ISR(PCINT0_vect) {

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