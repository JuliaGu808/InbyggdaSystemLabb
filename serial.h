#ifndef SERIAL_H_
#define SERIAL_H_

void uart_init(void);

void uart_putchar(char chr);
void uart_putstr(const char *str);

char uart_getchar(void);

void uart_echo(void);

void USART_TransmitPolling(uint8_t DataByte);

void uart_led_input(void);
void uart_check_led(const char *str);

#endif

