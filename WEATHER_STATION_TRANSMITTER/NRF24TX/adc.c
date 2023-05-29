/*
 * adc.c
 *
 * Created: 19.11.2021 17:07:06
 *  Author: Alex2
 */ 
#include "adc.h"
//—————————————-
extern unsigned int adc_value1;//напр батареи
extern unsigned int adc_value2;//кол-во оасадков
//—————————————-
char high_adc=0,low_adc=0;
int count = 0;

//—————————————-
void ADC_Init(void)
{
   ADCSRA |= (1<<ADEN);//вкл АЦП
   ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//устанвоили 64кГЦ
  // ADMUX |= (1<<REFS0)|(1<<REFS1);//ист оп напр 2.56В
  //включаем канал АЦП ADC0 для измерения напр аккумулятора
}
//—————————————-
void ADC_convert (void)
{
	ADCSRA |= (1<<ADSC);//начало измерений
	while((ADCSRA&(1<<ADSC)));
	if (count == 0)
	{
		//выключаем канал АЦП для измерения напр аккумулятора
		ADMUX |= (1<<MUX0);//включаем канал АЦП для измерения кол-ва дождя
		count++;
		adc_value1 = ADC;
	}
	else
	{
		ADMUX &= ~(1<<MUX0);//выключаем канал АЦП для измерения кол-ва дождя
		//включаем канал АЦП для измерения напр аккумулятора
		count--;
		adc_value2 = ADC;
	}
}