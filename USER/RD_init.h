#ifndef _RD_INIT_
#define _RD_INIT_

//#include "Ringbuffer.h"
#include "BAT32G139.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "userdefine.h"                 // Cmsemicon::Device:Startup
#include "sci.h"
#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "Ringbuffer.h"





typedef struct{
	union{
		unsigned char portState;
		struct{
			unsigned char relay0				:1;// Low significan bit
            unsigned char relay1				:1;
            unsigned char relay2				:1;
            unsigned char relay3				:1;
            unsigned char relay4				:1;
            unsigned char relay5				:1;
            unsigned char relay6				:1;
            unsigned char relay7				:1;// Most significan bit
		};
	};
}rdPortStateTypeDef;



/*define pin input*/
#define PORT_BTN0	PORT2
#define Pin_0		PIN3

#define PORT_BTN1	PORT6
#define Pin_1		PIN2

#define PORT_BTN2	PORT6
#define Pin_2		PIN1

#define PORT_BTN3	PORT6
#define Pin_3		PIN0

#define PORT_BTN4	PORT13
#define Pin_4		PIN6

/*define control relay*/
#define PORT_RELAY1	PORT3
#define RL_1		PIN1 
#define PORT_RELAY2	PORT7
#define RL_2		PIN5
#define PORT_RELAY3	PORT7
#define RL_3		PIN4
#define PORT_RELAY4	PORT7
#define RL_4		PIN3

/*define relay FB status*/
#define PORT_RL_FB1	PORT7
#define FB_1		PIN2
#define PORT_RL_FB2	PORT7
#define FB_2		PIN1
#define PORT_RL_FB3	PORT7
#define FB_3		PIN0
#define PORT_RL_FB4	PORT3
#define FB_4		PIN0
/*define pincontrol 485*/

#define PORT_485	PORT1
#define PIN_485		PIN2


#define PORT_LED	PORT4
#define PIN_LED		PIN1

extern void RD_uart_putarr(uint8_t *data,uint16_t lenght);


void RD_init_ring(void);




void setRelayState();
void rdGpioSet(PORT_TypeDef PORTx,uint8_t data);
void testRelay();


void controlAllRelay(unsigned char dataPort);
void uartRead1(ringbuffer_t *handle);












#endif
