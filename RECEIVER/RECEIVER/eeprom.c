/*
 * eeprom.c
 *
 * Created: 19.11.2021 15:25:34
 *  Author: Alex2
 */ 
#include "eeprom.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)//1 байт
{
	while(EECR & (1<<EEPE)) //ждем освобождени€ флага окончани€ последней операцией с пам€тью
	{}
	EEAR = uiAddress; //”станавливаем адрес
	EEDR = ucData; //ѕищем данные в регистр
	EECR |= (1<<EEMPE); //–азрешаем запись
	EECR |= (1<<EEPE); //ѕишем байт в пам€ть
}
unsigned char EEPROM_read(unsigned int uiAddress)//1 байт
{
	while(EECR & (1<<EEPE))
	{} //ждем освобождени€ флага окончани€ последней операцией с пам€тью
	EEAR = uiAddress; //”станавливаем адрес
	EECR |= (1<<EERE); //«апускаем операцию считывани€ из пам€ти в регистр данных
	return EEDR; //¬озвращаем результат
}
void EEPROM_write_word(unsigned int uiAddress, uint16_t ucData)//2 байта
{
    EEPROM_write(uiAddress, (unsigned char) ucData);
    unsigned char dt = ucData>>8;
    EEPROM_write(uiAddress+1, dt);   
}
uint16_t EEPROM_read_word(unsigned int uiAddress)//2 байта
{
	uint16_t dt = EEPROM_read(uiAddress+1)*256;//считываем старшую часть
	asm("nop");
	dt += EEPROM_read(uiAddress);//считываем младшую часть
	return dt;
}
void EEPROM_write_dword(unsigned int uiAddress, uint32_t ucData)//4 байта
{
	EEPROM_write_word(uiAddress, (uint16_t) ucData);
	uint16_t dt = ucData>>16;
	EEPROM_write_word(uiAddress+2, dt);
}
uint32_t EEPROM_read_dword(unsigned int uiAddress)//4 байта
{
	uint32_t dt = EEPROM_read_word(uiAddress+2)*65536;
	asm("nop");
	dt += EEPROM_read_word(uiAddress);
	return dt;
}
void EEPROM_write_string(unsigned int uiAddress, char str1[])//строка
{
   wchar_t n;
   for(n=0;str1[n]!='\0';n++)
   EEPROM_write(uiAddress+n,str1[n]);
}
const char* EEPROM_read_string(unsigned int uiAddress, unsigned int sz)//строка
{
   unsigned int i;
   char* str1;
   str1 = (char *) realloc(NULL,sz);
   for (i=0;i<sz;i++)
   str1[i] = EEPROM_read(uiAddress+i);
   return str1;
}