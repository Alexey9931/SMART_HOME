/*
 * NRF24-Test.c
 *
 * Created: 11.02.2022 20:28:22
 * Author : Alex2
 */ 

#include "main.h"

extern uint8_t home_temp_sign;//знак домаш темп
extern uint8_t home_temp_integer;//целая часть домаш темп
extern uint8_t home_temp_fraction;//дробная часть домаш темп
extern int home_hum_integer;//целая часть домаш влажн

extern uint8_t street_temp_sign;//знак уличн темп
extern uint8_t street_temp_integer;//целая часть уличн темп
extern uint8_t street_temp_fraction;//дробная часть уличн темп
extern int street_hum_integer;//целая часть уличн влажн
extern uint8_t wind_speed_integer;//целая часть ск ветра
extern uint8_t wind_speed_fraction;//дробная часть ск ветра
extern uint8_t rain; //целая часть осадков
extern uint8_t V_Bat_integer;//целая часть заряда акб
extern uint8_t V_Bat_fraction;//дробная часть заряда акб
extern char wind_direction[6];//направление ветра

extern char temp_street_to_DB[6];
extern char hum_street_to_DB[5];
extern char WIND_speed_to_DB[5];
extern char wind_direction_to_DB[6];
extern char Vbat_to_DB[5];
extern char Rain_to_DB[6];
extern char temp_home_to_DB[6];
extern char hum_home_to_DB[5];
extern char Press_home_to_DB[5];
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
char receive_time[20] = "Нет сигнала";
char start_time[20] = {0};
char send_time[30] = "Отправки не было";
char rx_time_gas_boiler[20] = "Нет сигнала";
char DATA_TO_UART[80] = {0};
uint8_t rx_count;
/*
menu_flag
0 - домашняя страница
1 - меню
2 - настройка времени
3 - о проекте
4 - доп инфо
5 - состояние wifi
6 - газовый котел
*/
uint8_t menu_flag;
int up_down_count = 0;

extern uint8_t rx_flag;
extern uint8_t receive_counter;
int32_t millis = 0;
uint8_t WiFi_Flag = 1;

uint8_t HomePage_Flag;
uint8_t MainMenuPage_Flag;
uint8_t TimeSettingsPage_Flag;
uint8_t PageAbout_Flag;
uint8_t AddInfoPage_Flag;
uint8_t WiFiInfoPage_Flag;
uint8_t GasBoilerPage_Flag;

uint8_t timer0_flag = 0;
int timer0_counter = 0;
uint8_t clock_change_flag = 0;
int8_t clock_setting_count = 0;

uint8_t wifi_enable_flag = 0;
char WiFi_SSID[20];
char WiFi_PSWD[20];
char WiFi_IP[20];

/*
gas_boiler_enable_flag = 0 выкл авто
gas_boiler_enable_flag = 1 вкл авто
gas_boiler_enable_flag = 10 выкл ручн
gas_boiler_enable_flag = 11 вкл ручн
*/
uint8_t gas_boiler_enable_flag = 0;
uint8_t gas_boiler_setpoint_temp_integer = 20;
uint8_t gas_boiler_setpoint_temp_fraction = 0;
int gas_boiler_setpoint_temp_counter = 0;
uint8_t gas_boiler_setpoint_change_flag = 0;
int gas_boiler_rx_counter = 0;
int gas_boiler_rx_counter_old = 0;
uint8_t gas_boiler_rx_level = 0;
uint8_t setpoint_change_flag = 0;

extern uint8_t pipe;//номер канала

int32_t millis_update_weather = 0;

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
ISR (TIMER1_COMPA_vect)
{	
	/*if (millis == 300001)
	{
		millis = 0;
	}*/
	millis++;
	//if ((millis % 750) == 0)
	//{
		//sprintf_HOME_Weath_Param();
	//}
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
	_delay_us(3000);
	PORTL |= (1 << BUZZER);
	if (menu_flag == 0)
	{
		up_down_count = 0;
		menu_flag = 1;
	}
	else if (menu_flag == 1)
	{
		switch(up_down_count%6)
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
			case 4:
				up_down_count = 0;
				menu_flag = 5;
				break;
			case 5:
				up_down_count = 0;
				menu_flag = 6;
				break;
		}
	}
	else if (menu_flag == 2)
	{
		switch (up_down_count%8)
		{
			case 0:
				if (!clock_change_flag)
				{
					clock_setting_count = hour;
					clock_change_flag = 1;
					clock_change_mode = 1;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 1:
				if (!clock_change_flag)
				{
					clock_setting_count = min;
					clock_change_flag = 1;
					clock_change_mode = 2;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 2:
				if (!clock_change_flag)
				{
					clock_setting_count = sec;
					clock_change_flag = 1;
					clock_change_mode = 0;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 3:
				if (!clock_change_flag)
				{
					clock_setting_count = date;
					clock_change_flag = 1;
					clock_change_mode = 3;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 4:
				if (!clock_change_flag)
				{
					clock_setting_count = month;
					clock_change_flag = 1;
					clock_change_mode = 4;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 5:
				if (!clock_change_flag)
				{
					clock_setting_count = year;
					clock_change_flag = 1;
					clock_change_mode = 5;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 6:
				if (!clock_change_flag)
				{
					clock_setting_count = day;
					clock_change_flag = 1;
					clock_change_mode = 6;
				}
				else
				{
					ModifyRTC();
					clock_change_flag = 0;
				}
				break;
			case 7:
				up_down_count = 0;
				clock_setting_count = 0;
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
	else if (menu_flag == 5)
	{
		up_down_count = 0;
		menu_flag = 1;
	}
	else if (menu_flag == 6)
	{
		switch (up_down_count%4)
		{
			case 0:
				if ((gas_boiler_enable_flag / 10) == 0)
				{
					gas_boiler_enable_flag += 10;
				}
				else
				{
					gas_boiler_enable_flag -= 10;
				}
				break;
			case 1:
				//вход сюда только в ручном режиме
				if ((gas_boiler_enable_flag / 10) == 1)
				{
					if ((gas_boiler_enable_flag % 10) == 0)
					{
						gas_boiler_enable_flag += 1;
					}
					else
					{
						gas_boiler_enable_flag -= 1;
					}
				}
				break;
			case 2:
				if (!gas_boiler_setpoint_change_flag) 
				{
					gas_boiler_setpoint_change_flag = 1;
					gas_boiler_setpoint_temp_counter = gas_boiler_setpoint_temp_integer*10 + gas_boiler_setpoint_temp_fraction;
				}
				else
				{
					gas_boiler_setpoint_temp_integer = gas_boiler_setpoint_temp_counter / 10;
					gas_boiler_setpoint_temp_fraction = gas_boiler_setpoint_temp_counter % 10;
					gas_boiler_setpoint_change_flag = 0;
				}
				break;
			case 3:
				up_down_count = 0;
				menu_flag = 1;
				break;
		}
	}
}
ISR (INT5_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(3000);
	PORTL |= (1 << BUZZER);
	up_down_count = 0;
	clock_setting_count = 0;
	menu_flag = 0;
}
ISR (INT6_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(3000);
	PORTL |= (1 << BUZZER);
	if (clock_change_flag == 1)
	{
		clock_setting_count--;
	}
	else if(gas_boiler_setpoint_change_flag == 1)
	{
		gas_boiler_setpoint_temp_counter -= 5;
	}
	else
	{
		up_down_count--;
	}
	if (clock_change_flag == 1)
	{
		switch(clock_change_mode)
		{
			case 0:
			if (clock_setting_count <= -1)
			{
				clock_setting_count = 59;
			}
			sec = clock_setting_count;
			break;
			case 1:
			if (clock_setting_count <= -1)
			{
				clock_setting_count = 23;
			}
			hour = clock_setting_count;
			break;
			case 2:
			if (clock_setting_count <= -1)
			{
				clock_setting_count = 59;
			}
			min = clock_setting_count;
			break;
			case 3:
			if (clock_setting_count <= -1)
			{
				clock_setting_count = 31;
			}
			date = clock_setting_count;
			break;
			case 4:
			if (clock_setting_count <= 0)
			{
				clock_setting_count = 12;
			}
			month = clock_setting_count;
			break;
			case 5:
			if (clock_setting_count <= -1)
			{
				clock_setting_count = 99;
			}
			year = clock_setting_count;
			break;
			case 6:
			if (clock_setting_count <= 0)
			{
				clock_setting_count = 7;
			}
			day = clock_setting_count;
			break;
		}
	}
	else if(gas_boiler_setpoint_change_flag == 1)
	{
		if (gas_boiler_setpoint_temp_counter < 0)
		{
			gas_boiler_setpoint_temp_counter = 400;
		}
	}
	else
	{
		if (up_down_count < 0)
		{
			up_down_count = 167;
		}
	}
}
ISR (INT7_vect)
{
	PORTL &= ~ (1 << BUZZER);
	_delay_us(3000);
	PORTL |= (1 << BUZZER);
	if (clock_change_flag == 1)
	{
		clock_setting_count++;
	}
	else if(gas_boiler_setpoint_change_flag == 1)
	{
		gas_boiler_setpoint_temp_counter += 5;
	}
	else
	{
		up_down_count++;
	}
	if (clock_change_flag == 1)
	{
		switch(clock_change_mode)
		{
			case 0:
				if (clock_setting_count >= 60)
				{
					clock_setting_count = 0;
				}
				sec = clock_setting_count;
				break;
			case 1:
				if (clock_setting_count >= 24)
				{
					clock_setting_count = 0;
				}
				hour = clock_setting_count;
				break;
			case 2:
				if (clock_setting_count >= 60)
				{
					clock_setting_count = 0;
				}
				min = clock_setting_count;
				break;
			case 3:
				if (clock_setting_count >= 32)
				{
					clock_setting_count = 0;
				}
				date = clock_setting_count;
				break;
			case 4:
				if (clock_setting_count > 12)
				{
					clock_setting_count = 1;
				}
				month = clock_setting_count;
				break;
			case 5:
				if (clock_setting_count >= 100)
				{
					clock_setting_count = 0;
				}
				year = clock_setting_count;
				break;
			case 6:
				if (clock_setting_count > 7)
				{
					clock_setting_count = 1;
				}
				day = clock_setting_count;
				break;
		}
		ModifyRTC();
	}
	else if(gas_boiler_setpoint_change_flag == 1)
	{
		if (gas_boiler_setpoint_temp_counter > 400)
		{
			gas_boiler_setpoint_temp_counter = 0;
		}
	}
	else
	{
		if (up_down_count > 167)
		{
			up_down_count = 0;
		}
	}
}
//обработчик прерывания по UART
int uart_rx_count = 0;
char uart_rx_buffer[100];
char uart_message[100];
ISR(USART0_RX_vect)
{
	uart_rx_buffer[uart_rx_count] = UDR0;
	if (uart_rx_buffer[uart_rx_count] == '/')
	{
		memcpy(uart_message,uart_rx_buffer,strlen(uart_rx_buffer)-1);
		memset(uart_rx_buffer,'\0',strlen(uart_rx_buffer));
		uart_rx_count = 0;
		//Получение данные о wifi по uart
		if (strstr(uart_message,"WiFi-OK")!=0)
		{
			wifi_enable_flag = 1;
		}
		if (strstr(uart_message,"WiFi-ERROR")!=0)
		{
			wifi_enable_flag = 0;
		}
		if (strstr(uart_message,"SSID")!=0)
		{
			memset(WiFi_SSID,'\0',strlen(WiFi_SSID));
			memcpy(WiFi_SSID,uart_message+5,strlen(uart_message)-6);
			memset(uart_message,'\0',sizeof(char)*strlen(uart_message));
		}
		if (strstr(uart_message,"PSWD")!=0)
		{
			memset(WiFi_PSWD,'\0',strlen(WiFi_PSWD));
			memcpy(WiFi_PSWD,uart_message+5,strlen(uart_message)-5);
			memset(uart_message,'\0',sizeof(char)*strlen(uart_message));
		}
		if (strstr(uart_message,"IP")!=0)
		{
			memset(WiFi_IP,'\0',strlen(WiFi_IP));
			memcpy(WiFi_IP,uart_message+3,strlen(uart_message)-3);
			memset(uart_message,'\0',sizeof(char)*strlen(uart_message));
		}
	}
	else
	{
		uart_rx_count++;
	}
}
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
	DDRD &= ~(1<<IRQ);
	PORTD |= (1<<IRQ);
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
	USART_Init(16);    //Инициализация модуля USART скорость 115200	
	ILI9486_ini();
	//Вывод приветствия
	Print_Hello_World();
	wdt_reset();
	//Инициализация оборудования
	NRF24_ini();
	RTC_init();
	dht22_init();
	BMP180_Calibration();
	wdt_reset();
	sprintf_HOME_Weath_Param();
	_delay_ms(1500);
	// Установка времени для DS3231(делается 1 разv)
	//RTC_write_time(22, 17, 0);
	//RTC_write_date(1, 13, 3, 23);
	//Вывод окна загрузки
	wdt_reset();
	Print_Download();
	wdt_reset();
	PORTL &= ~(1<<LED);
	//Инициализация таймеров и прерываний
	//timer2_ini();
	timer1_ini();
	timer0_ini();
	// настраиваем параметры прерывания
	EICRB |= (1<<ISC41) | (1<<ISC51) | (1<<ISC61) | (1<<ISC71);
	EICRA |= (0<<ISC20) | (1<<ISC21);
	EIMSK |= (1<<INT4) | (1<<INT5) | (1<<INT6) | (1<<INT7) | (1<<INT2);
	//enc_interrupt_ini();
	//Начальная конфигурация
	Weath_Param_ini();
	//Фиксация времени начала работы
	Clock ();
	sprintf(start_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);	
	sprintf_HOME_Weath_Param();
	Print_Static_Home_Page();
	Print_Home_Page_In();
	Print_Home_Page_WeatherForecast();
	Print_Home_Page_Out();
	HomePage_Flag = 0;
	MainMenuPage_Flag = 1;
	sei();
	//отправка запроса к wifi модулю на получение инфо о wifi
	USART_Transmit("GETWIFI");
	_delay_ms(500);
    while (1) 
    {
		//прием данных от передатчика метеостанции
		if ((rx_flag == 1)&&(pipe == 0))
		{
			rx_count = 0;
			time1 = millis;
			while(rx_count < 6)
			{
				time2 = millis;
				if ((time2-time1)>15000)
				{
					rx_flag = 0;
					receive_counter = 0;
					rx_count = 0;
					break;
				}
				if((rx_flag == 1)&&(pipe == 0))
				{
					PORTL |= (1<<LED);
					_delay_us(300);
					PORTL &= ~(1<<LED);
					NRF24L01_Receive();
					rx_count++;
				}
				//обновление показаний на экране
				if (menu_flag == 0)
				{
					DrawLevelNrfWeather();
				}
				wdt_reset();
				
			}
			Clock ();
			sprintf(receive_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);
			if (menu_flag == 0)
			{
				Print_Home_Page_WeatherForecast();
				Print_Home_Page_Out();
			}
			//_delay_ms(1000);
		}
		//прием данных от газового котла
		else if ((rx_flag == 1)&&(pipe == 1))
		{
			Clock ();
			memset(rx_time_gas_boiler, '\0', sizeof(char) * strlen(rx_time_gas_boiler));//очистка массива
			sprintf(rx_time_gas_boiler,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);
			PORTL |= (1<<LED);
			_delay_us(300);
			PORTL &= ~(1<<LED);
			NRF24L01_Receive();
			uint8_t buf1[10] = {0};//буффер для отправки
			uint8_t dt;
			buf1[0] = gas_boiler_enable_flag;
			if (setpoint_change_flag == 1)
			{
				buf1[0] += 100;
				setpoint_change_flag = 0;
			}
			buf1[1] = gas_boiler_setpoint_temp_integer;
			buf1[2] = gas_boiler_setpoint_temp_fraction;
			buf1[3] = home_temp_integer;
			buf1[4] = home_temp_fraction;
			dt = NRF24L01_Send(buf1);
			gas_boiler_rx_counter++;
			if (gas_boiler_rx_counter > 65000)
			{
				gas_boiler_rx_counter = 0;
				gas_boiler_rx_counter_old = 0;
			}
		}
		else
		{
			//отправка данных в БД
			if(millis >= 300000)
 			{
				Clock ();
				memset(send_time, '\0', sizeof(char) * strlen(send_time));//очистка массива
				sprintf(send_time,"%s:%s:%s,%s/%s/%s", T_Param.hours, T_Param.minutes, T_Param.seconds, T_Param.mounthday, T_Param.Mounth, T_Param.Year);
				sprintf_HOME_Weath_Param();
				sprintf(DATA_TO_UART,"%s %s %s %s %s %s %s %s %s %s ", temp_street_to_DB, temp_home_to_DB, hum_street_to_DB, hum_home_to_DB, Press_home_to_DB, Rain_to_DB, Vbat_to_DB, WIND_speed_to_DB, wind_direction_to_DB, send_time);
				USART_Transmit(DATA_TO_UART);
				memset(DATA_TO_UART, '\0', sizeof(char) * strlen(DATA_TO_UART));//очистка массива
				millis = 0;
				millis_update_weather = 0;
			}
			//обновление домашних показаний и обновление уровня приема от газового котла
			else if (abs(millis - millis_update_weather) > 14000)
			{
				sprintf_HOME_Weath_Param();
				//вывод уровня сигнала NRF от газового котла
				FindLevelNrfGasBoiler();
				gas_boiler_rx_counter_old = gas_boiler_rx_counter;
				millis_update_weather = millis;
			}
			//обновление изображения на дисплее
			else if((millis % 100) == 0)
			{	
				switch (menu_flag)
				{
					case 0:	
						if (HomePage_Flag == 1)
						{
							//sprintf_HOME_Weath_Param();
							Print_Static_Home_Page();
							Print_Home_Page_In();
							Print_Home_Page_WeatherForecast();
							Print_Home_Page_Out();
							HomePage_Flag = 0;
							MainMenuPage_Flag = 1;
							TimeSettingsPage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 1;
							WiFiInfoPage_Flag = 1;
							GasBoilerPage_Flag = 1;
						}
						else
						{
							//sprintf_HOME_Weath_Param();
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
							WiFiInfoPage_Flag = 1;
							GasBoilerPage_Flag = 1;
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
							WiFiInfoPage_Flag = 1;
							GasBoilerPage_Flag = 1;
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
							WiFiInfoPage_Flag = 1;
							GasBoilerPage_Flag = 1;
						}
						
						break;
					case 4:	
						if(AddInfoPage_Flag == 1)
						{
							Print_WeatherStation_Info_Static();
							Print_WeatherStation_Info();
							TimeSettingsPage_Flag = 1;
							MainMenuPage_Flag = 1;
							HomePage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 0;
							WiFiInfoPage_Flag = 1;
							GasBoilerPage_Flag = 1;
						}
						else
						{
							Print_WeatherStation_Info();
						}
						break;
					case 5:
						if(WiFiInfoPage_Flag == 1)
						{
							Print_WIFI_Page();
							TimeSettingsPage_Flag = 1;
							MainMenuPage_Flag = 1;
							HomePage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 1;
							WiFiInfoPage_Flag = 0;
							GasBoilerPage_Flag = 1;
						}		
						break;
					case 6:
						if(GasBoilerPage_Flag == 1)
						{
							Print_Gas_Boiler_Page_Static();
							Print_Gas_Boiler_Page();
							TimeSettingsPage_Flag = 1;
							MainMenuPage_Flag = 1;
							HomePage_Flag = 1;
							PageAbout_Flag = 1;
							AddInfoPage_Flag = 1;
							WiFiInfoPage_Flag = 1;
							GasBoilerPage_Flag = 0;
						}
						else
						{
							Print_Gas_Boiler_Page();
						}
						break;
				}
			}
		}
		wdt_reset();
    }
}

