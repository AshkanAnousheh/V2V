#include "TableClass.h"
#include "cc2530_rf.h"
#include "logClass.h"

TableClass::TableClass(){
	for(int i = 0; i<TABLESIZE; i++){
		table[i].validity = false;
		table[i].txDone = false;
		table[i].pendingTime = 0;
	}	
}

char TableClass::insert(CarPacket_t * inputPacketAddr, int distanse){ 
	for(int i = 0; i<TABLESIZE; i++) { // Discard forwarding for pending packet 
		if((table[i].validity == true) && (ID_CMP(inputPacketAddr->id, table[i].packet.id)) && inputPacketAddr->sn == table[i].packet.sn && table[i].validity){		
			logger.tx_drop_pendingTime++;
			return -1;
		}	
	}
	for(int i = 0; i<TABLESIZE; i++) { // insert at firs empty entry
		if(!table[i].validity){
			memcpy((uint8_t *)&table[i].packet, (uint8_t *)inputPacketAddr, CARPACKETLENGTH);
			table[i].validity = true;
			if(distanse>MAXRFRANGE){
				distanse = MAXRFRANGE;
			}
			table[i].pendingTime = (char)((MAXRFRANGE - distanse)/10);
			table[i].txDone = false;
			return 0;
		}	
	}
	return -1;
}  

void TableClass::forwardOn() { // Send one packet per call
	for(int i = 0; i<TABLESIZE; i++) {
		if((table[i].pendingTime==0) && (table[i].txDone==false) && (table[i].validity==true)) {			
			sendPacket(&table[i].packet, FORWARDPACKET);
			table[i].txDone = true;
			table[i].pendingTime = PENDINGTIMEAFTERTX;
			return;
		}
	}
}

void TableClass::timer() { 
	for(int i = 0; i<TABLESIZE; i++) {
		if((table[i].validity==true) && (table[i].pendingTime>0)){
			table[i].pendingTime--;
		}	
		if((table[i].pendingTime==0) && (table[i].txDone==true)){
			table[i].validity = false;
			table[i].txDone=false;
		}
	}
}	

uint8_t TableClass::getCurrentTableLength(){
	uint8_t currentRowCnt = 0;	
	for(int i = 0; i<TABLESIZE; i++){
		if(table[i].validity == true)
			currentRowCnt++;
	}
	return currentRowCnt;	
}

bool TableClass::broadcastChecker(){
	for(int i=0; i<TABLESIZE; i++){
		if((table[i].validity == true) && CHECK_XREGPACKETID_BY(table[i].packet.id) && (CHECK_XREGPACKETSN_BY(table[i].packet.sn))){
			table[i].txDone=true;
			logger.rx_drop_forwardEcho++;
			return false;	
		}
	}
	return true;
}

void TableClass::sendPacket(CarPacket_t * packetPtr, bool locFor){
	P1_1 ^= 1;
	cc2530_rf_send((uint8_t *)packetPtr, CARPACKETLENGTH);
	if(locFor == LOCALPACKET){
		logger.tx_done_localPacket++;
        }        
	else if(locFor == FORWARDPACKET)
		logger.tx_done_forward++;
}

TableClass txTable;