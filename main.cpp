#include "packetProcessor.h"
#include "motionProcessor.h"
#include "cc2530_usart.h"
#include "cc2530_timer1.h"
#include "TableClass.h"
#include "QueueClass.h"
#include "cc2530_spi.h"

#include "types.h"

uint8_t myCarID[5] = {0,0,0,0,2};

void main(void){
  	// temp buffer
	char buff[30];
	
	packetProceccor.stackInit();

	// Sender (Braker)
	motionProceccor.myLocation_old.lat = 37.593388;
	motionProceccor.myLocation_old.lon = 45.062480;
	motionProceccor.myLocation.lat = 37.593592;
	motionProceccor.myLocation.lon = 45.062459;
	motionProceccor.setMyDirection(); 
	uart0_sendbuf((uint8_t *)buff, sprintf(buff,"S-DIR: %d, S-ChangedLoc: %d\n",motionProceccor.getMyDirection(),motionProceccor.getDistanse(motionProceccor.myLocation_old)));
	uart0_sendbuf((uint8_t *)buff, sprintf(buff,"LAT: %f, LAN: %f\n", motionProceccor.myLocation.lat, motionProceccor.myLocation.lon));    
        

	/*CarPacket_t senderPacket;
	senderPacket.sd = '!';
	senderPacket.type = BRAKE_PACKET;
	SET_CAR_ID(senderPacket.id);
	senderPacket.sn = 1;
	senderPacket.fs = 4;
	senderPacket.vt = CAR;
	senderPacket.ttl = 1;
	senderPacket.v = 20;
	senderPacket.aa = 0;
	senderPacket.dir = motionProceccor.getMyDirection();
	senderPacket.loc.lat = motionProceccor.myLocation.lat;
	senderPacket.loc.lon = motionProceccor.myLocation.lon;
	senderPacket.opt = 1;
	senderPacket.ed = '$';
	*/
	
	P0DIR &= ~(1<<6 | 1<<7);
	
	while(1){
		//senderPacket.sn++;
        //txTable.sendPacket(&senderPacket,LOCALPACKET);	
		packetProceccor.on();
		//for(int i=0; i<30000;i++);
		//for(int i=0; i<30000;i++);
		//for(int i=0; i<30000;i++);
		//for(int i=0; i<30000;i++);
                
	}
}
