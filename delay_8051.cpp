#include "delay_8051.h"
void delay_ms( unsigned long int ms)
  {
    unsigned long int i=0;
    ms= ms*110;
    for (i=0;i< ms ; i++) ;
  }
void delay_us()
  {
//    unsigned long int i=0;
//   us=us*1;
// i = i + 1;
  }
