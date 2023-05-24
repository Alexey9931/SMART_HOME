/*
 * usart.c
 *
 * Created: 19.11.2021 0:05:26
 *  Author: Alex2
 */ 
#include "usart.h"

extern uint8_t rx_flag;

void USART_Init( unsigned int speed)//Инициализация модуля USART
{	
  UBRR0H = (unsigned char)(speed>>8);
  UBRR0L = (unsigned char)speed;
  UCSR0B |= (1<<RXEN0)|( 1<<TXEN0); //Включаем прием и передачу по USART
  UCSR0B |= (1<<RXCIE0); //Разрешаем прерывание при приеме
  UCSR0A |= (1<<U2X0); // Для 8 мгц, удвоение скорости
  UCSR0C |= (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);// Обращаемся именно к регистру UCSRC (URSEL=1),
  //ассинхронный режим (UMSEL=0), без контроля четности (UPM1=0 и UPM0=0),
  //2 стоп-бит (USBS=1), 8-бит посылка (UCSZ1=1 и UCSZ0=1)
  //UCSRC |= (1<<UPM1);//четность
}
void USART_Transmit( char* data ) //Функция отправки данных
{
  uint8_t i;
  
  for (i = 0; i < strlen(data); i++)
  {
	  while ( !(UCSR0A & (1<<UDRE0)) );//Ожидание опустошения буфера приема
	  UDR0 = data[i]; //Начало передачи данных
  }
}
void USART_Transmit_bytes( uint8_t data[], uint8_t data_size ) //Функция отправки данных
{
	uint8_t i;
	
	for (i = 0; i < data_size; i++)
	{
		while ( !(UCSR0A & (1<<UDRE0)) )//Ожидание опустошения буфера приема
		{
		}
		UDR0 = data[i]; //Начало передачи данных
	}
}