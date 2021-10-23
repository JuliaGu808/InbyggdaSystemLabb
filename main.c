#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"

#define F_CPU 16000000
#define BLINK_DELAY_MS 500

int main(void)
{
   DDRB |= 0b000001; // PIN 8

   uart_init();

   while (1)
   {
      // turn LED on
      PORTB |= 0b000001;
      _delay_ms(BLINK_DELAY_MS);

      // turn LED off
      PORTB &= ~0b000001;
      _delay_ms(BLINK_DELAY_MS);

      uart_putstr("Julia\n");
      _delay_ms(BLINK_DELAY_MS);

      uart_echo();
      _delay_ms(BLINK_DELAY_MS);

   }
}
