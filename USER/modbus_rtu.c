


#include "modbus_rtu.h"


ModbusRxTypedef *modbusRxMessage;
unsigned char data_buff[32] = {0};
unsigned char slave_addr = 0x23;


rd_message_handle_t rd_modbus_function_code_list [NUM_OF_FUNCTION] = {
	{MODBUS_FUNC_CODE_READ_COIL,rd_modbus_read_coil_handle},
	{MODBUS_FUNC_CODE_READ_DISCRETE_INPUTS,rd_modbus_read_discrete_handle},
	{MODBUS_FUNC_CODE_READ_HOLDING_REG,rd_modbus_read_holding_register_handle},
	{MODBUS_FUNC_CODE_READ_INPUT_REG,rd_modbus_read_input_register_handle},
	{MODBUS_FUNC_CODE_WRITE_SINGLE_COID,rd_modbus_write_single_coid_handle},
	{MODBUS_FUNC_CODE_WRITE_SINGLE_REG,rd_modbus_write_single_byte_handle},
};



unsigned char readCoidData[8] = {0};
unsigned char readRegisterData[32] = {0};

//void modbus_send_ (unsigned char slave_addr, modbus_tdef functionCode,unsigned char start_addr,unsigned char lenght){
//	unsigned char buff_send[32] = {0};
//	modbusTxMessage.addr = slave_addr;
//	modbusTxMessage.function_code = functionCode;
//	modbusTxMessage.data[0]
//	buff_send
//	HAL_UART_Transmit(&huart2, buff_send, Size, Timeout), pData, Size, Timeout);
//
//}


void mobusSendNormalFunction (unsigned char slave_addr, modbus_tdef functionCode,unsigned short start_reg_addr,unsigned short lenght){
	modbusTxNormal_t modbusTxMessage;
	modbusTxMessage.addr = slave_addr;
	modbusTxMessage.function_code = functionCode;
	modbusTxMessage.start_reg_addr[0] = start_reg_addr>>8;
	modbusTxMessage.start_reg_addr[1] = start_reg_addr;
	modbusTxMessage.lenght[0] = lenght>>8;
	modbusTxMessage.lenght[1] = lenght;
	unsigned short crcRead =  modbus_rtu_crc((unsigned char *)&(modbusTxMessage),sizeof(modbusTxMessage));;
	modbusTxMessage.crc[0] = crcRead;
	modbusTxMessage.crc[1] = crcRead >> 8;
//	if(HAL_UART_Transmit(&huart2, (unsigned char *)(&modbusTxMessage), sizeof(modbusTxMessage),100) == HAL_OK){

//	}
}




int rd_modbus_read_coil_handle(ModbusRxTypedef messgae){
/*
	//RD_EDIT: thu tu cac byte read coil from 27 -> 51
	 * byte 1: tu coil 27->20   0xDA (11011010) -> coil 27 = 1 coil 20 = 0
	 * byte 2: tu coil 35->28
	 * byte 3: tu coil 43->36
	 * byte 4: tu coil 51->44
*/
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readCoidData[i];
	}
	return 0;
}


int rd_modbus_read_discrete_handle(ModbusRxTypedef messgae){
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readCoidData[i];
	}
	return 0;
}


int rd_modbus_read_holding_register_handle(ModbusRxTypedef messgae){
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readRegisterData[i];
	}
	return 0;
}

int rd_modbus_read_input_register_handle(ModbusRxTypedef messgae){
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readRegisterData[i];
	}
	return 0;
}

int rd_modbus_write_single_coid_handle(ModbusRxTypedef messgae){
	return 0;
}

int rd_modbus_write_single_byte_handle(ModbusRxTypedef messgae){
	return 0;
}



int rd_modbus_message_handle (ModbusRxTypedef temp){
	for(unsigned char i = 0;i<NUM_OF_FUNCTION;i++)
	if(temp.function_code  == rd_modbus_function_code_list[i].function_code){
		rd_modbus_function_code_list[i].cb_cmd_function(temp);
		return 0;
	}
	return -1;
}


unsigned short modbus_rtu_crc(unsigned char *buf, int len)
{
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
	  crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

	  for (int i = 8; i != 0; i--) {    // Loop over each bit
		if ((crc & 0x0001) != 0) {      // If the LSB is set
		  crc >>= 1;                    // Shift right and XOR 0xA001
		  crc ^= 0xA001;
		}
		else                            // Else LSB is not set
		  crc >>= 1;                    // Just shift right
	  }
  }
  return crc;
}

void uartRead(ringbuffer_t *handle){
	if(handle->count >= 1){
		ring_pop_tail(handle,(void *)(&modbusRxMessage->addr));
		if( modbusRxMessage->addr == slave_addr ){
			ring_pop_tail(handle,(void *)(&modbusRxMessage->function_code));
			ring_pop_tail(handle,(void *)(&modbusRxMessage->lenght));
			for (unsigned char i = 0; i < (modbusRxMessage->lenght);i++){
				ring_pop_tail(handle,(void *)(&modbusRxMessage->data[i]));
			}
			ring_pop_tail(handle,(void *)(modbusRxMessage->crc));
			ring_pop_tail(handle,(void *)(modbusRxMessage->crc + 1));
			modbusRxMessage->message_flag = 1;
		}
	}
}


void modbus_message_handle_ (ringbuffer_t *handle){
	if(handle->ring_push_flag){
		if(GET_TICK - handle->ring_push_time_out >= 100){
			handle->ring_push_flag = 0;
			uartRead(handle);
		}
	}
}


