


#include "ai7688.h"
#include "RD_init.h"
#include "../Driver/inc/sci.h"
#include "../Driver/inc/tim4.h"
unsigned char readData[16];
rdPortStateTypeDef rdRelay;
int AiUartRead(ringbuffer_t *handle){
	if(handle->count >= 1){
		ring_pop_tail(handle,(void *)(&readData[0]));
		if( readData[0] != 0x55 ){ // header 1
			return 0;
		}
		ring_pop_tail(handle,(void *)(&readData[1]));
		if( readData[1] != 0xaa ){ // header 2
			return 0;
		}
		ring_pop_tail(handle,(void *)(&readData[2]));
		unsigned char messageLenght = readData[2];
		for (unsigned char i = 0; i < messageLenght;i++){
			ring_pop_tail(handle,(void *)(&readData[3+i]));
		}
		unsigned char checkSum = readData[3 + messageLenght - 1];
		//RD_EDIT: sau nghi check
		if(checkSum != createCheckSum(&readData[2],messageLenght)){
			return 0;
		}
		if(readData[3] != 0x03){
			return 0;
		}
		unsigned int operationCode = readData[4]<<8|readData[5];
		if(operationCode == 0x0001){
			unsigned char pin = readData[6];
			unsigned char state = readData[7];
			controlSingleRelayState(pin,state);
			unsigned char dataSend[8] = {0x55,0xaa,0x05,0x03,0x00,0x04,rdRelay.portState,0};
			dataSend[7] = createCheckSum(&dataSend[2],5);
			respondToAi7688(dataSend,8);
		}
		else if(operationCode == 0x0002){
			unsigned char portState = readData[6];
			controlAllRelayState(portState);
			unsigned char dataSend[8] = {0x55,0xaa,0x05,0x03,0x00,0x04,rdRelay.portState,0};
			dataSend[7] = createCheckSum(&dataSend[2],5);
			respondToAi7688(dataSend,8);
		}
		else if(operationCode == 0x0003){
			unsigned char dataSend[8] = {0x55,0xaa,0x05,0x03,0x00,0x04,rdRelay.portState,0};
			dataSend[7] = createCheckSum(&dataSend[2],5);
			respondToAi7688(dataSend,8);
		}
		else if(operationCode = 0x0005){
			unsigned char pwmId = readData[6];
			unsigned char pwmDuty = readData[7];
			//RD_EDIT: dimming ra chan uart BLE cua BAT32
			controlPWM(pwmId,pwmDuty);
			unsigned char dataSend[8] = {0x55,0xaa,0x06,0x03,0x00,0x06,pwmId,pwmDuty,0};
			dataSend[7] = createCheckSum(&dataSend[2],5);
			respondToAi7688(dataSend,8);
			//controlAllRelay(portState);
		}
	}
	else handle->ring_push_flag = 0;
	return 0;
}

void rdDelay(unsigned char time){
	for(unsigned int i=0;i<time;i++){
		for(unsigned char j=0;j<1000;j++){
		}
	}
}

void Ai7688MessageHandle(ringbuffer_t *handle){
	if(handle->ring_push_flag){
		if(GET_TICK - handle->ring_push_time_out >= 30){
			AiUartRead(handle);
		}
	}
}


unsigned char createCheckSum(unsigned char Data[], unsigned char size)
{
	  unsigned char BufferCounter;
	  unsigned char TempCreateCheckSum = 0;
	  for (BufferCounter = 0; BufferCounter < size; BufferCounter++)
	  {			
			TempCreateCheckSum ^= Data[BufferCounter];
	  }
	  return TempCreateCheckSum;
}


void controlSingleRelayState(unsigned char relayID, unsigned char relayState){
	//gpio_write(RELAY_PORT,relayID,relayState);
	relayState & 1 ? (rdRelay.portState |= (1 << relayID)) : (rdRelay.portState &= ~ (1 << relayID));
}

void controlAllRelayState(unsigned char allRelayState){
	rdRelay.portState = allRelayState;
}

void respondToAi7688(unsigned char *data,unsigned char lenght){
	UART2_Send(data,lenght);
}


/*brief : this function use to change pwm duty cycle
  pwmId: use to control correct pwm pin,when system need more than 1 pwm output
  pwmDuty: pwm duty
*/
void controlPWM(unsigned char pwmId, unsigned char pwmDuty){
	TM40_PWM_1Period_1Duty(RD_PWM_PERIOD,RD_PWM_DUTY_PERSCENT(pwmDuty));
}