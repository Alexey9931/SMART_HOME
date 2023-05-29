/*
 * adc.c
 *
 * Created: 19.11.2021 17:07:06
 *  Author: Alex2
 */ 
#include "adc.h"
//�������������-
extern unsigned int adc_value1;//���� �������
extern unsigned int adc_value2;//���-�� ��������
//�������������-
char high_adc=0,low_adc=0;
int count = 0;

//�������������-
void ADC_Init(void)
{
   ADCSRA |= (1<<ADEN);//��� ���
   ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//���������� 64���
  // ADMUX |= (1<<REFS0)|(1<<REFS1);//��� �� ���� 2.56�
  //�������� ����� ��� ADC0 ��� ��������� ���� ������������
}
//�������������-
void ADC_convert (void)
{
	ADCSRA |= (1<<ADSC);//������ ���������
	while((ADCSRA&(1<<ADSC)));
	if (count == 0)
	{
		//��������� ����� ��� ��� ��������� ���� ������������
		ADMUX |= (1<<MUX0);//�������� ����� ��� ��� ��������� ���-�� �����
		count++;
		adc_value1 = ADC;
	}
	else
	{
		ADMUX &= ~(1<<MUX0);//��������� ����� ��� ��� ��������� ���-�� �����
		//�������� ����� ��� ��� ��������� ���� ������������
		count--;
		adc_value2 = ADC;
	}
}