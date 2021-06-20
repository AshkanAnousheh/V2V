#include "logClass.h"

LogClass::LogClass(){
	tx_done_forward = 0;
	tx_done_localPacket = 0;
	tx_drop_pendingTime = 0;
	rx_drop_forwardEcho = 0;
	rx_drop_localEcho = 0;
	rx_drop_pendingInRXQueue = 0;
	rx_drop_QueueFull = 0;
	rx_done = 0;
}

LogClass logger;