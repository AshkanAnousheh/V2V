#ifndef cc2530_usart_h
#define cc2530_usart_h

#include "types.h"

extern void cc2530_uart0_init();
extern void uart0_sendbuf(uint8_t *buff, char len);

#endif