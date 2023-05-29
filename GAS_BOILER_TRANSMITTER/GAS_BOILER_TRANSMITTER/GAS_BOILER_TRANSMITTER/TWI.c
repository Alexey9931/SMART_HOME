/*
 * TWI.c
 *
 * Created: 28.11.2021 19:33:09
 *  Author: Alex2
 */ 
#include "TWI.h"

void I2C_Init(void)
{
	TWBR=0x20; //скорость передачи (при 8 м√ц получаетс€ 100 к√ц)
}
void I2C_StartCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));//подождем пока установитс€ TWIN
}
void I2C_StopCondition(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}
void I2C_SendByte(unsigned char c)
{
	TWDR = c;//запишем байт в регистр данных
	TWCR = (1<<TWINT)|(1<<TWEN);//включим передачу байта
	while (!(TWCR & (1<<TWINT)));//подождем пока установитс€ TWIN
}
void I2C_SendByteByADDR(unsigned char c,unsigned char addr)
{
    I2C_StartCondition(); // ќтправим условие START
	I2C_SendByte(addr); // ќтправим в шину адрес устройства + бит чтени€-записи
	I2C_SendByte(c);// ќтправим байт данных
	I2C_StopCondition();// ќтправим условие STOP
}
unsigned char I2C_ReadByte(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));//ожидание установки бита TWIN
	return TWDR;//читаем регистр данных
}
unsigned char I2C_ReadLastByte(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));//ожидание установки бита TWIN
	return TWDR;//читаем регистр данных
}