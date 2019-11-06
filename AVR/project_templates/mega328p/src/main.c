#include <avr/io.h>
#include <util/delay.h>

#include "main.h"

#define LED_PIN  PB5
#define LED_DDR  DDRB
#define LED_PORT PORTB

#define DELAY 1000

int main(void)
{
    set_output(LED_PORT, LED_PIN);

    while ( true ) {
        sbi(LED_PORT, LED_PIN); // OUTPUT to HIGH
        _delay_ms(DELAY);

        cbi(LED_PORT, LED_PIN); // OUTPUT to LOW
        _delay_ms(DELAY);
    }
}
