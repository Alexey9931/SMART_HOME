/*
 * TWI.h
 *
 * Created: 28.11.2021 19:32:59
 *  Author: Alex2
 */ 


#ifndef TWI_H_
#define TWI_H_

#include "main.h"

void I2C_Init(void);
void I2C_StartCondition(void);
void I2C_StopCondition(void);
void I2C_SendByte(unsigned char c);
void I2C_SendByteByADDR(unsigned char c,unsigned char addr);
unsigned char I2C_ReadByte(void);
unsigned char I2C_ReadLastByte(void);


#endif /* TWI_H_ */