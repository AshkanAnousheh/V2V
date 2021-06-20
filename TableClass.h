#ifndef table_class_h
#define table_class_h

#include <string.h>
#include "types.h"

#define TABLESIZE 10			// Packet
#define MAXRFRANGE 200 			// Meter
#define PENDINGTIMEAFTERTX 30 	// msec

class TableClass {
	private:
	public:
	/* Variables */
	tableRow_t table[TABLESIZE];	
	/* Functions(Public) */
	
	TableClass();
	char insert(CarPacket_t * inputPacketAddr, int distanse);
	void forwardOn();
	void timer();
	uint8_t getCurrentTableLength();
	bool broadcastChecker();
	void sendPacket(CarPacket_t * packetPtr, bool locFor);
};

extern TableClass txTable;

#endif