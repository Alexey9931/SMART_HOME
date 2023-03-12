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
	//ждем ответа датчика
	DDRB &= ~(1<<DHT22_PORT);   // set as input
	_delay_us(60);//экспериментом вы€влено 60 а не 40 сек
	//если датчик не ответил прит€гиванием шины, то ошибка
	if (PINB & (1 << DHT22_PORT))
	{
		return 0;
	}
	_delay_us(80);
	//если датчик не отпустил шину, то ошибка
	if (!(PINB & (1 << DHT22_PORT)))
	{
		return 0;
	}
	_delay_us(80);
	//читаем данные (записываем байты в массив наоборот,
	//т.к. сначала передаетс€ старший, чтобы потом
	//не переворачивать двухбайтовый результат
	for (j=0; j<5; j++)
	{
		data[4-j] = 0;
		for (i = 0; i<8; i++)
		{
			while(!(PINB & (1 << DHT22_PORT)));//ждем отпускани€ шины
			_delay_us(35);
			if (PINB & (1 << DHT22_PORT))//читаем результат по прошествии 30 микросекунд
			//если шина за это врем€ не прит€нулась к земле, то значит это единица, иначе ноль
			data[4-j] |= (1<<(7-i));
			while(PINB & (1 << DHT22_PORT));//ждем пока датчик прит€нет шину(в случае единицы)
		}
	}
	return 1;
}