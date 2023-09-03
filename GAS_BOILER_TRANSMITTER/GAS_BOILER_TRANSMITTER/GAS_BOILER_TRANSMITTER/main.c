/*
 * NRF24-Test.c
 *
 * Created: 29.05.2023 12:50:22
 * Author : Alex2
 */ 

#include "main.h"

#define TX_PLOAD_WIDTH 5// �������� ������(���-�� ���� � ������)
extern uint8_t RX_BUF[TX_PLOAD_WIDTH];
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
uint16_t temp_setpoint;
extern uint8_t temp_setpoint_integer;
extern uint8_t temp_setpoint_fraction;
extern uint8_t home_temp_rx_integer;
extern uint8_t home_temp_rx_fraction;
extern uint8_t work_mode;

uint8_t home_temp_own_integer = 0;
uint8_t home_temp_own_fraction = 0;

/*
����� ������: 0-���� 1-������
*/
uint8_t work_mode = 0;

int32_t millis = 0;
int32_t millis_hometemp_update = 0;

uint8_t OK_BUTTON_FLAG = 0;
uint8_t UP_BUTTON_FLAG = 0;
uint8_t DOWN_BUTTON_FLAG = 0;

char DATA_TO_UART[80] = {0};

unsigned int TIM2_COUNT = 0;
void timer2_ini(void)//������ 100���
{
	TCCR2 |= (1<<WGM21); // ������������� ����� ��� (����� �� ����������)
	TIMSK |= (1<<OCIE2); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A(H � L)
	OCR2 = 0b11001000; //���������� � ������� ����� ��� ��������� 200
	TCCR2 |= (1<<CS21);//��������� �������� 8.
}
ISR (TIMER2_COMP_vect)
{
	if(OK_BUTTON_FLAG == 1)
	{
		if(((PINC&(1<<BUTTON_UP)) == 0) && (UP_BUTTON_FLAG == 0))
		{
			temp_setpoint = temp_setpoint_integer * 10 + temp_setpoint_fraction;
			temp_setpoint += 5;
			temp_setpoint_integer = temp_setpoint/10;
			temp_setpoint_fraction = temp_setpoint%10;
			UP_BUTTON_FLAG = 1;
			EEPROM_write(1, temp_setpoint_integer);
			EEPROM_write(2, temp_setpoint_fraction);
		}
		else if(((PINC&(1<<BUTTON_DOWN)) == 0) && (DOWN_BUTTON_FLAG == 0))
		{
			temp_setpoint = temp_setpoint_integer * 10 + temp_setpoint_fraction;
			temp_setpoint -= 5;
			temp_setpoint_integer = temp_setpoint/10;
			temp_setpoint_fraction = temp_setpoint%10;
			DOWN_BUTTON_FLAG = 1;
		    EEPROM_write(1, temp_setpoint_integer);
			EEPROM_write(2, temp_setpoint_fraction);
		}
		if ((PINC&(1<<BUTTON_UP)) != 0)
		{
			UP_BUTTON_FLAG = 0;
		}
		if ((PINC&(1<<BUTTON_DOWN)) != 0)
		{
			DOWN_BUTTON_FLAG = 0;
		}
	}
}
void timer1_ini(void)//������ 1��
{
	TCCR1B |= (1<<WGM12); // ������������� ����� ��� (����� �� ����������)
	TIMSK |= (1<<OCIE1A); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A(H � L)
	OCR1AH = 0b00000111; //���������� � ������� ����� ��� ��������� 2000
	OCR1AL = 0b11010000;
	TCCR1B |= (1<<CS11);//��������� �������� 8.
}
ISR (TIMER1_COMPA_vect)
{	
	millis++;
	if(millis > INT32_MAX)
	{
		millis = 0;
		millis_hometemp_update = 0;
	}
	//������ 10 ��� �������� ���������� � �������� ���������� ������ � ��
	if ((millis % 10000)==0)
	{
		gas_boiler_controller();
		//���������� � �� ������ � ����� ����
		sprintf(DATA_TO_UART,"%d %d.%d %d.%d %d ", gas_boiler_enable_flag, home_temp_rx_integer, home_temp_rx_fraction, temp_setpoint_integer, temp_setpoint_fraction, work_mode);
		USART_Transmit(DATA_TO_UART);
		memset(DATA_TO_UART, '\0', sizeof(char) * strlen(DATA_TO_UART));//������� �������
	}
}
//���������� �� ������� ������ OK
ISR (INT1_vect)
{
	if (OK_BUTTON_FLAG == 1) OK_BUTTON_FLAG = 0;
	else OK_BUTTON_FLAG = 1;
}
SPI_init(void) //������������� SPI
{
	DDRB |= ((1<<SS)|(1<<MOSI)|(1<<CE)|(1<<SCK)); //����� SPI �� �����
	PORTB &= ~((1<<SS)|(1<<MOSI)|(1<<SCK)); //������ �������
	HIGH_CSN;
	SPCR |= (1<<SPE)|(1<<MSTR);//������� ����, ������� �������
}
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
	//���������
	DDRD |= (1<<LED_BOILER_STATUS);
	PORTD &= ~(1<<LED_BOILER_STATUS);
	//���������
	DDRC |= (1<<LED_WIFI);
	PORTC &= ~(1<<LED_WIFI);
	//������������� �������
	DDRB |= (1<<MOSFET);
	PORTB &= ~(1<<MOSFET);
	//������������� ����������
	DDRC |= (1<<MAX7219_SS);
	PORTC |= (1<<MAX7219_SS);
	//��������� ������
	DDRD &= ~(1<<BUTTON_OK);
	PORTD |= (1<<BUTTON_OK);
	DDRC &= ~(1<<BUTTON_UP);
	PORTC |= (1<<BUTTON_UP);
	DDRC &= ~(1<<BUTTON_DOWN);
	PORTC |= (1<<BUTTON_DOWN);
	//������ �����������
	DDRTEMP &= ~(1<<BITTEMP);
	PORTTEMP &= ~(1<<BITTEMP);
}
//���������� ���������� �� UART
int uart_rx_count = 0;
char uart_rx_buffer[100];
char uart_message[100];
ISR(USART_RXC_vect)
{
	uart_rx_buffer[uart_rx_count] = UDR;
	if (uart_rx_buffer[uart_rx_count] == '/')
	{
		memcpy(uart_message,uart_rx_buffer,strlen(uart_rx_buffer)-1);
		memset(uart_rx_buffer,'\0',strlen(uart_rx_buffer));
		uart_rx_count = 0;
		//��������� ������ � wifi �� uart
		if (strstr(uart_message,"WiFi-OK")!=0)
		{
			PORTC |= (1<<LED_WIFI);
		}
		if (strstr(uart_message,"WiFi-ERROR")!=0)
		{
			PORTC &= ~(1<<LED_WIFI);
		}
	}
	else
	{
		uart_rx_count++;
	}
}
uint8_t spi_send_recv(uint8_t data) // ������� � ��������� 1 ���� �� SPI, ���������� ���������� ��������
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
int main(void)
{
	_delay_ms(1000);
	int k = 0;
	uint8_t data[5] = {0};
	port_init();
	PORTD |= (1<<LED_TX);
	PORTD |= (1<<LED_RX);
	PORTD |= (1<<LED_BOILER_STATUS);
	PORTC |= (1<<LED_WIFI);
	SPI_init();
	USART_Init(16);    //������������� ������ USART �������� 115200	
    NRF24_ini();
	_delay_ms(100);
	MAX7219_init();
	_delay_ms(1000);
	// ����������� ��������� ����������
	//----------------------
	MCUCR |= (1<<ISC01)|((1<<ISC11));
	GICR |= (1<<INT0)|(1<<INT1);
	//----------------------
	_delay_ms(2000);
	PORTD &= ~(1<<LED_TX);
	PORTD &= ~(1<<LED_RX);
	PORTD &= ~(1<<LED_BOILER_STATUS);
	PORTC &= ~(1<<LED_WIFI);
	timer1_ini();
	timer2_ini();
	gas_boiler_controller();
    //�������� WDT
    WDTCR &= ~(1<<WDE);
    //�������� Watchdog �� 2 �
    wdt_enable(WDTO_2S);
    ACSR |= (1<<ACD);//���� ����������
	sei();
	_delay_ms(1500);
	wdt_reset();
	_delay_ms(1500);
	wdt_reset();
	//��������� �� eeprom �������� ��� �����
	/*��������� eeprom:
	1-������� �����
	2-������� �����
	3-����������� �����
	4-����������� �����
	5-������ �����
	6-����� ������
	*/
	if(EEPROM_read(1) > 100) EEPROM_write(1,20);
	if(EEPROM_read(2) > 100) EEPROM_write(2,0);
	if(EEPROM_read(3) > 100) EEPROM_write(3,20);
	if(EEPROM_read(4) > 100) EEPROM_write(4,0);
	if(EEPROM_read(5) > 100) EEPROM_write(5,0);
	if(EEPROM_read(6) > 100) EEPROM_write(6,0);
	temp_setpoint_integer = EEPROM_read(1);
	temp_setpoint_fraction = EEPROM_read(2);
	home_temp_rx_integer = EEPROM_read(3);
	home_temp_rx_fraction = EEPROM_read(4);
	gas_boiler_enable_flag = EEPROM_read(5);
	work_mode = EEPROM_read(6);
	//���������� � �� ��������� ������ ����� ���������
	sprintf(DATA_TO_UART,"%d %d.%d %d.%d %d ", gas_boiler_enable_flag, home_temp_rx_integer, home_temp_rx_fraction, temp_setpoint_integer, temp_setpoint_fraction, work_mode);
	USART_Transmit(DATA_TO_UART);
	memset(DATA_TO_UART, '\0', sizeof(char) * strlen(DATA_TO_UART));//������� �������
	while (1) 
    {
		
		//���� ��� ����� ����� 15 ���, �� ����� ����������� �� ������������ ������� � ��������� � ����� ����
		if (abs(millis - millis_hometemp_update) > 15000)
		{
			work_mode = 0;
			home_temp_rx_integer = home_temp_own_integer;
			home_temp_rx_fraction = home_temp_own_fraction;
			millis_hometemp_update = millis;
			EEPROM_write(3, home_temp_rx_integer);
			EEPROM_write(4, home_temp_rx_fraction);
			EEPROM_write(6, work_mode);
		}
		//������ 3��� ���������� ������ � �������� �����������
		if ((millis%3000) == 0)
		{
			buf1[0] = gas_boiler_enable_flag;
			//���� ����� �������� �������
			if ((temp_setpoint_integer != RX_BUF[1]) || (temp_setpoint_fraction != RX_BUF[2]))
			{
				buf1[0] += 100;
			}
			buf1[1] = temp_setpoint_integer;
			buf1[2] = temp_setpoint_fraction;
			buf1[3] = home_temp_rx_integer;
			buf1[4] = home_temp_rx_fraction;
			dt = NRF24L01_Send(buf1);
			memset(buf1, 0, sizeof(int) * strlen(buf1));//������� �������
			DS18b0_find_temp();
		}
		//����� ���� �� ���������
		if ((millis%500) == 0)
		{
			PrintTemp_MAX7219(home_temp_rx_integer*10+home_temp_rx_fraction, temp_setpoint_integer*10+temp_setpoint_fraction);
		}
		wdt_reset();
    }
}

