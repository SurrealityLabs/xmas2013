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

#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "hardware_setup.h"
#include "debounce.h"
#include "charlieplex.h"
#include "xmas_common.h"
#include "xmas_boardspecific.h"

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
            xmasModeSwitcher(mode);
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
    registerDebounceCount();
}

ISR(PCINT0_vect) {

}
