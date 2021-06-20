#ifndef SPI_H
#define SPI_H


//------- dast nazan------//
#define Master 1
#define Slave 0
#define alt1 0
#define alt2 1
#define rx 1
#define tx 0
//-----------------------//


//-----------SPI0--------///
///************* tanzimate marbooot be payeye chip select(SS) Spi0 , dar halate master *********
#define PortSpi0 P0DIR // PORT marboooot be entxabe NSS dar halate master  P0DIR or P1DIR or P2DIR
#define PinSpi0 0  // pin mawrde nazar dar PORT tarif shooode baraye NSS
#define NssSpi0 P0_0
////////----------------------------------////////


//------ tanzimate paikar bandi SPi 0-----//
#define ModeSpi0 Master  // Master or Slave 
#define CPOLSpi0   0 // 0 or 1
#define CPHASpi0   0 // 0 or 1
#define ORDERSpi0  1 // 1=msb first or 0=Lsbfirst
//baude rate=8MHz
#define BAUD_ESpi0 18  // 5 bit only
#define BAUD_MSpi0 0 // 8bit                    
#define locationSpi0 alt2 // alt1 or alt2

//------------------------------------//

//-----------SPI1-------////////


///************* tanzimate marbooot be payeye chip select(SS) Spi1 , dar halate master   *********
#define PortSpi1 P1DIR // PORT marboooot be entxabe NSS dar halate master  P0DIR or P1DIR or P2DIR
#define PinSpi1 4  // pin mawrde nazar dar PORT tarif shooode baraye NSS
#define NssSpi1 P1_4
////////----------------------------------////////




//------ tanzimate paikar bandi SPi 1-----//
#define ModeSpi1 Master  // Master or Slave 
#define CPOLSpi1   0 // 0 or 1
#define CPHASpi1   0 // 0 or 1
#define ORDERSpi1  1 // 1=msb first or 0=Lsbfirst
//baude rate=230400
#define BAUD_ESpi1 18  // 5 bit only
#define BAUD_MSpi1 0 // 8bit 
#define locationSpi1 alt2 // alt1 or alt2

//------------------------------------//

extern void spi1_init();
extern void stmGet(uint8_t *value, uint8_t len);

#endif