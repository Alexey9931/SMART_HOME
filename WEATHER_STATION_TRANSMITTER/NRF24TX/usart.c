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
  UCSRB=(1<<RXEN)|( 1<<TXEN); //�������� ����� � �������� �� USART
  UCSRB |= (1<<RXCIE); //��������� ���������� ��� ������
  UCSRA |= (1<<U2X); // ��� 8 ���, �������� ��������
  UCSRC = (1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);// ���������� ������ � �������� UCSRC (URSEL=1),
  //������������ ����� (UMSEL=0), ��� �������� �������� (UPM1=0 � UPM0=0),
  //1 ����-��� (USBS=0), 8-��� ������� (UCSZ1=1 � UCSZ0=1)
  //UCSRC |= (1<<UPM1);//��������
}
void USART_Transmit( unsigned char data ) //������� �������� ������
{
  while ( !(UCSRA & (1<<UDRE)) ); //�������� ����������� ������ ������
  UDR = data; //������ �������� ������
}
int USART_Transmit_str(char* str)
{
	for(int i=0; i<strlen(str); i++)		//fixed ��. ���������� (25.07.2020)
	{
		while(!(UCSRA & (1<<UDRE))){}; // wait ready of port
		UDR = str[i];
	}
	return 0;
}