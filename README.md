# Interrupt Based Timer for ATmega328P

This project is an example of an interrupt based timer for the commonly used ATmega328P microcontroller.

## Why use interrupt based timers?
There is two main approaches when it comes to delaying something in embedded systems. This project is 
an example of interrupt based waiting, but I will explain both options with examples.

### 1. Busy Waiting - *the blocking selfish way*
The selfish and the easy way of delaying something is just blocking the CPU with some unnecessary work,
usually by creating loops that accomplish nothing but look like they do to the compiler. If compiler
realises that your new shiny loop is not doing anything useful it will just throw it out in the 
optimization phase. You can always disable or change the optimization level, but I will just assume that 
you are using the default level of optimization.

An example to this can be; assigning or reading a value from a ***volatile*** variable over and over.
The below code will block the CPU for however long it takes for the CPU to process that operation for 100
times. 

```c
#define SOMEVAR (*(volatile uint8_t *) (0x80)) // Define a volatile variable

for (int i = 0; i < 100; ++i) { 
    // Set a value to the volatile variable. If this is not volatile, the compiler will run this only once and 
    // optimize out the rest of it to save time and resources since it basically does nothing. But because it
    // is volatile it has no way to know if this value will be the same since declaring as volatile means that
    // this value can change any time by a factor that is not known to the compiler.
    SOMEVAR = 0x00;
}
```
If manually implemented this option requires the developer to perform some calculations in order to
understand how long this delay will take, or you can always do trial and error to guesstimate the time 
it takes and try to get it to a value close to what you are looking for but that's going to take some time
going back and forth and will require an oscilloscope to get it at least close to the value you looking for
(no, you can't use your phone's timer).

The `delay(ms)` function that all of us know from the Arduino lib is an example of busy waiting.

### 2. Interrupt Based Waiting - *non blocking way*
Another and arguably the better way of waiting for something is using timers with interrupts to free the CPU 
from the waiting task. This works as in that when the timer is triggered it will go ahead and interrupt the
CPU from whatever it is doing and will ask it to do what it wants immediately. For more details about how 
interrupts work, visit the 
[website of University of Texas at Austin.](https://users.ece.utexas.edu/~valvano/Volume1/E-Book/C12_Interrupts.htm)

An example code snippet for an interrupt based timer using the 16-bit Timer 1 of ATmega328P can be found below.
```c
ISR (TIMER1_OVF_vect) {
    PORTB ^= (1 << PORTB5); // Toggle the 5th data register of PORTB
    TCNT1 = 49911; // 50 ms for 16MHz clock
}

int main() {
    DDRB = (1 << DDB5); // Set 5th data direction register of PORTB. A set value means output
    TCNT1 = 49911; // 50 ms for 16MHz clock
    TCCR1A = 0x00; // Set normal counter mode
    TCCR1B = (1<<CS10) | (1<<CS12); // Set 1024 pre-scaler
    TIMSK1 = (1 << TOIE1); // Set overflow interrupt enable bit
    sei(); // Enable interrupts globally
    while(1)
    {
        // Do anything, this timer is non-blocking. It will interrupt the CPU only when needed
    }
    // Add return so compilers don't cry about it being missing.
    // Under normal circumstances this will never be hit
    return 0;
}
```

This creates a non-blocking interrupt based timer that will toggle the 5th data register of PORTB every 50ms.
But how do we calculate how long we need to wait? Unless we need to change the wait time in the runtime we should 
pre-compute the value for `TCNT1` which is in basic terms what determines the wait time.

## Calculate the wait time
WIP