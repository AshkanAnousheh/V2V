#include "motionProcessor.h"
#include "cc2530_spi.h"
#include "cc2530_usart.h"
#include "main_accel_myfilter.h"
#include "TableClass.h"

MotProc::MotProc(){
	serialNumber = 0;
	beaconTimer = 0;
}

int MotProc::getDistanse(location_t senderLocation){
	float myLat = myLocation.lat;
	float myLon = myLocation.lon;
	
	senderLocation.lat *= conver_dag_rad;
	senderLocation.lon *= conver_dag_rad;
	myLat *= conver_dag_rad;
	myLon *= conver_dag_rad;
	float x = (myLon-senderLocation.lon) * cos((myLat+senderLocation.lat)/2);
	float y =(myLat-senderLocation.lat);
	float d = sqrt(x*x+y*y) * R;
	return (int)floor(d);	
}

location_t MotProc::getMyLocation(){
	return myLocation;
}

void MotProc::setMyLocAndV(stmData_t stmData){
	memcpy((uint8_t*)&myLocation_old, (uint8_t*)&myLocation, 8);
	memcpy((uint8_t*)&myLocation, (uint8_t*)&stmData.loc, 8);
	myVelocity = stmData.v;
	setMyDirection();
}


void MotProc::setMyDirection(){
	myDirection = getDirection(myLocation_old, myLocation);
}

int MotProc::getMyDirection(){
	return myDirection;
}

int MotProc::getDirection(location_t locA, location_t locB){
	locA.lat *=conver_dag_rad;
	locB.lat *=conver_dag_rad;
	locA.lon *=conver_dag_rad;
	locB.lon *=conver_dag_rad;
	float y = sin(locB.lon-locA.lon) * cos(locB.lat);
	float x = cos(locA.lat)*sin(locB.lat) - sin(locA.lat)*cos(locB.lat)*cos(locB.lon-locA.lon);
	float bear = atan2(y, x)*conver_rad_deg;
	bear= ((int)bear+180)%360;
	return (int)bear;
}

bool MotProc::isSameDirection(location_t senderLocation){
	int deltaAlpha = abs(myDirection - getDirection(myLocation, senderLocation));
	if((0<deltaAlpha) && (deltaAlpha<30))
		return true;	
	return false;	
}	

bool MotProc::brakePacketForwardChecker(int senderDirection, location_t senderLocation){
	int deltaAlpha = abs(senderDirection-myDirection);
	if((150<deltaAlpha) && (deltaAlpha<210))
		return false;
	if(isSameDirection(senderLocation)){
		return true;	
	}
	return false;	
}

bool MotProc::stm_init(){
	P1DIR &= ~(1<<3);
	spi1_init();
	return true;
}

bool MotProc::acce_init(){
	accelInit();
	return true;
}

void MotProc::acceInterfaceOn(){
	if(acce_data_ready){
		calculateAccel();
                if(abs(linearAccY) > ACCE_THRESHOLD){
                  sendPacket(BRAKE_PACKET);
                }       
	}
}

void MotProc::stmInterfaceOn(){
	stmData_t stmData;
	uint8_t *stmDataPtr;
	stmDataPtr = (uint8_t *)&stmData;	
	if(P1_3){
		stmGet(stmDataPtr,9);		
		setMyLocAndV(stmData);
	}	
}

void MotProc::sendPacket(uint8_t type){
	CarPacket_t senderPacket;	
	senderPacket.sd[0] = '!';
	senderPacket.sd[1] = '!';
	senderPacket.sd[2] = '!';
	senderPacket.sd[3] = '!';
	senderPacket.type = type;
	SET_CAR_ID(senderPacket.id);
	senderPacket.sn = serialNumber++;
	if(type == BRAKE_PACKET)
		senderPacket.fs = 3;
	if(type == BEACON_PACKET)
		senderPacket.fs = 2;
	senderPacket.vt = CAR;
	if(type == BRAKE_PACKET)
		senderPacket.ttl = 5;
	if(type == BEACON_PACKET)
		senderPacket.ttl = 2;
	senderPacket.v = myVelocity;
	senderPacket.aa = 0;
	senderPacket.dir = getMyDirection();
	//senderPacket.loc.lat = myLocation.lat;
        //senderPacket.loc.lon = myLocation.lon;
        senderPacket.loc.lat = 35.742785;
        senderPacket.loc.lon = 51.499824;
        senderPacket.opt = 0;
	senderPacket.ed = '$';
	//senderPacket.ed[1] = '$';
	//senderPacket.ed[2] = '$';
	//senderPacket.ed[3] = '$';
	txTable.sendPacket(&senderPacket,LOCALPACKET);	
	uart0_sendbuf((uint8_t *)&senderPacket,CARPACKETLENGTH);
	//uint8_t buff[100];
	//char len = sprintf((char *)buff, "--LAT:%f LAN:%f DIR:%d--\n\r", senderPacket.loc.lat, senderPacket.loc.lon, senderPacket.dir);
	//uart0_sendbuf(buff,len);
        //if(type == BRAKE_PACKET) P1_2 ^= 1;          
}

MotProc motionProceccor;
