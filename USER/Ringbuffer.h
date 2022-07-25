
#ifndef GATEWAYMANAGER_RINGBUFFER_H_
#define GATEWAYMANAGER_RINGBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
	
	
#define RINGBUFFER_LEN 		(512)
#define true 1
#define false 0
extern volatile unsigned long count_timer;

#define GET_TICK	count_timer		//ms

typedef struct ringbuffer_t
{
	void *buffer;   // data buffer
	void *buffer_end; // end of data buffer
	size_t capacity; // maximum number of items in the buffer
	size_t count;   // number of items in the buffer
	size_t sz;    // size of each item in the buffer
	void *head;    // pointer to head
	void *tail;    // pointer to tail
	unsigned long ring_push_time_out;
	unsigned char ring_push_flag;
} ringbuffer_t;



// extern volatile unsigned char modbusUartBuff[RINGBUFFER_LEN];
// extern volatile unsigned char ai7688UartBuff[RINGBUFFER_LEN];
// extern volatile unsigned char bleUartBuff[RINGBUFFER_LEN];


extern  ringbuffer_t 		modbusUartRing;
extern  ringbuffer_t 		ai7688UartRing;
extern  ringbuffer_t 		bleUartRing;



unsigned char ring_init(ringbuffer_t *cb, unsigned int capacity, unsigned int sz,unsigned char *buff);

unsigned char ring_free(ringbuffer_t *cb);

unsigned char ring_push_head(ringbuffer_t *cb, const void *item);

unsigned char ring_pop_tail(ringbuffer_t *cb, void *item);

#ifdef __cplusplus
}
#endif



#endif /* GATEWAYMANAGER_RINGBUFFER_H_ */