/*
 * eeprom.c
 *
 * Created: 19.11.2021 15:25:34
 *  Author: Alex2
 */ 
#include "eeprom.h"

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)//1 ����
{
	while(EECR & (1<<EEWE)) //���� ������������ ����� ��������� ��������� ��������� � �������
	{}
	EEAR = uiAddress; //������������� �����
	EEDR = ucData; //����� ������ � �������
	EECR |= (1<<EEMWE); //��������� ������
	EECR |= (1<<EEWE); //����� ���� � ������
}
unsigned char EEPROM_read(unsigned int uiAddress)//1 ����
{
	while(EECR & (1<<EEWE))
	{} //���� ������������ ����� ��������� ��������� ��������� � �������
	EEAR = uiAddress; //������������� �����
	EECR |= (1<<EERE); //��������� �������� ���������� �� ������ � ������� ������
	return EEDR; //���������� ���������
}
void EEPROM_write_word(unsigned int uiAddress, uint16_t ucData)//2 �����
{
    EEPROM_write(uiAddress, (unsigned char) ucData);
    unsigned char dt = ucData>>8;
    EEPROM_write(uiAddress+1, dt);   
}
uint16_t EEPROM_read_word(unsigned int uiAddress)//2 �����
{
	uint16_t dt = EEPROM_read(uiAddress+1)*256;//��������� ������� �����
	asm("nop");
	dt += EEPROM_read(uiAddress);//��������� ������� �����
	return dt;
}
void EEPROM_write_dword(unsigned int uiAddress, uint32_t ucData)//4 �����
{
	EEPROM_write_word(uiAddress, (uint16_t) ucData);
	uint16_t dt = ucData>>16;
	EEPROM_write_word(uiAddress+2, dt);
}
uint32_t EEPROM_read_dword(unsigned int uiAddress)//4 �����
{
	uint32_t dt = EEPROM_read_word(uiAddress+2)*65536;
	asm("nop");
	dt += EEPROM_read_word(uiAddress);
	return dt;
}
void EEPROM_write_string(unsigned int uiAddress, char str1[])//������
{
   wchar_t n;
   for(n=0;str1[n]!='\0';n++)
   EEPROM_write(uiAddress+n,str1[n]);
}
const char* EEPROM_read_string(unsigned int uiAddress, unsigned int sz)//������
{
   unsigned int i;
   char* str1;
   str1 = (char *) realloc(NULL,sz);
   for (i=0;i<sz;i++)
   str1[i] = EEPROM_read(uiAddress+i);
   return str1;
}