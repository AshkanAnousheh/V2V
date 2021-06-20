#include "types.h"

#include "cc2530_rf.h"
#include "QueueClass.h"
#include "TableClass.h"
#include "cc2530_usart.h"

/** RF Functions **/
#pragma vector=RF_VECTOR
__interrupt void rf_isr(void){ // 188usec duration for ISR time
	EA = 0;
	if (RFIRQF0 & ( 1<<6 )){  	// RF Interrupt Flags
								// 		FRAME_ACCEPTED == 1 : Frame has passed frame filtering.
		/**------------V2V Version------------**/	
		int rf_rx_len = XREG(RX_PACKET_LEN) - 2;
		rf_rx_len &= 0x7F;
		int rssi = XREG(RX_PACKET_RSSI);
		char crc_ok = XREG(RX_PACKET_CRC);
		
		if(crc_ok & 0x80) // CRC OK
			if(txTable.broadcastChecker() && rxQueue.broadcastChecker()){
				rxQueue.put();
			}
			else{
				P1_0 ^= 1;
			}	
		else{ // CRC Error
		}
		
		RFST = 0xED;  // RF command strobe: ISFLUSHRX
					  // The ISFLUSHRX instruction immediately flushes the RXFIFO buffer.
		/**------------V2V Version------------**/	
			
		S1CON = 0;              // Interrupt Flags 3
		RFIRQF0 &= ~(1<<6);                         
    }	
	EA = 1;
}
void cc2530_rf_init(){
    TXPOWER   = 0xF5;       // Transmit Power
							// 		0xD5 = 1dBm = 1.25mW
                                                        // 		0xF5 = 4.5dBm = 2.81mW
    
	CCACTRL0  = 0xF8;       // CCA Threshold  
							// 		From SmartRF

    FRMFILT0  = 0x0C;       // Frame Filtering
							// 0-000-11-0-1: Reserved-FCF_RESERVED_MASK-MAX_FRAME_VERSION-PAN_COORDINATOR-FRAME_FILTER_EN
							// 		MAX_FRAME_VERSION: 11 = if the frame version subfield(FCF) is higher than MAX_FRAME_VERSION[1:0] and frame filtering is enabled, the frame is rejected.
							//		FCF: 00-0-0-0-0-000-00-00-00: FrameType-SecurityEnable-FramePadding-AckRequest-IntraPan-Reserved-DstAddressingMode-Reserved-SrcAddressingMode
	
	/** 
		This register settings must be updated from their default value to have optimal performance. 
		Start:
	**/
    FSCAL1 =    0x00;       // Tune Frequency Calibration.
							// 001010-01: Reserved-VCO_CURR
							//		VCO: Voltage-controlled oscillator	
							//		VCO=0 : Reduces the VCO leakage by about 3 dB compared to default setting. 
							//		From SmartRF
    TXFILTCFG = 0x09;		// TX Filter Configuration.
							// 0000-1111: Reserved-FC
							// 		FC: 9 = Reduces spurious emissions close to signal. 
							// 		From SmartRF
    AGCCTRL1 =  0x15;		// AGC Reference Level (AGC: Automatic Gain Control).
							// 00-100001: Reserved-AGC_REF
							// 		AGC_REF: 21 = Adjusts AGC target value.
							//		From SmartRF
    AGCCTRL2 =  0xFE;		// AGC Gain Override.
							// 00-000-00-0: LNA1_CURRENT-LNA2_CURRENT-LNA3_CURRENT-LNA_CURRENT_OE
							// 		LNA1_CURRENT: 6 dB gain
							// 		LNA2_CURRENT: 18 dB gain
							// 		LNA3_CURRENT: 9 dB gain
							//		LNA_CURRENT_OE: 1 = Override the AGC LNA current setting with the values above.
							//		From SmartRF
	/** 
		END
	**/								
	
    FREQCTRL = 0x0B;		// Controls the RF Frequency
							// 0-0000000: Reserved-FREQ
							//		F_rf = (2394 + FREQ) MHz
							//		CH_11 = 0x0B
							//		CH_15 = 0x19
							//		CH_26 = 0x1A

    RFIRQM0 |= (1<<6);  	// RF Interrupt Masks
							// 0-0-0-0-0-0-0-0: RXMASKZERO-RXPKTDONE-FRAME_ACCEPTED-SRC_MATCH_FOUND-SRC_MATCH_DONE-FIFOP-SFD-ACT_UNUSED
							//		FRAME_ACCEPTED: 1 = Frame has passed frame filtering. 

    IEN2 |= (1<<0);         // Interrupt Enable 2
							// 00-0-0-0-0-0-0-0-0: Reserved-WDTIE-P1IE-UTX1IE-UTX0IE-P2IE-RFIE
							// 		RFIE:0 = RF general interrupt enable

    RFST = 0xED;            // RF command strobe: ISFLUSHRX
							// 		ISFLUSHRX: The ISFLUSHRX instruction immediately flushes the RXFIFO buffer and resets the demodulator.	
    RFST = 0xE3;            // RF command strobe: ISRXON  
							// 		ISRXON: The ISRXON instruction immediately enables and calibrates the frequency synthesizer for RX.
}

void cc2530_rf_send(uint8_t *pbuf , int len){
    /** 1: indicates whether the channel is available for transmission or not. **/
	RFST = 0xE3;            // RF command strobe: ISRXON  
							// 		Enable and calibrate frequency synthesizer for RX.
							//		The instruction waits for the radio to acknowledge the command before executing the next instruction.
	while( FSMSTAT1 & (( 1<<1 ) | ( 1<<5 ))); // Radio Status Register
							// 0-0-0-0-0-0-0-0: FIFO-FIFOP-SFD-CCA-SAMPLED_CCA-LOCK_STATUS-TX_ACTIVE-RX_ACTIVE
							// 		TX_ACTIVE: Status signal, active when FFCTRL is in one of the transmit states.									
							// 		CCA: Clear-channel assessment. 
    
	/** 2: Channel is clear for transmission, turn off RF Interrupts **/
	RFIRQM0 &= ~(1<<6);     // RF Interrupt Masks.
							// 0-0-0-0-0-0-0-0: RXMASKZERO-RXPKTDONE-FRAME_ACCEPTED-SRC_MATCH_FOUND-SRC_MATCH_DONE-FIFOP-SFD-ACT_UNUSED
							// 		FRAME_ACCEPTED: 0 = Interrupt disabled.

    IEN2 &= ~(1<<0);        // Interrupt Enable 2
							// 00-0-0-0-0-0-0: Reserved-WDTIE-P1IE-UTX1IE-UTX0IE-P2IE-RFIE
							//	 	RFIE: 0 = RF general interrupt disabled
	
	/** 3: Channel is clear for transmission, Clear TXFIFO buffer **/
    RFST = 0xEE;            // RF command strobe: ISFLUSHTX
							// 		The ISFLUSHTX instruction immediately flushes the TXFIFO buffer.
	
	/** 4: Channel is clear for transmission, Enable transmission done interrupt **/	
    RFIRQF1 = ~(1<<1);      // RF Interrupt Flags.
							// 00-0-0-0-0-0-0: Reserved-CSP_WAIT-CSP_STOP-CSP_MANINT-RFIDLE-TXDONE-TXACKDONE
							// 		TXDONE: 1 = A complete frame transmission interrupt flag cleared.
	
	/** 5: Channel is clear for transmission, Write frame content in TXFIFO buffer. **/
	RFD = len + 2;
    for (int i = 0; i < len; i++) {
        RFD = *pbuf++;
    }
	
	/** 6: Channel is clear for transmission, Do frame transmission **/
    RFST = 0xE9;            // RF command strobe: ISTXON  
							// 		The ISTXON instruction immediately enables TX after calibration. 
							//		The instruction waits for the radio to acknowledge the command before executing the next instruction.
	while (!(RFIRQF1 & (1<<1))); 
    RFIRQF1 = ~(1<<1);      // RF Interrupt Flags.
							// 00-0-0-0-0-0-0: Reserved-CSP_WAIT-CSP_STOP-CSP_MANINT-RFIDLE-TXDONE-TXACKDONE
							// 		TXDONE: 1 = A complete frame transmission interrupt flag cleared.
    
	/** 7: One Frame transmission is done, enale receive frame interrupt **/
	RFIRQM0 |= (1<<6);      // RF Interrupt Masks.
							// 0-0-0-0-0-0-0-0: RXMASKZERO-RXPKTDONE-FRAME_ACCEPTED-SRC_MATCH_FOUND-SRC_MATCH_DONE-FIFOP-SFD-ACT_UNUSED
							// 		FRAME_ACCEPTED: 0 = Interrupt enabled.         
    
	IEN2 |= (1<<0);			// Interrupt Enable 2
							// 00-0-0-0-0-0-0: Reserved-WDTIE-P1IE-UTX1IE-UTX0IE-P2IE-RFIE
							//	 	RFIE: 0 = RF general interrupt enabled.					
}



