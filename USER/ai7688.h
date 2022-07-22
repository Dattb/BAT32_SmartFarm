


#include "Ringbuffer.h"


#define	RELAY_PORT	PORT2

typedef struct {
	unsigned char header[2];
	unsigned char lenght;
	unsigned char data[8];
}ai7688MessageTypeDef;

typedef struct {
	unsigned char function_code ;
	int(*cb_cmd_function)(ai7688MessageTypeDef messgae);
}rdAi7688MessageHandleTypeDef;

int AiUartRead(ringbuffer_t *handle);
void rdDelay(unsigned char time);
void Ai7688MessageHandle(ringbuffer_t *handle);
unsigned char createCheckSum(unsigned char Data[], unsigned char size);
void controlSingleRelay(unsigned char relayID, unsigned char relayState);