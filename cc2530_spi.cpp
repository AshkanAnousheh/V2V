#include "types.h"
#include "cc2530_spi.h"
//---------------------SPI0--------------------//
void spi0_init()
{
   if(ModeSpi0)
   {
     PortSpi0 |= (1<<PinSpi0);  // NSS is OUTPUT
     NssSpi0=1; // NSS set high use for slave select
   }
   
  
  U0GCR = (BAUD_ESpi0)|(ORDERSpi0<<5)|(CPHASpi0<<6)|(CPOLSpi0<<7);
  U0BAUD = BAUD_MSpi0;

  
  
  // --- configure i/o for use peripheral FOR SPI0 ALT2 & priorety
  if(locationSpi0)
  {
    if(ModeSpi0)
      P1SEL |= (1<<3)|(1<<4)|(1<<5); // GPIOs is peripheral
    else
      P1SEL |= (1<<2)|(1<<3)|(1<<4)|(1<<5); // GPIOs is peripheral
    PERCFG |= 1; // USART 0 IN alt 2 
    P2SEL &= ~((1<<6)|(1<<3)); // extesas dadne awlawiat be USART0
  }
  else
  {
    if(ModeSpi0)
      P0SEL |=(1<<2)|(1<<3)|(1<<5);     // GPIOs is peripheral
    else
      P0SEL |=(1<<2)|(1<<3)|(1<<4)|(1<<5);
    PERCFG &=~(1);  // USART 0 IN alt 1
    P2DIR &=~(0xC0); // // extesas dadne awlawiat be USART0 dar alt1
  }
  
  //--- configure usart registers
  U0CSR &= ~(1<<7); // SELECT SPI MODE
  if(ModeSpi0)  // selact slave or master
   U0CSR &= ~(1<<5);
  else
   U0CSR |= (1<<5);
  

  
}




unsigned char read_spi0()
{
  return U0DBUF;
}

void sendSpi0 (unsigned char value)
{

  NssSpi0=0;
  U0DBUF = value;
  //while( !(U0CSR & 0x02)); // wait 
  while( (U0CSR & 0x01)); // wait spi inter idle mooode -> end of transmiit
  NssSpi0=1;

}



void enable_int_Spi0 (char type)  // rx or tx 
{
  if(type)
    IEN0 |=(1<<2);
  else
    IEN2 |=(1<<2);
  IEN0 |= (1<<7);
}

void disable_int_Spi0 (char type)  // rx or tx 
{
  if(type)
    IEN0 &=~(1<<2);
  else
    IEN2 &=~(1<<2);
  
}

//----tariqeye tarif routin baraye rx tx usart 0---//
/*
#pragma vector=URX0_VECTOR
__interrupt void UART0_ISR(void){
    URX0IF = 0; //Cpu be sorate utomatic ham pak mikonad pas in lazm nis                
}

#pragma vector=UTX0_VECTOR
__interrupt void UART0_ISR(void){
    UTX0IF = 0; //flag marboot be baxshe inttrupt ke bayad pak shawad           
}
*/
//------------------------SPI1----------------------//
void spi1_init()
{
   if(ModeSpi1)
   {
     PortSpi1 |= (1<<PinSpi1);  // NSS is OUTPUT
     NssSpi1=1; // NSS set high use for slave select
   }
   
  
  U1GCR = (BAUD_ESpi1)|(ORDERSpi1<<5)|(CPHASpi1<<6)|(CPOLSpi1<<7);
  U1BAUD = BAUD_MSpi1;

  
  
  // --- configure i/o for use peripheral FOR SPI1 ALT2 & priorety
  if(locationSpi1)
  {
    if(ModeSpi1)
      P1SEL |= (1<<5)|(1<<6)|(1<<7); // GPIOs is peripheral
    else
      P1SEL |= (1<<4)|(1<<5)|(1<<6)|(1<<7); // GPIOs is peripheral
    PERCFG |= 2; // USART 1 IN alt 2 
    P2SEL |=(1<<6);
    P2SEL &= ~((1<<5)|(1<<3)); // extesas dadne awlawiat be USART1
  }
  else
  {
    if(ModeSpi1)
      P0SEL |=(1<<3)|(1<<4)|(1<<5);     // GPIOs is peripheral
    else
      P0SEL |=(1<<2)|(1<<3)|(1<<4)|(1<<5);     // GPIOs is peripheral
    PERCFG &=~(2);  // USART 1 IN alt 1
    P2DIR &=~(0x80); // // extesas dadne awlawiat be USART1 dar alt1
    P2DIR |=0x40;
  }
  
  //--- configure usart registers
  U1CSR &= ~(1<<7); // SELECT SPI MODE
  if(ModeSpi1)  // selact slave or master
   U1CSR &= ~(1<<5);
  else
   U1CSR |= (1<<5);
  

  
}


unsigned char read_spi1()
{
  return U1DBUF;
}


void sendSpi1 (unsigned char value)
{

  NssSpi1=0;
  U1DBUF = value;
  //while( !(U1CSR & 0x02)); // wait 
  while((U1CSR & 0x01)); // wait spi inter idle mooode -> end of transmiit
  NssSpi1=1;

}


void stmGet(uint8_t *value, uint8_t len){
	sendSpi1(0x00);
	char tmp = U1DBUF;
	for(int i=0; i<len; i++){
		sendSpi1(0x00);
		*value = U1DBUF;
		value++; 
	}
}


void enable_int_Spi1 (char type) //rx or tx  
{
  if(type)
    IEN0 |=(1<<3);
  else
    IEN2 |=(1<<3);
  IEN0 |= (1<<7);
}

void disable_int_Spi1 (char type) //rx or tx  
{
  if(type)
    IEN0 &=~(1<<3);
  else
    IEN2 &=~(1<<3);
}

//----tariqeye tarif routin baraye rx tx usart 1---//
/*
#pragma vector=URX1_VECTOR
__interrupt void UART0_ISR(void){
    URX1IF = 0; //Cpu be sorate utomatic ham pak mikonad pas in lazm nis                
}

#pragma vector=UTX1_VECTOR
__interrupt void UART0_ISR(void){
    UTX1IF = 0; //flag marboot be baxshe inttrupt ke bayad pak shawad           
}

*/
