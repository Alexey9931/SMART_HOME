/*
 * HTU21D.c
 *
 * Created: 21.03.2023 0:38:42
 *  Author: Alex2
 */ 
#include "HTU21D.h"

#define HTU21D_W 0x80
#define HTU21D_R 0x81

uint16_t HTU21D_get_temperature()
{
	// промежуточные переменные
	uint16_t D1;
	uint16_t D2;
	uint16_t D3;
	uint16_t result;
	I2C_StartCondition();              
	I2C_SendByte(HTU21D_W);        
	I2C_SendByte(0xF3);	 
	I2C_StopCondition(); 
	
	_delay_ms(85);         
     
	I2C_StartCondition();
	I2C_SendByte(HTU21D_R);
  
	D1=I2C_ReadByte();            
	D2=I2C_ReadByte();  
	D3=I2C_ReadLastByte();                          
	I2C_StopCondition();                
	result = (uint16_t)((D1 << 8) | D2);
	result &= 0xFFFC;


	return result;  
}

uint16_t HTU21D_get_humidity()
{
	// промежуточные переменные
	uint16_t D1;
	uint16_t D2;
	uint16_t D3;
	uint16_t result;
	I2C_StartCondition();
	I2C_SendByte(HTU21D_W);
	I2C_SendByte(0xF5);
	I2C_StopCondition();
	
	_delay_ms(85);
	
	I2C_StartCondition();
	I2C_SendByte(HTU21D_R);
	
	D1=I2C_ReadByte();
	D2=I2C_ReadByte();
	D3=I2C_ReadLastByte();
	I2C_StopCondition();
	result = (uint16_t)((D1 << 8) | D2);
	result &= 0xFFFC;


	return result;
}