#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "serial.h"

#define USART_BAUDRATE 38400 // Desired Baud Rate
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0 << UMSEL00) // USART Mode Selection

#define DISABLED (0 << UPM00)
#define EVEN_PARITY (2 << UPM00)
#define ODD_PARITY (3 << UPM00)
#define PARITY_MODE DISABLED // USART Parity Bit Selection

#define ONE_BIT (0 << USBS0)
#define TWO_BIT (1 << USBS0)
#define STOP_BIT ONE_BIT // USART Stop Bit Selection

#define FIVE_BIT (0 << UCSZ00)
#define SIX_BIT (1 << UCSZ00)
#define SEVEN_BIT (2 << UCSZ00)
#define EIGHT_BIT (3 << UCSZ00)
#define DATA_BIT EIGHT_BIT // USART Data Bit Selection

void uart_init(void)
{
    // Set Baud Rate
    UBRR0H = BAUD_PRESCALER >> 8;
    UBRR0L = BAUD_PRESCALER;

    // Set Frame Format
    UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;

    // Enable Receiver and Transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_putchar(char chr)
{
    char line = '\n';
    if (chr == line)
    {

        USART_TransmitPolling('\r');
        USART_TransmitPolling('\n');
    }
    else if (chr == '\r') // "enter" på tangentbord == '\r'
    {
        USART_TransmitPolling('\r');
        USART_TransmitPolling('\n');
    }
    else
    {
        USART_TransmitPolling(chr);
    }
}

void uart_putstr(const char *str)
{

    char last_char = str[strlen(str) - 1];
    for (int i = 0; i < strlen(str) - 1; i++)
    {
        USART_TransmitPolling(str[i]);
    }
    uart_putchar(last_char);
}

void USART_TransmitPolling(uint8_t DataByte)
{
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    }; // Do nothing until UDR is ready
    UDR0 = DataByte;
}

void uart_echo(void)
{
    char LocalData = uart_getchar();
    uart_putchar(LocalData);
}

char uart_getchar(void)
{
    uint8_t DataByte;
    while ((UCSR0A & (1 << RXC0)) == 0)
    {
    }; // Do nothing until data have been received
    DataByte = UDR0;
    return DataByte;
}