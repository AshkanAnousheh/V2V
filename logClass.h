#ifndef logClass_h
#define logClass_h

#include <string.h>
#include "types.h"

class LogClass {
	public:
	/* Variables */
	uint32_t tx_done_forward;
	uint32_t tx_done_localPacket;
	uint32_t tx_drop_pendingTime;
	uint32_t rx_drop_forwardEcho;
	uint32_t rx_drop_localEcho;
	uint32_t rx_drop_pendingInRXQueue;
	uint32_t rx_drop_QueueFull;
	uint32_t rx_done;
	/* Functions(Public) */
	
	LogClass();
};

extern LogClass logger;

#endif

