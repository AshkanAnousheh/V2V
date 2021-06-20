#include "cc2530_rf.h"
#include "cc2530_usart.h"
#include "cc2530_timer1.h"
#include "packetProcessor.h"
#include "motionProcessor.h"
#include "QueueClass.h"
#include "TableClass.h"

PackProc::PackProc(){
	
}

void PackProc::sendToSmartPhone(int len){
	uart0_sendbuf(BUFFPTR, len);	
} 	

void PackProc::on(){
	if(rxQueue.get(BUFFPTR)){
		switch(currentPacket.type){
			case BEACON_PACKET:
				if(currentPacket.fs>1){
					currentPacket.fs--;
					txTable.insert(&currentPacket, motionProceccor.getDistanse(currentPacket.loc));	
				}
				sendToSmartPhone(CARPACKETLENGTH);		
			break;
			case BRAKE_PACKET:
				if(motionProceccor.brakePacketForwardChecker(currentPacket.dir, currentPacket.loc)){	
					if(currentPacket.fs>1){
						currentPacket.fs--;
						txTable.insert(&currentPacket, motionProceccor.getDistanse(currentPacket.loc));	
					}
					sendToSmartPhone(CARPACKETLENGTH);	
				}
				else{
					//Drop packet
					P1_0 ^= 1;
				} 
			break;
		}	
	}
	
	txTable.forwardOn();
	motionProceccor.stmInterfaceOn();
	motionProceccor.acceInterfaceOn();
        if(motionProceccor.beaconTimer>900){
		motionProceccor.sendPacket(BEACON_PACKET);
		motionProceccor.beaconTimer = 0;
	}
} 	

void PackProc::off(){
	
} 	

void PackProc::stackInit(){
	EA = 0;
	// 32MHz Operation
	CLKCONCMD = 0x00;
	while((CLKCONSTA != CLKCONCMD));
	
	// LED Conf
	P1DIR |= 0x07;
	P1 = 0x00;
	
	// Initialize Peripheral Components
	cc2530_rf_init();
	cc2530_uart0_init();
	motionProceccor.stm_init();
	motionProceccor.acce_init();
	timer1_init();
	timer1_enable();
	EA = 1;	
}

PackProc packetProceccor;	
