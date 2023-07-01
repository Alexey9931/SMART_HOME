/*
 * NRF24-Test.c
 *
 * Created: 29.05.2023 12:50:22
 * Author : Alex2
 */ 

#include "main.h"

uint8_t buf1[10] = {0};//������ ��� ��������
uint8_t  reg = 0x00;
uint8_t retr_cnt, dt;
uint16_t retr_cnt_full;
//uint8_t data[5];
unsigned int hall_counter = 0;
unsigned int speed = 0;
char speed_str[10] = {0};
char temperature_humidity_str[10] = {0};
char wind_direction_str[10] = {0};
char Vbat[10] = {0};
char RainAmount[10] = {0};		
int adc_value1 = 0, adc_value2 = 0;	
int INTER_COUNT;
uint8_t gas_boiler_enable_flag = 0;
extern uint8_t temp_setpoint_integer;
extern uint8_t temp_setpoint_fraction;

unsigned int TIM2_COUNT = 0;
void timer2_ini(void)//������ 0.008�
{
	TCCR2 |= (1<<WGM21); // ������������� ����� ��� (����� �� ����������)
	OCR2 = 0b11111010; //���������� � ������� ����� ��� ��������� 250
	TCCR2 |= ((1<<CS21)|(1<<CS22));//��������� �������� 256.
    TIMSK |= (1<<OCIE2); //������������� ��� ���������� ���������� 2��� �������� �� ���������� � OCR2	
}
//���������������
ISR(TIMER2_COMP_vect)
{
  if (TIM2_COUNT == 39750) TIM2_COUNT = 0;
  else TIM2_COUNT++;
}
void timer_ini(void)//������ 2���
{
	TCCR1B |= (1<<WGM12); // ������������� ����� ��� (����� �� ����������)
	TIMSK |= (1<<OCIE1A); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A(H � L)
	OCR1AH = 0b00111101; //���������� � ������� ����� ��� ���������
	OCR1AL = 0b00001001;
	TCCR1B |= (1<<CS12)|(1<<CS10);//��������� �������� 1024.
}
ISR (TIMER1_COMPA_vect)
{
   speed = hall_counter;
   hall_counter = 0;
}
SPI_init(void) //������������� SPI
{
	DDRB |= ((1<<SS)|(1<<MOSI)|(1<<CE)|(1<<SCK)); //����� SPI �� �����
	PORTB &= ~((1<<SS)|(1<<MOSI)|(1<<SCK)); //������ �������
	HIGH_CSN;
	SPCR |= (1<<SPE)|(1<<MSTR);//������� ����, ������� �������, �������� 16
}
//-------------------------------------------------------------
port_init(void)
{
	//������������� ����� IRQ ��� ���� ����������
	DDRD &= ~(1<<IRQ);
	PORTD |= (1<<IRQ);
	//��������� 
	DDRD |= (1<<LED_TX);
	PORTD &= ~(1<<LED_TX);
	//���������
	DDRD |= (1<<LED_RX);
	PORTD &= ~(1<<LED_RX);
	//������������� �������
	DDRB |= (1<<MOSFET);
	PORTB &= ~(1<<MOSFET);
	//������������� ����������
	DDRC |= (1<<MAX7219_SS);
	PORTC |= (1<<MAX7219_SS);
}
//-------------------------------------------------------------
uint8_t spi_send_recv(uint8_t data) // ������� � ��������� 1 ���� �� SPI, ���������� ���������� ��������
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

int main(void)
{
	int k = 0;
	uint8_t data[5] = {0};
	port_init();
	PORTD |= (1<<LED_TX);
	PORTD |= (1<<LED_RX);
	SPI_init();
    NRF24_ini();
	_delay_ms(100);
	MAX7219_init();
	_delay_ms(1000);
	// ����������� ��������� ����������
	//----------------------
	MCUCR |= (1<<ISC01) ;
	GICR |= (1<<INT0);
	//----------------------
	_delay_ms(1000);
	PORTD &= ~(1<<LED_TX);
	PORTD &= ~(1<<LED_RX);
	WDTCR &= ~(1<<WDE);//���� WDT
	ACSR |= (1<<ACD);//���� ����������
    //INTER_COUNT == 0;
	timer_ini();
	timer2_ini();
	sei();
	//PrintTemp_MAX7219(200, temp_setpoint_integer*10+temp_setpoint_fraction);
    while (1) 
    {

		//PrintTemp_MAX7219(200, temp_setpoint_integer*10+temp_setpoint_fraction);
		//�������� �����������
		buf1[0] = gas_boiler_enable_flag;
		int tt = 0;
		/*tt = dt_check();
		uint8_t temp_sign = tt>>11;//���������� ����� �����������
		uint8_t temp_integer;//����� ����� ����
		uint8_t temp_fraction;//������� ����� ����
		if (temp_sign == 0x00)
		{
			temp_fraction = tt & 0xF;
			temp_fraction = (temp_fraction<<1) + (temp_fraction<<3);// �������� �� 10
			temp_fraction = (temp_fraction>>4);//����� �� 16 ��� �������� �� 0.0625
			temp_integer = (tt&0x07FF)>>4;
		}
		else
		{
			temp_fraction = ((~tt) & 0xF);
			temp_fraction = (temp_fraction<<1) + (temp_fraction<<3);// �������� �� 10
			temp_fraction = (temp_fraction>>4);//����� �� 16 ��� �������� �� 0.0625
			temp_integer = ((~(tt))&0x07FF)>>4;
		}
		buf1[1] = temp_sign;
		buf1[2] = temp_integer;
		buf1[3] = temp_fraction;*/
		dt = NRF24L01_Send(buf1);
		/*PORTD |= (1<<LED_TX);
		_delay_ms(100);
		PORTD &= ~(1<<LED_TX);*/
		memset(buf1, 0, sizeof(int) * strlen(buf1));//������� �������
		_delay_ms(2000);
    }
}

