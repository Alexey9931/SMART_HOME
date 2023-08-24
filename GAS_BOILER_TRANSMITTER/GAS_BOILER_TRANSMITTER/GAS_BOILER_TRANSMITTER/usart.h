/*
 * usart.h
 *
 * Created: 19.11.2021 0:05:39
 *  Author: Alex2
 */ 


#ifndef USART_H_
#define USART_H_

#include "main.h"

void USART_Init( unsigned int speed);
void USART_Transmit( char* data );
void USART_Transmit_bytes( uint8_t data[], uint8_t data_size );


#endif /* USART_H_ */