#include "BAT32G139.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "userdefine.h"                 // Cmsemicon::Device:Startup
#include "sci.h"
#include <stdio.h>
#include <string.h>
#include "gpio.h"
#include "RD_init.h"
#include "wdt.h"
#include "Ringbuffer.h"
#include "modbus_rtu.h"
#include "ai7688.h"
#include "../Driver/inc/tim4.h"
extern ringbuffer_t 		modbusUartRing;
extern unsigned char AIReceiveUart;
extern  uint8_t data_tx2; 
extern rdPortStateTypeDef rdRelay;
volatile uint32_t g_ticks;
unsigned char duty = 100;
void rdControlTest();
void delayMS(uint32_t n)
{
	g_ticks = n;
	while(g_ticks);
}

void RD_uart_putarr(uint8_t *data,uint16_t lenght){
	for(int i = 0; i< lenght; i++){
		UART0_Send(data[i]);
		//delayMS(1);
	}
}




volatile unsigned long count_timer = 0;
extern unsigned char crcFail;
extern unsigned char crcDone;
int main (void){
	
	MD_STATUS status;
	uint32_t msCnt; 
	uint8_t tmp;
	SystemCoreClockUpdate();
	msCnt = SystemCoreClock / 1000;
	SysTick_Config(1*msCnt); 
	SystemCoreClockUpdate();
	
	status = UART0_Init(SystemCoreClock,9600);
//	PORT_Init(PORT7,PIN1,OUTPUT);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
	status = UART1_Init(SystemCoreClock,9600);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
	status = UART2_Init(SystemCoreClock,9600);
	if(status == MD_ERROR)
	{
		while(1); // The baud rate cannot get at the current system clock frequency.
	}
	INTC_EnableIRQ(SR0_IRQn);
	// RD_Init_check_buff_Screen();
	// RD_Init_check_buff_I2c();
	// RD_init_IO();
	tmp = RST->RESF;
	if(tmp & RST_RESF_WDTRF_Msk)
	{
		//da rst by wdt
		//				RD_uart_putarr("Hello, UART_i2c\n",19);
		//				UART1_Send("Hello, UART_screen\n",19);
		//				UART2_Send("Hello, UART2\n",13);
	}
	else{
		//chua rst by wdt
		//		RD_uart_putarr("Hella, UART_i2c\n",);
		//		UART1_Send("Hella, UART_screen\n",19);
		//		UART2_Send("Hella, UART2\n",13);
	}
	RD_init_ring();
	// RD_uart_putarr("uart0\n",19);
	// UART1_Send("uart1\n",19);
	// UART2_Send("uart2\n",13);
	setRelayState();
	
	while(1){		
		Ai7688MessageHandle(&ai7688UartRing);
		controlAllRelay(rdRelay.portState);
		WDT_Restart();
	}
}

void rdControlTest(){
	static unsigned char mainCnt = 0;
//	RD_uart_putarr("uart0\n",6);
//	UART1_Send("uart1\n",6);
//	UART2_Send("uart2\n",6);
}


void SysTick_Handler(void)
{
	//	WDT->WDTE = 0xAC;
	g_ticks--;
	count_timer++;
}
