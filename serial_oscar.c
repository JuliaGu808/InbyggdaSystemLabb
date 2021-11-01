#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

#define BAUD 38400
#define FOSC 16000000

void uart_init(void){
    //Enable transmitter
    UCSR0B |= _BV(TXEN0);

    //Enable Receiver
    UCSR0B |= _BV(RXEN0);

    //8N1 Mode: (Controlled by UCSR0C register, see datasheet ch. 24.12.4 p. 249-250)
    //8-bit data
    //UCSR0C |= _BV(UCSZ01) & ~_BV(UCSZ00); // Detta funkar INTE!!!
    UCSR0C |= _BV(UCSZ01);                  // Utan måste göras i steg
    UCSR0C |= _BV(UCSZ00);
    // No parity
    UCSR0C &= ~_BV(UPM01) & ~_BV(UPM00);    // Detta funkar ==> UCSR0C &= ~(_BV(UPM01) | ~_BV(UPM00))
    //UCSR0C &= ~_BV(UPM01);
    //UCSR0C &= ~_BV(UPM00);
    // 1 stop bit
    UCSR0C &= ~_BV(USBS0);

    // Set baudrate: (see datasheet ch. 24)
    uint16_t ubrrn = FOSC/(16*BAUD) - 1;
    //UBRR0H = ubrrn & 0xFF00; // High bits of 16 bit number
    //UBRR0H = ubrrn & 0x00FF; // Low bits of 16 bit number
    UBRR0H = ubrrn;
}

void uart_putchar(char chr){
    while(!(UCSR0A & (1<<UDRE0))){}; // Wait for empty transmit buffer by checking the...
    UDR0 = chr;         // Put data into buffer, sends the data
    if(chr=='\0'){
        uart_putchar('\r'); // Adds the symbol '\r' (Carriage Return) if the character being...
    }
}

void uart_putstr(const char *str){
    char chr = *str;    // Get the first character
    uart_putchar(chr);  // Send the first character
    while(chr != '\0'){ // Loop until end of string is encountered
        str++;          // Increment the pointer
        chr=*str;       // Get next character
        uart_putchar(chr);  // Send next character
    }
}

char uart_getchar(void){
    // Wait for data to be received
    while(!(UCSR0A & (1<<RXC0))){};
    // Get and return received data from buffer
    return UDR0;
}

void uart_echo(void){
    char chr= uart_getchar();  // Wait for and receive a char from uart
    uart_putchar(chr);         // Return chr over uart
}

void uart_ledctrl(void){
    const char on[] = "ON\r\n";
    const char off[] = "OFF\r\n";
    char msg[5];
    char incoming = 'a';

    for(int i = 0; i<sizeof(msg)&&incoming!='\n'; i++){   //Read until end of line is reach
        incoming= uart_getchar();
        uart_putchar(incoming);   // return chr over uart
        msg[i]=incoming;
    }

    int on_match=0;
    int off_match=0;

    for(int i = 0; i<sizeof(msg); i++){
        if(i<sizeof(on)-1 && msg[i]==on[i]){  // check if current character matches the...
            on_match++;
            if(on_match==sizeof(on)-1){  // complete match with "on", turn on led connection
                PORTB |= (1<<PORTB1);
                break;
            }
        }
        if(i<sizeof(off)-1 && msg[i]==off[i]){  // check if current character matches the...
            off_match++;
            if(off_match==sizeof(off)-1){  // complete match with "off", turn on led connection
                PORTB &= ~(1<<PORTB1);
                break;
            }
        }
    }
    uart_flush();  // flush any possible data left in the incoming buffer
}

void uart_flush(void){
    unsigned char dummy;
    while(UCSR0A & (1<<RXC0)){
        dummy=UDR0;
    }
    (void) dummy;
}

