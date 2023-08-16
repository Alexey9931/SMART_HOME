/*
 * CFile1.c
 *
 * Created: 02.01.2022 21:06:48
 *  Author: Alex2
 */ 

#include "DS18B20.h"

extern uint8_t home_temp_own_integer;
extern uint8_t home_temp_own_fraction;

void DS18b0_find_temp(void)
{
	int tt = 0;
	tt = dt_check();
	uint8_t temp_sign = tt>>11;//вычисление знака температуры
	uint8_t temp_integer;//целая часть темп
	uint8_t temp_fraction;//дробная часть темп
	if (temp_sign == 0x00)
	{
		home_temp_own_fraction = tt & 0xF;
		home_temp_own_fraction = (home_temp_own_fraction<<1) + (home_temp_own_fraction<<3);// умножаем на 10
		home_temp_own_fraction = (home_temp_own_fraction>>4);//делим на 16 или умножаем на 0.0625
		home_temp_own_integer = (tt&0x07FF)>>4;
	}
	else
	{
		home_temp_own_fraction = ((~tt) & 0xF);
		home_temp_own_fraction = (home_temp_own_fraction<<1) + (home_temp_own_fraction<<3);// ”множаем на 10
		home_temp_own_fraction = (home_temp_own_fraction>>4);//делим на 16 или умножаем на 0.0625
		home_temp_own_integer = ((~(tt))&0x07FF)>>4;
	}
}
//функция преобразования показаний датчика в температуру
int dt_check(void)
{
  unsigned char bt;//переменная для считывания байта
  unsigned int tt=0;
  if(dt_testdevice()==1) //если устройство нашлось
  {
   dt_sendbyte(NOID); //пропустить идентификацию, тк у нас только одно устройство на шине
   dt_sendbyte(T_CONVERT); //измеряем температуру
   _delay_ms(188); //в 10 битном режиме преобразования — 188 милисекунд
   dt_testdevice(); //снова используем те же манипуляции с шиной что и при проверке ее присутствия
   dt_sendbyte(NOID); //пропустить идентификацию, тк у нас только одно устройство на шине
   dt_sendbyte(READ_DATA); //даем команду на чтение данных с устройства
   bt = dt_readbyte(); //читаем младший бит
   tt = dt_readbyte(); //читаем старший бит MS
   tt = (tt<<8)|bt;//сдвигаем старший влево, младший пишем на его место, тем самым получаем общий результат
  }
  return tt;
}
//функция определения датчика на шине
char dt_testdevice(void) //dt — digital termomether | определим, есть ли устройство на шине
{
  char stektemp=SREG;// сохраним значение стека
  cli(); //запрещаем прерывания
  char dt;
  DDRTEMP |= 1<<BITTEMP; //притягиваем шину
  _delay_us(485); //задержка как минимум на 480 микросекунд
  DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
  _delay_us(65); //задержка как минимум на 60 микросекунд
  if ((PINTEMP & (1<<BITTEMP))==0)//проверяем, ответит ли устройство
  {
	  dt=1;//устройство есть
  }
  else dt=0;//устройства нет
  SREG = stektemp;// вернем значение стека
  _delay_us(420); //задержка как минимум на 480 микросекунд, но хватит и 420, тк это с учетом времени прошедших команд
  return dt; //вернем результат
}
//функция чтения байта с устройства
unsigned char dt_readbyte(void)
{
	char c=0;
	char i;
	for(i=0;i<8;i++)
	{
	  c|=dt_readbit()<<i; //читаем бит
	}
	return c;
}
//функция чтения бита с устройства
char dt_readbit(void)
{
  char stektemp=SREG;// сохраним значение стека
  cli(); //запрещаем прерывание
  char bt; //переменная хранения бита
  DDRTEMP |= 1<<BITTEMP; //притягиваем шину
  _delay_us(2); //задержка как минимум на 2 микросекунды
  DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
  _delay_us(13);
  bt = (PINTEMP & (1<<BITTEMP))>>BITTEMP; //читаем бит
  _delay_us(45);
  SREG = stektemp;// вернем значение стека
  return bt; //вернем результат
}
//функция записи байта на устройство
void dt_sendbyte(unsigned char bt)
{
	char i;
	for(i=0;i<8;i++)//посылаем отдельно каждый бит на устройство
	{
		if((bt & (1<<i)) == 1<<i)//посылаем 1
		dt_sendbit(1);
		else //посылаем 0
		dt_sendbit(0);
	}
}
//функция записи бита на устройство
void dt_sendbit(char bt)
{
	char stektemp=SREG;// сохраним значение стека
	cli(); //запрещаем прерывание
	DDRTEMP |= 1<<BITTEMP; //притягиваем шину
	_delay_us(2); //задержка как минимум на 2 микросекунды
	if(bt)
    {
	  DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
	}
	_delay_us(65); //задержка как минимум на 60 микросекунд
	DDRTEMP &= ~(1<<BITTEMP); //отпускаем шину
	SREG = stektemp;// вернем значение стека
}
//преобразование температуры в единицы
char converttemp (unsigned int tt)
{
	char t = tt;//сдвиг и отсечение части старшего байта
	return t;
}