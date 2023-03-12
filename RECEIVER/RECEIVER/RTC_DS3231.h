/*
 * RTC.h
 *
 * Created: 16.12.2021 16:08:31
 *  Author: Alex2
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "main.h"

unsigned char RTC_ConvertFromDec(unsigned char c); //перевод двоично-десятичного числа в десятичное
unsigned char RTC_ConvertFromBinDec(unsigned char c); //перевод десятичного числа в двоично-десятичное
void ModifyRTC(void);
void RTC_init(void);
void RTC_read_time(void);
void RTC_write_time(unsigned char hour1,unsigned char min1, unsigned char sec1);
void RTC_write_date(unsigned char wday, unsigned char day, unsigned char month, unsigned char year);
void RTC_read_temper(void);


#endif /* RTC_H_ */