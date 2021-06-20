#include "cc2530_timer1.h"
#include "TableClass.h"
#include "motionProcessor.h"

/** Timer1@CompareMode and Interrupt(1mSec) **/
#pragma vector=T1_VECTOR
__interrupt void Timer1_ISR(void){
    T1STAT &= ~( 1<< 0);
	txTable.timer();
	motionProceccor.beaconTimer++; 
}

void timer1_init(){
    T1CTL = 0x0C;         	// Timer 1 Control
							// 0000-00-00: Reserved-DIV-MODE
							// DIV: 11 = Tick frequency/128
							// MODE: 00 = Operation is suspended.
							
    T1CCTL0 = 0x44;       	// Timer 1 Channel 0 Capture/Compare Control
							// 0-1-000-0-00: RFIRQ-IM-CMP-MODE-CAP
							// 		RFIRQ: 0 = Regular capture input.
							// 		IM: 1 = Enables interrupt request. 
							// 		CMP: 000 = Set output on compare.
							// 		MODE: 1 = Compare mode.
							// 		CAP: 00 = No capture.
							
    T1STAT = 0x00;        	// Timer 1 Status
							// 00-0-0-0-0-0-0: Reserved-OVFIF-CH4IF-CH3IF-CH2IF-CH1IF-CH0IF
							// 		OVFIF: Timer 1 counter-overflow interrupt flag.
							// 		CH4IF: Timer 1 channel 4 interrupt flag.
							// 		CH3IF: Timer 1 channel 3 interrupt flag.
							// 		CH2IF: Timer 1 channel 2 interrupt flag.
							// 		CH1IF: Timer 1 channel 1 interrupt flag.
							// 		CH0IF: Timer 1 channel 0 interrupt flag.
							
    T1IE = 1;             	// IEN1.T1IE
							// Timer 1 interrupt enable

    T1CC0L = 250;         	// Timer 1 channel 0 capture/compare value low
    T1CC0H = 0;				// Timer 1 channel 0 capture/compare value high
							// 32MHz/128 = 256KHz
							// T = 1/F = 1/256KHz = 4uSec
							// one cycle interrupt = 4uSec * 250 = 1mSec
}
void timer1_disable(){
    T1CTL &= ~( 1 << 1);   	// Timer 1 Control
							// 0000-00-00: Reserved-DIV-MODE
							// DIV: 11 = Tick frequency/128
							// MODE: 00 = Operation is suspended.
}
void timer1_enable(){
    T1CTL |= ( 1 << 1 );  	// Timer 1 Control
							// 0000-00-00: Reserved-DIV-MODE
							// DIV: 11 = Tick frequency/128
							// MODE: 01 = Free-running, repeatedly count from 0x0000 to 0xFFFF.

    T1CNTH = 0;           	// Timer 1 Counter High 
    T1CNTL = 0;				// Timer 1 Counter Low
							// All write accesses to the T1CNTL register reset the 16-bit counter.
	
	T1STAT = 0x00;      	// Timer 1 Status
							// 00-0-0-0-0-0-0: Reserved-OVFIF-CH4IF-CH3IF-CH2IF-CH1IF-CH0IF
							// 		CH0IF: Timer 1 channel 0 interrupt flag.						
}
