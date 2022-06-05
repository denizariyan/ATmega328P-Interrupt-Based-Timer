//
// Created by Deniz on 05/06/2022.
//
#include <avr/io.h>
#include <avr/interrupt.h>

ISR (TIMER1_OVF_vect) {
    PORTB ^= (1 << PORTB5);
    TCNT1 = 64755; // 50 ms
}

int main(void) {
    DDRB = (0x01 << DDB5);
    TCNT1 = 64755; // 50 ms
    TCCR1A = 0x00;
    TCCR1B = (1<<CS10) | (1<<CS12);
    TIMSK1 = (1 << TOIE1);
    sei();
    while(1)
    {
        // Do anything, this timer in non-blocking
    }
    // Add return so old compilers don't cry about it being missing. Under normal circumstances this will never be hit
    return 0;
}