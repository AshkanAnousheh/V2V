#include "QueueClass.h"

/** UART0@115200 and Interrupt@(RX,TX) **/
#pragma vector=URX0_VECTOR
__interrupt void UART0_ISR(void){
    URX0IF = 0;                                  
	char rx_char = U0DBUF;
}

void cc2530_uart0_init(){
    PERCFG = 0x00;          // Peripheral Control
							// 0-0-0-0-00-0-0: Reserved-T1CFG-T3CFG-T4CFG-Reserved-U1CFG-U0CFG
							// 		USART0 @ Alternative 1 location

    P0SEL |= 0x0C;          // Port 0 Function Select
							// 0-0-0-0-0-0-0-0: P0.7-P0.6-P0.5-P0.4-P0.3-P0.2-P0.1-P0.0
							// 		1 = Peripheral function, 0: General-purpose I/O	
							// 		0x0C = P0.3,P0.2 as Peripheral function.

    U0CSR |= 0xC0;          // USART 0 Control and Status
							// 0-0-0-0-0-0-0-0: MODE-RE-SLAVE-FEERR-RX_BYTE-TX_BYTE-ACTIVE
							// 		MODE: 1 = UART mode.
							// 		RE: 1 = Receiver enabled.
							
    U0GCR |= 15;            // USART 0 Generic Control
							// 0-0-0-00000: CPOL-CPHA-ORDER-BAUD_E
							//		BAUD_E: 11	
    U0BAUD = 98;           // USART 0 Baud-Rate Control
							// 00000000: BAUD_M 
							// 		BAUD_M: 216
							// UART Speed: 115200 bps
							
    UTX0IF = 1;				// USART 0 TX interrupt flag
							// IRCON2.UTX0IF

    URX0IE = 1;             // USART0 RX interrupt enable
							// IEN0.URX0IE	
}

void uart0_sendbuf(uint8_t *buff, char len){
	for( int i = 0 ; i < len ; i++){
        while(!UTX0IF);
        UTX0IF = 0;
        U0DBUF = *buff++;
    }
}
