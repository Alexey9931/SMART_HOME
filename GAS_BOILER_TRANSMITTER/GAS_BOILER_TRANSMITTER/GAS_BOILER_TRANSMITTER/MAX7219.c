/*
 * MAX7219.c
 *
 * Created: 19.06.2023 23:16:51
 *  Author: Alex2
 */ 
#include "MAX7219.h"

void Send_MAX7219(char rg, char dt)
{
	PORTC &= ~(1<<MAX7219_SS);
	asm("nop");
	spi_send_recv(rg);
	spi_send_recv(dt);
	PORTC |= (1<<MAX7219_SS);
	asm("nop");
}
void MAX7219_init()
{
	Send_MAX7219(0x09, 0xFF); //включим режим декодирования для всех разрядов 
	_delay_ms(10);
	Send_MAX7219(0x0B, 7); //сколько разрядов используем
	_delay_ms(10);
	Send_MAX7219(0x0A, 0x05); //яркость
	_delay_ms(10);
	Send_MAX7219(0x0C, 1); //включим индикатор
	_delay_ms(10);
	MAX7219_clear();
	
	Send_MAX7219(0x01, 1);
	Send_MAX7219(0x02, 2);
	Send_MAX7219(0x03, 3);
	Send_MAX7219(0x04, 4);
	Send_MAX7219(0x05, 5);
	Send_MAX7219(0x06, 6);
	Send_MAX7219(0x07, 7);
	Send_MAX7219(0x08, 8);
	_delay_ms(1000);

	PrintTemp_MAX7219(200, 261);
}
void MAX7219_clear()
{
	uint8_t i=8;
	 do
	 {
		 Send_MAX7219(i,0xF);//символ пустоты
	 } while (--i);
}
void Number_MAX7219(volatile long n)
{
	char ng = 0; //переменная для минуса
	if (n < 0)
	{
		ng = 1;
		n *= -1;
	}
	MAX7219_clear();
	if (n == 0) 
	{
		Send_MAX7219(0x01, 0); //в первый разряд напишем 0
		return;
	}
	char i = 0;
	do {
		Send_MAX7219(++i, n%10);
		n /= 10;
	} while (n);
	if (ng) {
		Send_MAX7219(i+1, 0xA); //символ —
	}
}
void PrintTemp_MAX7219(int current_temp, int setpoint_temp)
{
	
	uint8_t start_pos;
	start_pos = 2;

	MAX7219_clear();
	Send_MAX7219(0x09, 0xEE); //включим режим декодирования для всех разрядов кроме позиций 2 и 6
	Send_MAX7219(1, 0x4E);//вывод букв С
	Send_MAX7219(5, 0x4E);//вывод букв С
	if (current_temp == 0)
	{
		Send_MAX7219(start_pos, 0);
		Send_MAX7219(start_pos+1, 0x80);
	}
	else
	{
		while (current_temp != 0)
		{
			if((start_pos == 3)||(start_pos == 7))
			{
				Send_MAX7219(start_pos, (current_temp%10)|0x80);//вывод с точкой
			}
			else
			{
				Send_MAX7219(start_pos, current_temp%10);
			}
			start_pos++;
			current_temp /= 10;
		}
	}
	start_pos = 6;
	if (setpoint_temp == 0)
	{
		Send_MAX7219(start_pos, 0);
		Send_MAX7219(start_pos+1, 0x80);
	}
	else
	{
		while (setpoint_temp != 0)
		{
			if((start_pos == 3)||(start_pos == 7))
			{
				Send_MAX7219(start_pos, (setpoint_temp%10)|0x80);//вывод с точкой
			}
			else
			{
				Send_MAX7219(start_pos, setpoint_temp%10);
			}
			start_pos++;
			setpoint_temp /= 10;
		}
	}
}
