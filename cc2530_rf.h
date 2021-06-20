#ifndef cc2530_rf_h
#define cc2530_rf_h

#include "types.h"

extern void cc2530_rf_init();
extern void cc2530_rf_send(uint8_t *pbuf , int len);

#endif