/*
 * adc.c
 *
 * Created: 19.11.2021 17:07:06
 *  Author: Alex2
 */ 
#include "adc.h"
//覧覧覧覧覧覧�-
extern unsigned int adc_value1;//�瑜� 矜�瑩裴
extern unsigned int adc_value2;//���-粽 �瑰琅���
//覧覧覧覧覧覧�-
char high_adc=0,low_adc=0;
int count = 0;

//覧覧覧覧覧覧�-
void ADC_Init(void)
{
   ADCSRA |= (1<<ADEN);//粲� ﾀﾖﾏ
   ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//���瑙粽齏� 64�ﾃﾖ
  // ADMUX |= (1<<REFS0)|(1<<REFS1);//頌� �� �瑜� 2.56ﾂ
  //粲���瑯� �瑙琿 ﾀﾖﾏ ADC0 蓁� 韈�褞褊�� �瑜� 瑕����������
}
//覧覧覧覧覧覧�-
void ADC_convert (void)
{
	ADCSRA |= (1<<ADSC);//�璞琿� 韈�褞褊韜
	while((ADCSRA&(1<<ADSC)));
	if (count == 0)
	{
		//糺����瑯� �瑙琿 ﾀﾖﾏ 蓁� 韈�褞褊�� �瑜� 瑕����������
		ADMUX |= (1<<MUX0);//粲���瑯� �瑙琿 ﾀﾖﾏ 蓁� 韈�褞褊�� ���-籵 蒡趾�
		count++;
		adc_value1 = ADC;
	}
	else
	{
		ADMUX &= ~(1<<MUX0);//糺����瑯� �瑙琿 ﾀﾖﾏ 蓁� 韈�褞褊�� ���-籵 蒡趾�
		//粲���瑯� �瑙琿 ﾀﾖﾏ 蓁� 韈�褞褊�� �瑜� 瑕����������
		count--;
		adc_value2 = ADC;
	}
}