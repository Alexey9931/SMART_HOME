/*
 * NRF24-Test.c
 *
 * Created: 11.02.2022 20:28:22
 * Author : Alex2
 */ 

#include "main.h"

extern char temp_street[6];
extern char hum_street[5];
extern char temp_home[6];
extern char hum_home[5];
extern char WIND_speed[5];
extern char wind_direction[6];
extern char Vbat[5];
extern char Rain[6];
extern char Press_home[6];
extern char temp_street_to_DB[6];
extern char hum_street_to_DB[5];
extern char WIND_speed_to_DB[5];
extern char wind_direction_to_DB[6];
extern char Vbat_to_DB[5];
extern char Rain_to_DB[6];
extern struct Time_Parameters {
	char hours[4];
	char minutes[4];
	char seconds[4];
	char weakday[4];
	char mounthday[4];
	char Mounth[4];
	char Year[4];
} T_Param;
unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;
unsigned char clock_change_mode;
char receive_time[20] = "������ �� ����!";
char start_time[20] = {0};
char send_time[20] = "�������� �� ����!";
//uint8_t Frame_buffer[1024] = { 0 }; //����� �����
uint8_t ST7920_width = 128; //������ ������� � ��������
uint8_t ST7920_height = 64; //������ ������� � ��������
char DATA_TO_UART[80] = {0};
uint8_t menu_flag = 0;
uint8_t change_flag = 0;
int8_t add_cnt;
int8_t cnt;
uint8_t enc_int_flag = 0;
//uint8_t timer2_flag = 0;
//uint8_t timer1_flag = 0;
extern uint8_t rx_flag;
extern uint8_t receive_counter;
int32_t millis = 0;
uint8_t WiFi_Flag = 1;
//uint8_t wifi_count = 0;
//uint8_t rx_count = 0;
//int32_t time1 = 0;
//int32_t time2 = 0;

//-------------------------------------------------------------
/*void timer2_ini(void)//������ 8��
{	
	TCCR2A |= (1<<WGM21); // ������������� ����� ��� (����� �� ����������)
	TIMSK2 |= (1<<OCIE2A); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A(H � L)
	OCR2A = 0b11111010; //���������� � ������� ����� ��� ��������� 250
	TCCR2B |= (1<<CS22)|(1<<CS21);//��������� �������� 256.
}*/
//-------------------------------------------------------------
void timer1_ini(void)//������ 1��
{
	TCCR1B |= (1<<WGM12); // ������������� ����� ��� (����� �� ����������)
	TIMSK |= (1<<OCIE1A); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A(H � L)
	OCR1AH = 0b00000111; //���������� � ������� ����� ��� ��������� 2000
	OCR1AL = 0b11010000;
	TCCR1B |= (1<<CS11);//��������� �������� 8.
}
//-------------------------------------------------------------
void timer0_ini(void) // ������ 100���
{
	TCCR0 |= (1<<WGM01); // ������������� ����� ��� (����� �� ����������)
	OCR0 = 0b11001000; //���������� � ������� ����� ��� ��������� 200
	TCCR0 |= (1<<CS01); //�������� 8
	TIMSK |= (1<<OCIE0); //������������� ��� ���������� ���������� 0-��� �������� �� ���������� � OCR0A
}
//-------------------------------------------------------------
/*ISR (TIMER2_COMPA_vect)
{ 
	if (timer2_flag == 0) 
	{
		timer2_flag = 1;
	}
	else 
	{
		timer2_flag = 0;
	}
}*/
//-------------------------------------------------------------
ISR (TIMER1_COMPA_vect)
{	
	/*if (millis == 300001)
	{
		millis = 0;
	}*/
	millis++;
}
//-------------------------------------------------------------
ISR (TIMER0_COMP_vect)
{
	  static unsigned char new_state=0;
	  static unsigned char old_state=0;
	  
		  new_state = (PIND&0b00110000)>>6;
		  
		  switch(old_state | new_state)
		  {
			  case 0x01: case 0x0e:
			  if (change_flag) {add_cnt++;}
			  else {cnt++; PORTD |= (1<<LED);}
			  break;
			  case 0x04: case 0x0b:
			  if (change_flag) add_cnt--;
			  else {cnt--; PORTD |= (1<<LED);}
			  break;
		  }
		  old_state = new_state<<2;
		  //����������� ��������� cnt
		  if (cnt >= 120){cnt = 0;}
		  else if (cnt <= -1){cnt = 119;}
		  //����������� ��������� add_cnt
		  switch(cnt%7)
		  {
			  case 0:	if (add_cnt >= 24){add_cnt = 0;}
			  else if (add_cnt <= -1){add_cnt = 23;}
			  break;
			  case 1: if (add_cnt >= 60){add_cnt = 0;}
			  else if (add_cnt <= -1){add_cnt = 59;}
			  break;
			  case 2: if (add_cnt >= 8){add_cnt = 0;}
			  else if (add_cnt <= -1){add_cnt = 7;}
			  break;
			  case 3: if (add_cnt >= 32){add_cnt = 0;}
			  else if (add_cnt <= -1){add_cnt = 31;}
			  break;
			  case 4: if (add_cnt >= 13){add_cnt = 0;}
			  else if (add_cnt <= -1){add_cnt = 12;}
			  break;
			  case 5: if (add_cnt >= 100){add_cnt = 0;}
			  else if (add_cnt <= -1){add_cnt = 99;}
			  break;
		  }
		  PORTD &= ~(1<<LED);
}
//-------------------------------------------------------------
//���������� �� ������ �������� ��� ������������ � ���� ����
void enc_interrupt_ini(void)
{
	//������� ���������� INT1 �� ����������� ������
	MCUCR |= (1<<ISC11);
	//�������� ������� ���������� INT1
	GICR |= (1<<INT1);
}
//-------------------------------------------------------------
//���������� �� UART
/*ISR(USART_RX_vect)
{
	UCSR0B &= ~(1<<RXCIE0); 
	WiFi_Flag = 1;
 	_delay_us(300);
 	PORTD |= (1<<LED);
 	_delay_us(300);
	PORTD &= ~(1<<LED);
}*/
//-------------------------------------------------------------
//���������� �����.���������� �� ������ ��������
ISR(INT1_vect)
{
	//enc_int_flag = 1;
	switch (menu_flag)
	{
		case 0: menu_flag = 1;
		//cnt = 0;
		break;
		case 1:	switch(cnt%4)
		{
			case 0:	menu_flag = 0;
			break;
			case 1:	menu_flag = 2;
			break;
			case 2:	menu_flag = 3;
			break;
			case 3:	menu_flag = 4;
			break;
		}
		//cnt = 0;
		break;
		case 2:	switch(cnt%7)
		{
			case 0:	if(!change_flag)
			{
				add_cnt = hour;
				change_flag = 1;
			}
			else
			{
				hour = add_cnt;
				clock_change_mode = 1;
				ModifyRTC();
				change_flag = 0;
			}
			break;
			case 1:	if(!change_flag)
			{
				add_cnt = min;
				change_flag = 1;
			}
			else
			{
				min = add_cnt;
				clock_change_mode = 2;
				ModifyRTC();
				change_flag = 0;
			}
			break;
			case 2:	if(!change_flag)
			{
				add_cnt = day;
				change_flag = 1;
			}
			else
			{
				day = add_cnt;
				clock_change_mode = 6;
				ModifyRTC();
				change_flag = 0;
			}
			break;
			case 3:	if(!change_flag)
			{
				add_cnt = date;
				change_flag = 1;
			}
			else
			{
				date = add_cnt;
				clock_change_mode = 3;
				ModifyRTC();
				change_flag = 0;
			}
			break;
			case 4:	if(!change_flag)
			{
				add_cnt = month;
				change_flag = 1;
			}
			else
			{
				month = add_cnt;
				clock_change_mode = 4;
				ModifyRTC();
				change_flag = 0;
			}
			break;
			case 5:	if(!change_flag)
			{
				add_cnt = year;
				change_flag = 1;
			}
			else
			{
				year = add_cnt;
				clock_change_mode = 5;
				ModifyRTC();
				change_flag = 0;
			}
			break;
			case 6:	clock_change_mode = 0;
			menu_flag = 1;
			//cnt = 1;
			break;
		}
		break;
		case 3: menu_flag = 1;
		//cnt = 0;
		break;
		case 4: menu_flag = 1;
		//cnt = 0;
		break;
	}
}
//-------------------------------------------------------------
void SPI_init(void) //������������� SPI
{
	DDRB |= ((1<<SS)|(1<<MOSI)|(1<<CE)|(1<<SCK)); //����� SPI �� �����
	PORTB &= ~((1<<SS)|(1<<MOSI)|(1<<SCK)); //������ �������
	HIGH_CSN;
	SPCR = ((1<<SPE)|(1<<MSTR));//������� ����, ������� �������
}
//-------------------------------------------------------------
void port_init(void)
{
	//������������� ����� IRQ ��� ���� ����������
	DDRD &= ~(1<<IRQ);
	PORTD |= (1<<IRQ);
	//������������� ������ ��� ����������
	DDRD |= (1 << LED);//led
	PORTD &= ~ (1 << LED);
	
	//������������� ������ ��� ILI9486
	DATA_PORT=0x00;
	DATA_DDR=0xFF;//���� ������ �� �����
	COMMAND_DDR |= ((1<<LCD_CS)|(1<<LCD_CD)|(1<<LCD_WR)|(1<<LCD_RD)|(1<<LCD_RESET));//��������� ����� ����� ��� �� �����
	COMMAND_PORT &= ~((1<<LCD_CS)|(1<<LCD_CD)|(1<<LCD_WR)|(1<<LCD_RD)|(1<<LCD_RESET));
	//������������� ������ ��� LCD12864
	/*DDRD |= (1<<RS);//����� RS �� ����� ��� LCD12864
	PORTD &= ~(1<<RS);
	DDRD |= (1<<E);
	PORTD &= ~(1<<E);
	DDRD |= (1<<RW);
	PORTD &= ~(1<<RW);*/
	
	//������������� ��� ��������
	DDRD &= ~(1<<SW_ENC);
	DDRD &= ~((1<<DT_ENC)||(1<<CLK_ENC));
	PORTD |= (1 << SW_ENC);
	PORTD &= ~((1 << DT_ENC)|(1 << CLK_ENC));
	 //������������� ������ ��� SD �����
	/* PORTB|=(1<<SS_SD);
	 DDRB|=(1<<SS_SD);
	 PORTC|=(1<<MISO_SD)|(1<<MOSI_SD);
	 DDRC|=(1<<MOSI_SD)|(1<<SCK_SD);*/
} 
//-------------------------------------------------------------
uint8_t spi_send_recv(uint8_t data) // ������� � ��������� 1 ���� �� SPI, ���������� ���������� ��������
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
//-------------------------------------------------------------
int main(void)
{	
	uint8_t rx_count = 0;
	int32_t time1 = 0;
	int32_t time2 = 0;
	//�������� WDT
	WDTCR &= ~(1<<WDE);
	//�������� Watchdog �� 2 �
	wdt_enable(WDTO_2S);
	port_init();
	PORTD |= (1<<LED);
	//������������� �����������
	SPI_init();
	I2C_Init();
	USART_Init(8);    //������������� ������ USART �������� 115200	
	ILI9486_ini();
	//LCD_12864_ini();
	//������� �������

	//����� �����������
	Print_Hello_World();
	wdt_reset();
	//������������� ������������
	NRF24_ini();
	//RTC_init();
	//dht22_init();
	//BMP180_Calibration();
	wdt_reset();
	_delay_ms(1500);
	// ��������� ������� ��� DS3231(�������� 1 ���v)
	//RTC_write_time(13, 30, 0);
	//RTC_write_date(2, 28, 6, 22);
	//����� ���� ��������
	wdt_reset();
	Print_Download();
	wdt_reset();
	PORTD &= ~(1<<LED);
	//������������� �������� � ����������
	//timer2_ini();
	timer1_ini();
	timer0_ini();
	// ����������� ��������� ����������
	MCUCR |= (1<<ISC01) | (0<<ISC00);
	GICR |= (1<<INT0);
	enc_interrupt_ini();
	// � ��������� ��� ���������
	sei();
	//��������� ������������
	strcpy(temp_street_to_DB,"NULL");
	strcpy(hum_street_to_DB,"NULL");
	strcpy(WIND_speed_to_DB,"NULL");
	strcpy(wind_direction_to_DB,"NULL");
	strcpy(Vbat_to_DB,"NULL");
	strcpy(Rain_to_DB,"NULL");
	temp_street[0] = '0';
	temp_street[1] = '0';
	temp_street[2] = '.';
	temp_street[3] = '0';
	hum_street[0] = '0';
	hum_street[1] = '0';
	Vbat[0] = '0';
	Vbat[1] = '.';
	Vbat[2] = '0';
	Vbat[3] = '0';
	temp_home[0] = '0';
	temp_home[1] = '0';
	temp_home[2] = '.';
	temp_home[3] = '0';
	hum_home[0] = '0';
	hum_home[1] = '0';
	sprintf(WIND_speed,"0.00");
	/*WIND_speed[0] = '0';
	WIND_speed[1] = '.';
	WIND_speed[2] = '0';
	WIND_speed[3] = '0';*/
	Press_home[0] = '0';
	wind_direction[0] = '-';
	Rain[0] = '-';
	//�������� ������� ������ ������
	//Clock ();
	sprintf(start_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);	
	
	sprintf_HOME_Weath_Param();
	Print_Static_Home_Page();
    while (1) 
    {
		//����� ������ �� �����������
		if (rx_flag == 1)
		{
			time1 = millis;
			while(rx_count < 6)
			{
				/*time2 = millis;
				if ((time2-time1)>15000)
				{
					rx_flag = 0;
					receive_counter = 0;
					rx_count = 0;
					break;
				}
				if(rx_flag == 1)
				{
					_delay_us(300);
					PORTD |= (1<<LED);
					_delay_us(300);
					PORTD &= ~(1<<LED);
					NRF24L01_Receive();
					Clock ();
					sprintf(receive_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);
					rx_count++;
				}
				//���������� ��������� �� ������
				switch (menu_flag)
				{
					case 0:	Print_Home_Page();
					break;
					case 1:	Print_Menu_Page();
					break;
					case 2:	Print_Page_Clock_Settings();
					break;
					case 3:	Print_Page_About();
					break;
					case 4:	Print_Page_Dop_Info();
					break;
				}
				wdt_reset();*/
			}
			rx_count = 0;
			_delay_ms(1000);
		}
		else
		{
			//�������� ������ � ��
			 if(millis >= 300000)
 			{
				/*Clock ();
				sprintf(send_time,"%d:%d:%d,%d/%d/%d", hour, min, sec, date, month, year);
				sprintf_HOME_Weath_Param();
				sprintf(DATA_TO_UART,"%s %s %s %s %s %s %s %s %s %s ", temp_street_to_DB, temp_home, hum_street_to_DB, hum_home, Press_home, Rain_to_DB, Vbat_to_DB, WIND_speed_to_DB, wind_direction_to_DB, send_time);
				USART_Transmit(DATA_TO_UART);
				memset(DATA_TO_UART, 0, sizeof(char) * strlen(DATA_TO_UART));//������� �������
				millis = 0;*/
			}
			//���������� �������� ���������
			else if ((millis % 6000) == 0)
			{
				sprintf_HOME_Weath_Param();
				Update_Home_Page();
				Print_Home_Page();
				//sprintf_HOME_Weath_Param();
				//Print_Home_Page();
			}
			//���������� ����������� �� �������
			else if((millis % 53) == 0)
			{
				switch (menu_flag)
				{
					/*case 0:	Print_Home_Page();
					break;
					case 1:	Print_Menu_Page();
					break;
					case 2:	Print_Page_Clock_Settings();
					break;
					case 3:	Print_Page_About();
					break;
					case 4:	Print_Page_Dop_Info();
					break;*/
				}
			}
		}
		wdt_reset();
    }
}

