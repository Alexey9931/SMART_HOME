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
	char weakday[10];
	char mounthday[4];
	char Mounth[4];
	char Year[4];
} T_Param;
unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;
unsigned char clock_change_mode;
char receive_time[20] = "No any receptions!";
char start_time[20] = {0};
char send_time[30] = "There was no sending!";
//char DATA_TO_UART[80] = {0};
/*
menu_flag
0 - домашняя страница
1 - меню
2 - настройка времени
3 - о проекте
4 - доп инфо
*/
uint8_t menu_flag;
int8_t up_down_count = 0;

extern uint8_t rx_flag;
extern uint8_t receive_counter;
int32_t millis = 0;
uint8_t WiFi_Flag = 1;

uint8_t HomePage_Flag;
uint8_t MainMenuPage_Flag;
uint8_t TimeSettingsPage_Flag;
uint8_t PageAbout_Flag;
uint8_t AddInfoPage_Flag;

uint8_t timer0_flag = 0;
int timer0_counter = 0;

//-------------------------------------------------------------
/*void timer2_ini(void)//период 8мс
{	
	TCCR2A |= (1<<WGM21); // устанавливаем режим СТС (сброс по совпадению)
	TIMSK2 |= (1<<OCIE2A); //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)
	OCR2A = 0b11111010; //записываем в регистр число для сравнения 250
	TCCR2B |= (1<<CS22)|(1<<CS21);//установим делитель 256.
}*/
//-------------------------------------------------------------
void timer1_ini(void)//период 1мс
{
	TCCR1B |= (1<<WGM12); // устанавливаем режим СТС (сброс по совпадению)
	TIMSK1 |= (1<<OCIE1A); //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)
	OCR1AH = 0b00000111; //записываем в регистр число для сравнения 2000
	OCR1AL = 0b11010000;
	TCCR1B |= (1<<CS11);//установим делитель 8.
}
//-------------------------------------------------------------
void timer0_ini(void) // период 100мкс
{
	TCCR0B |= (1<<WGM01); // устанавливаем режим СТС (сброс по совпадению)
	OCR0A = 0b11001000; //записываем в регистр число для сравнения 200
	TCCR0B |= (1<<CS01); //делитель 8
	TIMSK0 |= (1<<OCIE0A); //устанавливаем бит разрешения прерывания 0-ого счетчика по совпадению с OCR0A
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
//обработчик прерываний по таймеру 0 для отслеживания нажатий кнопок
ISR (TIMER0_COMPA_vect)
{
	/*timer0_counter++;
	if (timer0_counter == 4000)
	{
		timer0_flag = 0;
		timer0_counter = 0;
	}
	if ((!(PINE & (1<<ButtonOK))) && (timer0_flag == 0))
	{
		timer0_flag = 1;
		if (menu_flag == 0)
		{
			up_down_count = 0;
			menu_flag = 1;
		}
		else if (menu_flag == 1)
		{
			switch(up_down_count%4)
			{
				case 0:
					up_down_count = 0;
					menu_flag = 0;
					break;
				case 1:
					up_down_count = 0;
					menu_flag = 2;
					break;
				case 2:
					up_down_count = 0;
					menu_flag = 3;
					break;
				case 3:
					up_down_count = 0;
					menu_flag = 4;
					break;
			}
		}
		else if (menu_flag == 2)
		{
			switch (up_down_count%8)
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					up_down_count = 0;
					menu_flag = 1;
					break;	
			}
		}
		else if (menu_flag == 3)
		{
			up_down_count = 0;
			menu_flag = 1;
		}
		else if (menu_flag == 4)
		{
			up_down_count = 0;
			menu_flag = 1;
		}
	}
	else if ((!(PINE & (1<<ButtonDown))) && (timer0_flag == 0))
	{
		timer0_flag = 1;
		up_down_count++;
		if (up_down_count > 15)
		{
			up_down_count = 0;
		}
	}
	else if ((!(PINE & (1<<ButtonUp))) && (timer0_flag == 0))
	{
		timer0_flag = 1;
		up_down_count--;
		if (up_down_count < 0)
		{
			up_down_count = 15;
		}
	}
	else if ((!(PINE & (1<<ButtonHome))) && (timer0_flag == 0))
	{
		timer0_flag = 1;
		up_down_count = 0;
		menu_flag = 0;
	}
	*/
	/*if ((!(PINE & (1<<ButtonOK))) || (!(PINE & (1<<ButtonHome)) || (!(PINE & (1<<ButtonUp)) || (!(PINE & (1<<ButtonDown))))))
	{
		TCCR0B &= ~(1<<CS01); //стоп таймер
		PORTL &= ~ (1 << BUZZER);
		_delay_us(50);
		PORTL |= (1 << BUZZER);
		TCCR0B |= (1<<CS01); //делитель 8
		timer0_counter = 0;
	}*/
}
//-------------------------------------------------------------
//прерывания по нажатию кнопок
ISR (INT4_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(1000);
	PORTL |= (1 << BUZZER);
	if (menu_flag == 0)
	{
		up_down_count = 0;
		menu_flag = 1;
	}
	else if (menu_flag == 1)
	{
		switch(up_down_count%4)
		{
			case 0:
			up_down_count = 0;
			menu_flag = 0;
			break;
			case 1:
			up_down_count = 0;
			menu_flag = 2;
			break;
			case 2:
			up_down_count = 0;
			menu_flag = 3;
			break;
			case 3:
			up_down_count = 0;
			menu_flag = 4;
			break;
		}
	}
	else if (menu_flag == 2)
	{
		switch (up_down_count%8)
		{
			case 0:
			break;
			case 1:
			break;
			case 2:
			break;
			case 3:
			break;
			case 4:
			break;
			case 5:
			break;
			case 6:
			break;
			case 7:
			up_down_count = 0;
			menu_flag = 1;
			break;
		}
	}
	else if (menu_flag == 3)
	{
		up_down_count = 0;
		menu_flag = 1;
	}
	else if (menu_flag == 4)
	{
		up_down_count = 0;
		menu_flag = 1;
	}
}
ISR (INT5_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(1000);
	PORTL |= (1 << BUZZER);
	up_down_count = 0;
	menu_flag = 0;
}
ISR (INT6_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(1000);
	PORTL |= (1 << BUZZER);
	PORTL &= ~(1<<LED);
	up_down_count--;
	if (up_down_count < 0)
	{
		up_down_count = 15;
	}
}
ISR (INT7_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(1000);
	PORTL |= (1 << BUZZER);
	up_down_count++;
	if (up_down_count > 15)
	{
		up_down_count = 0;
	}
}
//прерывания по UART
/*ISR(USART_RX_vect)
{
	UCSR0B &= ~(1<<RXCIE0); 
	WiFi_Flag = 1;
 	_delay_us(300);
 	PORTB |= (1<<LED);
 	_delay_us(300);
	PORTB &= ~(1<<LED);
}*/
//-------------------------------------------------------------
//обработчик внешн.прерываний от кнопки энкодера
/*ISR(INT1_vect)
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
}*/
//-------------------------------------------------------------
void SPI_init(void) //инициализация SPI
{
	DDRB |= ((1<<SS)|(1<<MOSI)|(1<<CE)|(1<<SCK)); //ножки SPI на выход
	PORTB &= ~((1<<SS)|(1<<MOSI)|(1<<SCK)); //низкий уровень
	HIGH_CSN;
	SPCR = ((1<<SPE)|(1<<MSTR));//включим шину, объявим ведущим
}
//-------------------------------------------------------------
void port_init(void)
{
	//инициализация ножки IRQ для внеш прерывания
	DDRE &= ~(1<<IRQ);
	PORTE |= (1<<IRQ);
	//инициализация портов для светодиода
	DDRL |= (1 << LED);//led
	PORTL &= ~ (1 << LED);
	//инициализация портов для пищалки
	DDRL |= (1 << BUZZER);
	PORTL |= (1 << BUZZER);
	//инициализация портов для ILI9486
	DATA_PORT=0x00;
	DATA_DDR=0xFF;//Шина данных на выход
	COMMAND_DDR |= ((1<<LCD_CS)|(1<<LCD_CD)|(1<<LCD_WR)|(1<<LCD_RD)|(1<<LCD_RESET));//Командные лапки также все на выход
	COMMAND_PORT &= ~((1<<LCD_CS)|(1<<LCD_CD)|(1<<LCD_WR)|(1<<LCD_RD)|(1<<LCD_RESET));
	//настройка кнопок
	DDRE &= ~(1<<ButtonOK);
	PORTE |= (1<<ButtonOK);
	DDRE &= ~(1<<ButtonHome);
	PORTE |= (1<<ButtonHome);
	DDRE &= ~(1<<ButtonUp);
	PORTE |= (1<<ButtonUp);
	DDRE &= ~(1<<ButtonDown);
	PORTE |= (1<<ButtonDown);
} 
//-------------------------------------------------------------
uint8_t spi_send_recv(uint8_t data) // Передаёт и принимает 1 байт по SPI, возвращает полученное значение
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
	//настрока WDT
	WDTCSR &= ~(1<<WDE);
	//включаем Watchdog на 8 с
	wdt_enable(WDTO_8S);
	port_init();
	PORTL |= (1<<LED);
	//Инициализация интерфейсов
	SPI_init();
	I2C_Init();
	USART_Init(8);    //Инициализация модуля USART скорость 115200	
	ILI9486_ini();
	//Вывод приветствия
	Print_Hello_World();
	wdt_reset();
	//Инициализация оборудования
	//NRF24_ini();
	RTC_init();
	//dht22_init();
	//BMP180_Calibration();
	wdt_reset();
	_delay_ms(1500);
	// Установка времени для DS3231(делается 1 разv)
	//RTC_write_time(22, 17, 0);
	//RTC_write_date(1, 13, 3, 23);
	//Вывод окна загрузки
	wdt_reset();
	//Print_Download();
	wdt_reset();
	PORTL &= ~(1<<LED);
	//Инициализация таймеров и прерываний
	//timer2_ini();
	timer1_ini();
	timer0_ini();
	// настраиваем параметры прерывания
	EICRB |= (1<<ISC41) | (1<<ISC51) | (1<<ISC61) | (1<<ISC71);
	EIMSK |= (1<<INT4) | (1<<INT5) | (1<<INT6) | (1<<INT7);
	//enc_interrupt_ini();
	//Начальная конфигурация
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
	//Фиксация времени начала работы
	Clock ();
	sprintf(start_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);	
	sprintf_HOME_Weath_Param();
	Print_Static_Home_Page();
	Print_Home_Page_In();
	Print_Home_Page_Out();
	HomePage_Flag = 0;
	MainMenuPage_Flag = 1;
	sei();
    while (1) 
    {
		//прием данных от передатчика
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
					PORTB |= (1<<LED);
					_delay_us(300);
					PORTB &= ~(1<<LED);
					NRF24L01_Receive();
					Clock ();
					sprintf(receive_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);
					rx_count++;
				}
				//обновление показаний на экране
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
			//отправка данных в БД
			if(millis >= 300000)
 			{
				/*Clock ();
				sprintf(send_time,"%d:%d:%d,%d/%d/%d", hour, min, sec, date, month, year);
				sprintf_HOME_Weath_Param();
				sprintf(DATA_TO_UART,"%s %s %s %s %s %s %s %s %s %s ", temp_street_to_DB, temp_home, hum_street_to_DB, hum_home, Press_home, Rain_to_DB, Vbat_to_DB, WIND_speed_to_DB, wind_direction_to_DB, send_time);
				USART_Transmit(DATA_TO_UART);
				memset(DATA_TO_UART, 0, sizeof(char) * strlen(DATA_TO_UART));//очистка массива*/
				millis = 0;
			}
			//обновление домашних показаний
			else if ((millis % 2010) == 0)
			{
				//sprintf_HOME_Weath_Param();
			}
			//обновление изображения на дисплее
			else if((millis % 100) == 0)
			{
				switch (menu_flag)
				{
					case 0:	
						if (HomePage_Flag == 1)
						{
							sprintf_HOME_Weath_Param();
							Print_Static_Home_Page();
							Print_Home_Page_In();
							Print_Home_Page_Out();
							HomePage_Flag = 0;
							MainMenuPage_Flag = 1;
							TimeSettingsPage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 1;
						}
						else
						{
							sprintf_HOME_Weath_Param();
							Print_Home_Page_In();
						}
						break;
					case 1:	
						if (MainMenuPage_Flag == 1)
						{
							Print_Menu_Page_Static();
							Print_Menu_Page();
							MainMenuPage_Flag = 0;
							HomePage_Flag = 1;
							TimeSettingsPage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 1;
						}
						else
						{
							Print_Menu_Page();
						}
						break;
					case 2:	
						if (TimeSettingsPage_Flag == 1)
						{
							Print_Page_Clock_Settings_Static();
							Print_Page_Clock_Settings();
							TimeSettingsPage_Flag = 0;
							MainMenuPage_Flag = 1;
							HomePage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 1;
						}
						else
						{
							Print_Page_Clock_Settings();
						}
						break;
					case 3:	
						if(PageAbout_Flag == 1)
						{
							Print_Page_About();
							TimeSettingsPage_Flag = 1;
							MainMenuPage_Flag = 1;
							HomePage_Flag = 1;
							PageAbout_Flag = 0;
							AddInfoPage_Flag = 1;
						}
						else
						{
							
						}
						break;
					case 4:	
						if(AddInfoPage_Flag == 1)
						{
							Print_Page_Dop_Info();
							TimeSettingsPage_Flag = 1;
							MainMenuPage_Flag = 1;
							HomePage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 0;
						}
						else
						{
							
						}
					break;
				}
			}
		}
		wdt_reset();
    }
}

