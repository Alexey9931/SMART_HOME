/*
 * usart.c
 *
 * Created: 19.11.2021 0:05:26
 *  Author: Alex2
 */ 
#include "usart.h"


void USART_Init( unsigned int speed)//Инициализация модуля USART
{	
  UBRRH = (unsigned char)(speed>>8);
  UBRRL = (unsigned char)speed;
  UCSRB |= (1<<RXEN)|( 1<<TXEN); //Включаем прием и передачу по USART
  UCSRB |= (1<<RXCIE); //Разрешаем прерывание при приеме
  UCSRA |= (1<<U2X); // Для 8 мгц, удвоение скорости
  UCSRC |= (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);// Обращаемся именно к регистру UCSRC (URSEL=1),
  //ассинхронный режим (UMSEL=0), без контроля четности (UPM1=0 и UPM0=0),
  //2 стоп-бит (USBS=1), 8-бит посылка (UCSZ1=1 и UCSZ0=1)
  //UCSRC |= (1<<UPM1);//четность
}
void USART_Transmit( char* data ) //Функция отправки данных
{
  uint8_t i;
  
  for (i = 0; i < strlen(data); i++)
  {
	  while ( !(UCSRA & (1<<UDRE)) );//Ожидание опустошения буфера приема
	  UDR = data[i]; //Начало передачи данных
  }
}
void USART_Transmit_bytes( uint8_t data[], uint8_t data_size ) //Функция отправки данных
{
	uint8_t i;
	
	for (i = 0; i < data_size; i++)
	{
		while ( !(UCSRA & (1<<UDRE)) )//Ожидание опустошения буфера приема
		{
		}
		UDR = data[i]; //Начало передачи данных
	}
}