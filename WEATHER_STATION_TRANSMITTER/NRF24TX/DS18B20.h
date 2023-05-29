/*
 * DS18B20.h
 *
 * Created: 02.01.2022 21:07:11
 *  Author: Alex2
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#include "main.h"
#define PORTTEMP PORTD
#define DDRTEMP DDRD
#define PINTEMP PIND
#define BITTEMP 7
#define NOID 0xCC //Пропустить идентификацию
#define T_CONVERT 0x44 //Код измерения температуры
#define READ_DATA 0xBE //Передача байтов ведущему

int dt_check(void); //функция преобразования показаний датчика в температуру
char dt_testdevice(void); //dt — digital termomether | определим, есть ли устройство на шине
unsigned char dt_readbyte(void);//функция чтения байта с устройства
char dt_readbit(void);//функция чтения бита с устройства
void dt_sendbyte(unsigned char bt);//функция записи байта на устройство
void dt_sendbit(char bt);//функция записи бита на устройство
char converttemp (unsigned int tt);//преобразование температуры в единицы

#endif /* DS18B20_H_ */