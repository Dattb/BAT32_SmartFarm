#include "Ringbuffer.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

volatile unsigned char modbusUartBuff[RINGBUFFER_LEN] = {0};
volatile unsigned char ai7688UartBuff[RINGBUFFER_LEN] = {0};
volatile unsigned char bleUartBuff[RINGBUFFER_LEN] = {0};

ringbuffer_t 		modbusUartRing;
ringbuffer_t 		ai7688UartRing;
ringbuffer_t 		bleUartRing; 

unsigned char ring_init(ringbuffer_t *cb, unsigned int capacity, unsigned int sz,unsigned char *buff){
	cb->buffer = ai7688UartBuff;  // cap phat bo nho dong
	if(cb->buffer == NULL){
		//printf("ring_init FAIL! \n");
        //UARTest_UartPutString("false");
		return false;
	}
	cb->buffer_end = (char *)cb->buffer + capacity * sz;
	cb->capacity = capacity;
	cb->count = 0;
	cb->sz = sz;
	cb->head = cb->buffer;
	cb->tail = cb->buffer;
	return true;
}


unsigned char ring_free(ringbuffer_t *cb)
{
	if(cb->buffer){
		free(cb->buffer);
		cb->buffer = NULL;
	}else{
		return false;
	}
	return true;
}

unsigned char ring_push_head(ringbuffer_t *cb, const void *item)
{
	cb->ring_push_time_out = GET_TICK;
	cb->ring_push_flag = 1;
	if(cb->count == cb->capacity){
		//uartCsend_485("day",3);
		return false;
	}
	memcpy(cb->head, item, cb->sz);
	cb->head = (char*)cb->head + cb->sz;
	if(cb->head == cb->buffer_end)
	cb->head = cb->buffer;
//	if(cb->head >= cb->tail){
//		cb->count = cb->head - cb->tail;
//	}
//	else if(cb->head < cb->tail){
//		cb->count = cb->head + RINGBUFFER_LEN - cb->tail;
//	}
	cb->count++;
	return true;
}

unsigned char ring_pop_tail(ringbuffer_t *cb, void *item)
{
	if(cb->count == 0){
		//printf("Ringbuffer is Empty ! \n");
		return false;
	}
	memcpy(item, cb->tail, cb->sz);
	cb->tail = (char*)cb->tail + cb->sz;
	if(cb->tail == cb->buffer_end)
		cb->tail = cb->buffer;
//	if(cb->head >= cb->tail){
//		cb->count = cb->head - cb->tail;
//	}
//	else if(cb->head < cb->tail){
//		cb->count = cb->head + RINGBUFFER_LEN - cb->tail;
//	}
	cb->count--;
	return true;
}