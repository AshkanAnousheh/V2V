#include "types.h"
#include "QueueClass.h"
#include "logClass.h"
#define adderBuffPtr ((uint8_t *)&fifo[AdderPtr].packet)
#define adderBuffValidity fifo[AdderPtr].validity
#define serverBuffPtr ((uint8_t *)&fifo[ServerPtr].packet)
#define serverBuffValidity fifo[ServerPtr].validity

QueueClass::QueueClass(){
	AdderPtr = 0;
	ServerPtr = 0;
	length = 0;
	for(char i=0; i<Q_SIZE; i++){
		fifo[i].validity=false;
	}
} 	
	
char QueueClass::put(){
	if(AdderPtr == (( ServerPtr - 1 + Q_SIZE) % Q_SIZE)){
		logger.rx_drop_QueueFull++;
		return -1; /* Queue Full*/
	}	
	
	for(int i=0; i<CARPACKETLENGTH; i++)
		adderBuffPtr[i] = XREG(RX_PACKET_PTR + i);
	adderBuffValidity = true;
	
	logger.rx_done++;
	
	length++;
	AdderPtr = (AdderPtr + 1) % Q_SIZE;
	return 0; // No errors
}

char QueueClass::get(uint8_t *packet){
	if(AdderPtr == ServerPtr)
        return 0; /* Queue Empty - nothing to get*/
	
	P1_0 ^= 1;
	
    memcpy(packet, serverBuffPtr, CARPACKETLENGTH);
	serverBuffValidity = false;
	
    ServerPtr = (ServerPtr + 1) % Q_SIZE;
	length--;
    return 1; // No errors
}

bool QueueClass::broadcastChecker(){
	if(CHECK_XREGPACKETID_BY_CARID){
		logger.rx_drop_localEcho++;
		return false;
	}	
	
	for(char i=0; i<Q_SIZE; i++){
		if((fifo[i].validity==true) && (CHECK_XREGPACKETID_BY(fifo[i].packet.id)) && (CHECK_XREGPACKETSN_BY(fifo[i].packet.sn))){
			fifo[i].packet.fs = 0;
			logger.rx_drop_pendingInRXQueue++;
			return false;
		}
	}
	
	return true;	
}

QueueClass rxQueue;

