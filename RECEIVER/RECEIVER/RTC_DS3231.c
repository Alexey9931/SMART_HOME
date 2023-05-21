/*
 * RTC.c
 *
 * Created: 16.12.2021 16:08:19
 *  Author: Alex2
 */ 

#include "RTC_DS3231.h"

//выбор режима редактирования
#define MODE_CHANGE_SECONDS 0
#define MODE_CHANGE_HOURS 1
#define MODE_CHANGE_MINUTES 2
#define MODE_CHANGE_DATE 3
#define MODE_CHANGE_MONTH 4
#define MODE_CHANGE_YEAR 5
#define MODE_CHANGE_WEAKDAY 6
#define MODE_CHANGE_ALARM_HOURS 7
#define MODE_CHANGE_ALARM_MINUTES 8

extern unsigned char clock_change_mode;
extern unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;

unsigned char RTC_ConvertFromBinDec(unsigned char c)//перевод в двоичный формат
{
	unsigned char ch = ((c/10)<<4)|(c%10);
	return ch;
}
unsigned char RTC_ConvertFromDec(unsigned char c)//перевод в десятичный формат
{
	unsigned char ch = ((c>>4)*10+(0b00001111&c));
	return ch;
}
// инициализация начальных установок
void RTC_init(void){
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010000);   // передача адреса устройства, режим записи
	I2C_SendByte(0x0E);            // передача адреса памяти
	I2C_SendByte(0b00100000);      // запустить преобразование температуры и выход на 1 Гц
	I2C_SendByte(0b00001000);      // разрешить выход 32 кГц
	I2C_StopCondition();                // остановка i2c
}
// получение времени и даты
void RTC_read_time(void){
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010000);   // передача адреса устройства, режим записи
	I2C_SendByte(0x00);            // передача адреса памяти
	I2C_StopCondition();                // остановка i2c
	
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010001);    // передача адреса устройства, режим чтения
	sec = RTC_ConvertFromDec(I2C_ReadByte());     // чтение секунд, ACK
	min = RTC_ConvertFromDec(I2C_ReadByte());     // чтение минут, ACK
	hour = RTC_ConvertFromDec(I2C_ReadByte());    // чтение часов, ACK
	day = RTC_ConvertFromDec(I2C_ReadByte());    // чтение день недели, ACK
	date = RTC_ConvertFromDec(I2C_ReadByte());     // чтение число, ACK
	month = RTC_ConvertFromDec(I2C_ReadByte());   // чтение месяц, ACK
	year = RTC_ConvertFromDec(I2C_ReadLastByte());    // чтение год, NACK
	I2C_StopCondition();                // остановка i2c	
}
// установка времени
void RTC_write_time(unsigned char hour1,unsigned char min1, unsigned char sec1){
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010000);   // передача адреса устройства, режим записи
	I2C_SendByte(0x00);            // передача адреса памяти
	I2C_SendByte(RTC_ConvertFromBinDec(sec1));       // 0x00 секунды (целесообразно ли задавать еще и секунды?)
	I2C_SendByte(RTC_ConvertFromBinDec(min1));       // 0x01 минуты
	I2C_SendByte(RTC_ConvertFromBinDec(hour1));      // 0x02 часы
	I2C_StopCondition();                // остановка i2c
}
// установка даты
void RTC_write_date(unsigned char wday, unsigned char day, unsigned char month, unsigned char year){	
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010000);   // передача адреса устройства, режим записи
	I2C_SendByte(0x03);        // передача адреса памяти
	I2C_SendByte(RTC_ConvertFromBinDec(wday));       // 0x03 день недели (воскресенье - 1, пн 2, вт 3, ср 4, чт 5, пт 6, сб 7)
	I2C_SendByte(RTC_ConvertFromBinDec(day));        // 0x04 день месяц
	I2C_SendByte(RTC_ConvertFromBinDec(month));      // 0x05 месяц
	I2C_SendByte(RTC_ConvertFromBinDec(year));       // 0x06 год
	I2C_StopCondition();                // остановка i2c
}
// чтение температуры
/*
void RTC_read_temper(void){
	I2C_StartCondition();                // запуск i2c
	I2C_SendByte(0b11010000);   // передача адреса устройства, режим записи
	I2C_SendByte(0x11);            // передача адреса памяти
	I2C_StopCondition();                // остановка i2c
	
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010001);   // передача адреса устройства, режим чтения
	t1 = I2C_ReadByte();           // чтение MSB температуры
	t2 = I2C_ReadLastByte();           // чтение LSB температуры
	I2C_StopCondition();                // остановка i2c
	
	t2=(t2/128);                    // сдвигаем на 6 - точность 0,25 (2 бита)
	// сдвигаем на 7 - точность 0,5 (1 бит)
	t2=t2*5;
}
*/
void ModifyRTC(void)
{
	I2C_StartCondition();
	I2C_SendByte(0b11010000);
	switch(clock_change_mode)
	{
		case MODE_CHANGE_SECONDS: //секунды
		{
			I2C_SendByte(0);//Переходим на 0x02 — байт секунд
			if (sec <= 59) I2C_SendByte(RTC_ConvertFromBinDec( sec ));
			else I2C_SendByte(RTC_ConvertFromBinDec(0));
			break;
		}
		case MODE_CHANGE_HOURS: //часы
		{
			I2C_SendByte(2);//Переходим на 0x02 — байт часов
			if (hour <= 23) I2C_SendByte(RTC_ConvertFromBinDec( hour ));
			else I2C_SendByte(RTC_ConvertFromBinDec(0));
			break;
		}
		case MODE_CHANGE_MINUTES: // минуты
		{
			I2C_SendByte(1);//Переходим на 0x01 — байт минут
			if(min<=59) I2C_SendByte(RTC_ConvertFromBinDec(min));
			else I2C_SendByte(RTC_ConvertFromBinDec(0));
			break;
		}
		case MODE_CHANGE_DATE: // дата
		{
			I2C_SendByte(4);//Переходим на 0x04 — байт даты
			if (month==2) //февраль
			{
				if(year%4==0) //високосный год
				{
					if(date<29) I2C_SendByte(RTC_ConvertFromBinDec(date));
					else I2C_SendByte(RTC_ConvertFromBinDec(1));
				}
				else
				{
					if(date<28) I2C_SendByte(RTC_ConvertFromBinDec(date));
					else I2C_SendByte(RTC_ConvertFromBinDec(1));
				}
			}
			else if ((month==4)|(month==6)|(month==9)|(month==11))
			{
				if(date<30) I2C_SendByte(RTC_ConvertFromBinDec(date));
				else I2C_SendByte(RTC_ConvertFromBinDec(1));
			}
			else
			{
				if(date<31) I2C_SendByte(RTC_ConvertFromBinDec(date));
				else I2C_SendByte(RTC_ConvertFromBinDec(1));
			}
			break;
		}
		case MODE_CHANGE_MONTH: // месяц
		{
			I2C_SendByte(5);//Переходим на 0x05 — байт месяца
			if(month<=12) I2C_SendByte(RTC_ConvertFromBinDec(month));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_YEAR: // год
		{
			I2C_SendByte(6);//Переходим на 0x06 — байт года
			if(year<99) I2C_SendByte(RTC_ConvertFromBinDec(year));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		case MODE_CHANGE_WEAKDAY: // день недели
		{
			I2C_SendByte(3);//Переходим на 0x03 — байт дня недели
			if(day<=7) I2C_SendByte(RTC_ConvertFromBinDec(day));
			else I2C_SendByte(RTC_ConvertFromBinDec(1));
			break;
		}
		/*case MODE_CHANGE_ALARM_HOURS: // часы будильника
		{
			if(alarmhour<23) EEPROM_write(2,alarmhour+1);
			else EEPROM_write(2,0);
			break;
		}
		case MODE_CHANGE_ALARM_MINUTES: // минуты будильника
		{
			if(alarmmin<59) EEPROM_write(3,alarmmin+1);
			else EEPROM_write(3,0);
			break;
		}*/
	}
	I2C_StopCondition();
}