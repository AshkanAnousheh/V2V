#ifndef type_h
#define type_h

#include "ioCC2530.h"


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef long long int64_t;

typedef unsigned char uint_fast8_t;
typedef unsigned short uint_fast16_t;
typedef unsigned long uint_fast32_t;
typedef unsigned long long uint_fast64_t;
/**------------------------     Packets Type     ------------------------**/
#define LOCALPACKET	0
#define FORWARDPACKET 1
/**----------------------------------------------------------------------**/
/**------------------------ ID and SN Assistants ------------------------**/
extern uint8_t myCarID[5];
#define ID_CMP(id1, id2) ((id1[0]==id2[0]) && (id1[1]==id2[1]) && (id1[2]==id2[2]) && (id1[3]==id2[3]) && (id1[4]==id2[4]))
	
#define CHECK_XREGPACKETID_BY_CARID ((XREG(RX_PACKET_ID)==myCarID[0]) && \
	(XREG(RX_PACKET_ID+1)==myCarID[1]) && \
	(XREG(RX_PACKET_ID+2)==myCarID[2]) && \
	(XREG(RX_PACKET_ID+3)==myCarID[3]) && \
	(XREG(RX_PACKET_ID+4)==myCarID[4]) )
#define CHECK_XREGPACKETID_BY(id) ((XREG(RX_PACKET_ID)==id[0]) && \
	(XREG(RX_PACKET_ID+1)==id[1]) && \
	(XREG(RX_PACKET_ID+2)==id[2]) && \
	(XREG(RX_PACKET_ID+3)==id[3]) && \
	(XREG(RX_PACKET_ID+4)==id[4]))
#define CHECK_XREGPACKETSN_BY(sn) ((XREG(RX_PACKET_SN)==sn))
#define SET_CAR_ID(id)\
do\
   {id[0]=myCarID[0];\
	id[1]=myCarID[1];\
	id[2]=myCarID[2];\
	id[3]=myCarID[3];\
	id[4]=myCarID[4];\
} while(0)
/**-----------------------------------------------------------------------**/

#define RXFIFO_PTR 0x6000
#define RX_PACKET_ID 0x6000 + 3
#define RX_PACKET_SN 0x6000 + 8
#define RX_PACKET_PTR 0x6000 + 1
#define RX_PACKET_RSSI 0x6000 + CARPACKETLENGTH + 1
#define RX_PACKET_CRC 0x6000 + CARPACKETLENGTH + 1 + 1
#define RX_PACKET_LEN 0x6000 

#define ACCE_THRESHOLD 	1

#define BEACON_PACKET   1
#define BRAKE_PACKET 	2
#define CAR				1


struct location_t{
	float lat, lon;
};

struct CarPacket_t{
	uint8_t sd[4], type, id[5], sn, fs, vt/*VehicleType*/, ttl, v, aa;
	int dir;
	struct location_t loc;
	int opt;
	char ed;
};
#define CARPACKETLENGTH sizeof(CarPacket_t)

struct tableRow_t{
	struct CarPacket_t packet;
	char pendingTime;
	bool txDone;
	bool validity;
};


struct queueElement_t{
	struct CarPacket_t packet;
	bool validity;
};

struct stmData_t{
	location_t loc;
	uint8_t v;
};


#endif