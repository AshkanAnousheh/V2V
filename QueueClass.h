#ifndef queue_class_h
#define queue_class_h

#include <string.h>
#include "types.h"

#define FIFOSIZE 10
#define Q_SIZE	(FIFOSIZE+1)
#define RXFIFO_PTR 0x6000


class QueueClass {
	private:
		char AdderPtr , ServerPtr;
		struct queueElement_t fifo[Q_SIZE];
			
	public:
		char length;
		QueueClass();
		char put();
		char get(uint8_t *packet);
		bool broadcastChecker();
};

extern QueueClass rxQueue;

#endif