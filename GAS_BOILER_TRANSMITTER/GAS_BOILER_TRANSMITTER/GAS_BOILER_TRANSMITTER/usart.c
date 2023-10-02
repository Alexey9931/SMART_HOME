/*
 * usart.c
 *
 * Created: 19.11.2021 0:05:26
 *  Author: Alex2
 */ 
#include "usart.h"


void USART_Init( unsigned int speed)//������������� ������ USART
{	
  UBRRH = (unsigned char)(speed>>8);
  UBRRL = (unsigned char)speed;
  UCSRB |= (1<<RXEN)|( 1<<TXEN); //�������� ����� � �������� �� USART
  UCSRB |= (1<<RXCIE); //��������� ���������� ��� ������
  UCSRA |= (1<<U2X); // ��� 8 ���, �������� ��������
  UCSRC |= (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);// ���������� ������ � �������� UCSRC (URSEL=1),
  //������������ ����� (UMSEL=0), ��� �������� �������� (UPM1=0 � UPM0=0),
  //2 ����-��� (USBS=1), 8-��� ������� (UCSZ1=1 � UCSZ0=1)
  //UCSRC |= (1<<UPM1);//��������
}
void USART_Transmit( char* data ) //������� �������� ������
{
  uint8_t i;
  
  for (i = 0; i < strlen(data); i++)
  {
	  while ( !(UCSRA & (1<<UDRE)) );//�������� ����������� ������ ������
	  UDR = data[i]; //������ �������� ������
  }
}
void USART_Transmit_bytes( uint8_t data[], uint8_t data_size ) //������� �������� ������
{
	uint8_t i;
	
	for (i = 0; i < data_size; i++)
	{
		while ( !(UCSRA & (1<<UDRE)) )//�������� ����������� ������ ������
		{
		}
		UDR = data[i]; //������ �������� ������
	}
}