/*
 * eeprom.h
 *
 * Created: 19.11.2021 15:25:24
 *  Author: Alex2
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);
void EEPROM_write_word(unsigned int uiAddress, uint16_t ucData); //пишем 2х-байтную величину
uint16_t EEPROM_read_word(unsigned int uiAddress); //читаем 2х-байтную величину
void EEPROM_write_dword(unsigned int uiAddress, uint32_t ucData); //пишем 4х-байтную величину
uint32_t EEPROM_read_dword(unsigned int uiAddress);//читаем 4х-байтную величину
void EEPROM_write_string(unsigned int uiAddress, char str1[]); //пишем строку
const char* EEPROM_read_string(unsigned int uiAddress, unsigned int sz); //читаем строку

#endif /* EEPROM_H_ */