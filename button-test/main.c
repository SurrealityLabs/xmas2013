#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define OUTPUT_0 2
#define OUTPUT_1 3
#define OUTPUT_2 4
#define OUTPUT_3 1
#define OUTPUT_4 0
#define BUTTON 4

volatile uint16_t frameCount = 0;
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

int main(void) {
    DDRB = 0b00101110;
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

    uint8_t mode = 0;
    
    while(1) {
        switch(mode) {
            case 0:
                PORTB &= 0b11110000;
                PORTB |= 0b00000000;
                if(debounceState == DEBOUNCE_IDLE) doSleep();
                break;
            case 1:
                PORTB &= 0b11110000;
                PORTB |= 0b00000010;
                break;
            case 2:
                PORTB &= 0b11110000;
                PORTB |= 0b00000100;
                break;
            case 3:
                PORTB &= 0b11110000;
                PORTB |= 0b00000110;
                break;
            case 4:
                PORTB &= 0b11110000;
                PORTB |= 0b00001000;
                break;
            case 5:
                PORTB &= 0b11110000;
                PORTB |= 0b00001010;
                break;
            case 6:
                PORTB &= 0b11110000;
                PORTB |= 0b00001100;
                break;
            case 7:
                PORTB &= 0b11110000;
                PORTB |= 0b00001110;
                break;
            default:
                mode = 0;
                break;
        }
        if(debounceButton()) {
            mode++;
            mode %= 8;
        }
    }

    return 0;
}

ISR(TIM0_COMPA_vect) {

}

ISR(TIM1_OVF_vect) {
    frameCount++;
    debounceCount++;
}

ISR(PCINT0_vect) {

}
