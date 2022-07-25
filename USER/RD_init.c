#include "RD_init.h"
#include "Ringbuffer.h"


extern unsigned char modbusUartBuff[];
extern unsigned char ai7688UartBuff[];
extern unsigned char bleUartBuff[];

void RD_init_ring(void){
//	ring_init(&modbusUartRing, RINGBUFFER_LEN, sizeof(uint8_t),modbusUartBuff);
	ring_init(&ai7688UartRing, RINGBUFFER_LEN, sizeof(uint8_t),ai7688UartBuff);
//	ring_init(&bleUartRing, RINGBUFFER_LEN, sizeof(uint8_t),bleUartBuff);
}
 


void uartRead1(ringbuffer_t *handle){
	
}



void setRelayState(){ 
	PORT_Init(PORT2,PIN0,OUTPUT);
	PORT_Init(PORT2,PIN1,OUTPUT);
	PORT_Init(PORT2,PIN2,OUTPUT);
	PORT_Init(PORT2,PIN3,OUTPUT);
	PORT_Init(PORT2,PIN4,OUTPUT);
	PORT_Init(PORT2,PIN5,OUTPUT);
	PORT_Init(PORT2,PIN6,OUTPUT);
	PORT_Init(PORT2,PIN7,OUTPUT);
}

void rdGpioSet(PORT_TypeDef PORTx,uint8_t data){
	if(data == 1){
		PORT_SetBit(PORT2,PIN0);
		PORT_SetBit(PORT2,PIN1);
		PORT_SetBit(PORTx,PIN2);
		PORT_SetBit(PORTx,PIN3);
		PORT_SetBit(PORTx,PIN4);
		 PORT_SetBit(PORTx,PIN5);
		PORT_SetBit(PORTx,PIN6);
		PORT_SetBit(PORTx,PIN7);
	}
	else{
		PORT_ClrBit(PORT2,PIN0);
		 PORT_ClrBit(PORT2,PIN1);
		PORT_ClrBit(PORTx,PIN2);
		PORT_ClrBit(PORTx,PIN3);
		PORT_ClrBit(PORTx,PIN4);
		PORT_ClrBit(PORTx,PIN5);
		PORT_ClrBit(PORTx,PIN6);
		PORT_ClrBit(PORTx,PIN7);
	}
}

void testRelay(){
	static unsigned char checkTimeCnt = 0;
	static unsigned char stateRelay = 0;
	checkTimeCnt ++;

	if(checkTimeCnt > 7){
		checkTimeCnt = 0;
		stateRelay = !stateRelay;
	}
	gpio_write(PORT2,checkTimeCnt,stateRelay);
	delayMS(20);
}




void controlAllRelay(unsigned char dataPort){
	rdPortStateTypeDef relayGroup;
	relayGroup.portState = dataPort;
	if(relayGroup.relay0){
		gpio_write(PORT2,PIN0,1);
	}
	else{
		gpio_write(PORT2,PIN0,0);
	}

	if(relayGroup.relay1){
		gpio_write(PORT2,PIN1,1);
	}
	else{
		gpio_write(PORT2,PIN1,0);
	}

	if(relayGroup.relay2){
		gpio_write(PORT2,PIN2,1);
	}
	else{
		gpio_write(PORT2,PIN2,0);
	}

	if(relayGroup.relay3){
		gpio_write(PORT2,PIN3,1);
	}
	else{
		gpio_write(PORT2,PIN3,0);
	}

	if(relayGroup.relay4){
		gpio_write(PORT2,PIN4,1);
	}
	else{
		gpio_write(PORT2,PIN4,0);
	}

	if(relayGroup.relay5){
		gpio_write(PORT2,PIN5,1);
	}
	else{
		gpio_write(PORT2,PIN5,0);
	}

	if(relayGroup.relay6){
		gpio_write(PORT2,PIN6,1);
	}
	else{
		gpio_write(PORT2,PIN6,0);
	}

	if(relayGroup.relay7){
		gpio_write(PORT2,PIN7,1);
	}
	else{
		gpio_write(PORT2,PIN7,0);
	}
}




