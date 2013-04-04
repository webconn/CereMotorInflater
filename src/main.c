#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

int main(void)
{
    // 1. Init input and output
    // PB3 is command input
    // PB4 is relay output

    DDRB |= (1<<4);
    PORTB |= (1<<3); // enable pull-up for PB3

    // Add pinchange interrupt for PB3
    // Active level - low
    // If turns high, start program again
    // So, make the interrupt for low-to-high
    // In interrupt, reset the chip
    PCMSK |= (1<<PCINT3);
    GIMSK |= (1<<PCIE); // enable PC interrupt

    sei(); // enable interrupts

    // Waiting for low signal on input - start to count
    while(PINB & (1<<3));

    // Waiting for 90 seconds
    uint8_t i;
    for(i=0; i<90; i++)
        _delay_ms(1000);

    // Launch relay
    PORTB |= (1<<4);
    // Wait for 10 seconds
    _delay_ms(10000);
    // Shut down relay
    PORTB &= ~(1<<4);

    while(1);;; // need for interrupt

    return 0; // stop program
}

ISR(PCINT0_vect)
{
    if(PINB & (1<<3)) // if high level detected
    {
        // Enable watchdog and reset chip
        wdt_enable(WDTO_15MS);
        _delay_ms(20); // after 15 ms device will reset
    }
}
