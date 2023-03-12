/*
 * DHT22.c
 *
 * Created: 20.03.2022 15:21:14
 *  Author: Alex2
 */ 
#include "DHT22.h"

uint8_t dht22_init (void)
{
	_delay_ms(2000);
	DDRB |= (1<<DHT22_PORT);   // set as output
	PORTB |= (1<<DHT22_PORT);// pull the pin high

	return 0;
}
uint8_t dht22_GetData (uint8_t *data)
{
	uint8_t i, j = 0;
	DDRB |= (1<<DHT22_PORT);   // set as output
	//reset ports
	PORTB &= ~(1<<DHT22_PORT);// pull the pin low
	PORTB |= (1<<DHT22_PORT);// pull the pin high
	_delay_ms(100);
	//start
	PORTB &= ~(1<<DHT22_PORT);// pull the pin low
	_delay_ms(18);
	PORTB |= (1<<DHT22_PORT);// pull the pin high
	//���� ������ �������
	DDRB &= ~(1<<DHT22_PORT);   // set as input
	_delay_us(60);//������������� �������� 60 � �� 40 ���
	//���� ������ �� ������� ������������� ����, �� ������
	if (PINB & (1 << DHT22_PORT))
	{
		return 0;
	}
	_delay_us(80);
	//���� ������ �� �������� ����, �� ������
	if (!(PINB & (1 << DHT22_PORT)))
	{
		return 0;
	}
	_delay_us(80);
	//������ ������ (���������� ����� � ������ ��������,
	//�.�. ������� ���������� �������, ����� �����
	//�� �������������� ������������ ���������
	for (j=0; j<5; j++)
	{
		data[4-j] = 0;
		for (i = 0; i<8; i++)
		{
			while(!(PINB & (1 << DHT22_PORT)));//���� ���������� ����
			_delay_us(35);
			if (PINB & (1 << DHT22_PORT))//������ ��������� �� ���������� 30 �����������
			//���� ���� �� ��� ����� �� ����������� � �����, �� ������ ��� �������, ����� ����
			data[4-j] |= (1<<(7-i));
			while(PINB & (1 << DHT22_PORT));//���� ���� ������ �������� ����(� ������ �������)
		}
	}
	return 1;
}