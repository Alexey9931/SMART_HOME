/*
 * MAX7219.h
 *
 * Created: 19.06.2023 23:17:03
 *  Author: Alex2
 */ 


#ifndef MAX7219_H_
#define MAX7219_H_

#include "main.h"
#define MAX7219_SS PORTC0

void Send_MAX7219(char rg, char dt);
void MAX7219_init();
void MAX7219_clear();
void Number_MAX7219(volatile long n);
uint8_t PrintTemp_MAX7219(int current_temp, int setpoint_temp);


#endif /* MAX7219_H_ */