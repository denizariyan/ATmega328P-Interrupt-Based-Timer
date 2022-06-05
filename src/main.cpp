//
// Created by Deniz on 05/06/2022.
//
#include <avr/io.h>
#include <avr/interrupt.h>

ISR (TIMER1_OVF_vect) {
    PORTB ^= (1 << PORTB5); // Toggle the 5th data register of PORTB
    TCNT1 = 64755; // 50 ms for 16MHz clock
}

int main() {
    DDRB = (1 << DDB5); // Set 5th data direction register of PORTB. A set value means output
    TCNT1 = 64755; // 50 ms for 16MHz clock
    TCCR1A = 0x00; // Set normal counter mode
    TCCR1B = (1<<CS10) | (1<<CS12); // Set 1024 pre-scaler
    TIMSK1 = (1 << TOIE1); // Set overflow interrupt enable bit
    sei(); // Enable interrupts globally
    while(1)
    {
        // Do anything, this timer is non-blocking. It will interrupt the CPU only when needed
    }
    // Add return so old compilers don't cry about it being missing. Under normal circumstances this will never be hit
    return 0;
}