/*
 * WeatherStation.c
 *
 * Created: 03.06.2022 19:15:57
 *  Author: Alex2
 */ 
#include "WeatherStation.h"
#include "Images.h"

#define RADIUS 0.139

char wind_direction[6];

char temp_street_to_DB[6] = {0};
char hum_street_to_DB[5] = {0};
char WIND_speed_to_DB[5] = {0};
char wind_direction_to_DB[6] = {0};
char Vbat_to_DB[5] = {0};
char Rain_to_DB[6] = {0};
char temp_home_to_DB[6] = {0};
char hum_home_to_DB[5] = {0};
char Press_home_to_DB[5] = {0};

struct Time_Parameters {
	char hours[4];
	char minutes[4];
	char seconds[4];
	char weakday[10];
	char mounthday[4];
	char Mounth[4];
	char Year[4];
} T_Param;

extern unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;
extern char receive_time[20];
extern char send_time[20];
extern char start_time[20];
extern char rx_time_gas_boiler[20];
extern char adc_value1[6];
extern char HALL_counter[5];
extern char adc_value2[6];

extern uint8_t rx_count;
extern uint8_t clock_change_flag;
extern int up_down_count;
extern int8_t add_cnt;
extern uint8_t receive_counter;
extern uint8_t WiFi_Flag;
extern int8_t clock_setting_count;


uint8_t home_temp_sign;//знак домаш темп
uint8_t home_temp_integer;//целая часть домаш темп
uint8_t home_temp_fraction;//дробная часть домаш темп
int home_hum_integer;//целая часть домаш влажн
int pressure_home;//целая часть давления

extern uint8_t street_temp_sign;//знак уличн темп
extern uint8_t street_temp_integer;//целая часть уличн темп
extern uint8_t street_temp_fraction;//дробная часть уличн темп
extern int street_hum_integer;//целая часть уличн влажн
extern uint8_t wind_speed_integer;//целая часть ск ветра
extern uint8_t wind_speed_fraction;//дробная часть ск ветра
extern uint8_t rain; //целая часть осадков
extern uint8_t V_Bat_integer;//целая часть заряда акб
extern uint8_t V_Bat_fraction;//дробная часть заряда акб

extern uint8_t wifi_enable_flag;
extern char WiFi_SSID[20];
extern char WiFi_PSWD[20];
extern char WiFi_IP[20];
extern uint8_t rx_flag;

extern uint8_t gas_boiler_enable_flag;
extern uint8_t gas_boiler_setpoint_temp_integer;
extern uint8_t gas_boiler_setpoint_temp_fraction;
extern uint8_t gas_boiler_setpoint_change_flag;
extern int gas_boiler_setpoint_temp_counter;
extern uint8_t gas_boiler_rx_level;

extern uint8_t pipe;//номер канала

//Окно приветсвия на экране дисплея
void Print_Hello_World()
{	
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(20,80);
	ILI9486_Print_String32x32("Вас приветствует",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(140,130);
	ILI9486_Print_String32x32("система",TFT_STRING_MODE_NO_BACKGROUND);
	ILI9486_SetCursor(110,180);
	ILI9486_Print_String32x32("Умный Дом!",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	//ILI9486_SetCursor(100,220);
	//ILI9486_Print_String32x24("is with you now!",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
}
//Окно загрузки
void Print_Download()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(120,50);
	ILI9486_Print_String32x32("Пожалуйста",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(120,90);
	ILI9486_Print_String32x32("подождите,",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(20,130);
	ILI9486_Print_String32x32("идёт загрузка...",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_DrawRect(50, 200,  383, 50,  WHITE);
	for(uint16_t i = 54; i < 429 ; i++)
	{
		ILI9486_Draw_Line(i, 204, i, 245, WHITE);
		_delay_ms(20);
		wdt_reset();
	}
}
//Статическое главное окно
void Print_Static_Home_Page()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	//надписи для уровней приема
	ILI9486_SetTextColor(0xB7F9,BLACK);
	ILI9486_SetCursor(160,40);
	ILI9486_Print_StringConsolas16x24("WiFi",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(223,40);
	ILI9486_Print_StringConsolas16x24("Ch2",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(275,40);
	ILI9486_Print_StringConsolas16x24("Ch1",TFT_STRING_MODE_BACKGROUND);
	//вывод границ
	Print_Boards_Main();
	//вывод статики флюгера
	DrawWeatherVane();
	//вывод всех иконок
	ILI9486_Draw_Image("/wsp.txt", 30, 30, 170, 255);//ск ветра
	ILI9486_Draw_Image("/pres.txt", 30, 30, 172, 285);//давление
	ILI9486_Draw_Image("/rain.txt", 30, 30, 330, 90);//осадки
	//вывод второго уровня сигнала NRF (в перспективе для котла)-тоже пока сюда
	//ILI9486_Draw_Image("/nrf0.txt", 30, 30, 229, 10);
	
	//вывод надписи для скорости ветра
	/*ILI9486_SetTextColor(BLUE,BLACK);
	ILI9486_SetCursor(173,241);
	ILI9486_Print_String14x24("W",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(186,250);
	ILI9486_Print_String14x24("I",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(198,255);
	ILI9486_Print_String14x24("N",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(215,257);
	ILI9486_Print_String14x24("D",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(239,257);
	ILI9486_Print_String14x24("S",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(253,257);
	ILI9486_Print_String14x24("P",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(267,255);
	ILI9486_Print_String14x24("E",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(281,250);
	ILI9486_Print_String14x24("E",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(295,240);
	ILI9486_Print_String14x24("D",TFT_STRING_MODE_BACKGROUND);*/
}
//Главное окно (уличные показания)
void Print_Home_Page_Out()
{
	char data[10];
	//вывод скорости ветра
	ILI9486_SetTextColor(LIGHTBLUE,BLACK);
	if(wind_speed_integer < 10)
	{
		sprintf(data," %d",wind_speed_integer);
	}
	else
	{
		sprintf(data,"%d",wind_speed_integer);
	}
	ILI9486_SetCursor(204,260);
	ILI9486_Print_StringConsolas16x24(data,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(231,260);
	ILI9486_Print_StringConsolas16x24(".",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(244,260);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	sprintf(data,"%d",wind_speed_fraction);
	ILI9486_Print_StringConsolas16x24(data,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(270,260);
	ILI9486_Print_StringConsolas16x24("m/",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(294,260);
	ILI9486_Print_StringConsolas16x24("s",TFT_STRING_MODE_BACKGROUND);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	//вывод уличных показаний
	ILI9486_SetTextColor(YELLOW,BLACK);
	if (street_temp_sign != 0x00)
	{
		ILI9486_SetCursor(397,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//точка
		if(street_temp_integer < 10)
		{
			sprintf(data," %d",street_temp_integer);
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
			ILI9486_SetCursor(346,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//знак -
		}
		else
		{
			sprintf(data,"%d",street_temp_integer);
			ILI9486_SetCursor(317,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//знак -
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		}
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
		sprintf(data,"%d",street_temp_fraction);
		ILI9486_SetCursor(425,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//дробная часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	else
	{
		ILI9486_SetCursor(397,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//точка
		if(street_temp_integer < 10)
		{
			sprintf(data," %d",street_temp_integer);
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		}
		else
		{
			sprintf(data,"%d",street_temp_integer);
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		}
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
		sprintf(data,"%d",street_temp_fraction);
		ILI9486_SetCursor(425,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	ILI9486_SetCursor(457,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_BACKGROUND);//ед измерения
	//влажность
	ILI9486_SetCursor(361,150);
	if(street_hum_integer < 10)
	{
		sprintf(data," %d",street_hum_integer);
	}
	else
	{
		sprintf(data,"%d",street_hum_integer);
	}
	ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	ILI9486_SetCursor(429,165);
	ILI9486_Print_String18x32("%",TFT_STRING_MODE_BACKGROUND);//ед измерения
	//вывод флюгера
	DrawWindDirect();
	//вывод кол-ва осадков
	ILI9486_SetTextColor(0xA39F,BLACK);
	ILI9486_SetCursor(363,95);
	if(rain < 10)
	{
		sprintf(data," %d",rain);
	}
	else
	{
		sprintf(data,"%d",rain);
	}
	ILI9486_Print_StringConsolas16x24(data,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(395,95);
	ILI9486_Print_StringConsolas16x24("%",TFT_STRING_MODE_BACKGROUND);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	//вывод уровня сигнала NRF от метеостанции
	DrawLevelNrfWeather();
}
//Прогноз погоды на главном окне
void Print_Home_Page_WeatherForecast()
{
	//вывод прогноза погоды
	if ((wind_speed (HALL_counter) == 0) && (RAIN_AMOUNT(adc_value2) <= 10))
	{
		ILI9486_Draw_Image("/sun.txt", 100, 100, 370, 3);
		//DrawSun();
	}
	else if ((wind_speed (HALL_counter) > 1000))
	{
		ILI9486_Draw_Image("/clou.txt", 100, 100, 370, 3);
		//DrawClouds();
	}
	else if ((wind_speed (HALL_counter) > 0) && (RAIN_AMOUNT(adc_value2) <= 10))
	{
		ILI9486_Draw_Image("/sucl.txt", 100, 100, 370, 3);
		//DrawSunWithClouds();
	}
	else if ((RAIN_AMOUNT(adc_value2) > 15) && (RAIN_AMOUNT(adc_value2) <= 40))
	{
		ILI9486_Draw_Image("/smra.txt", 100, 100, 370, 3);
		//DrawSmalRain();
	}
	else if ((RAIN_AMOUNT(adc_value2) > 40) && (RAIN_AMOUNT(adc_value2) <= 100))
	{
		ILI9486_Draw_Image("/hara.txt", 100, 100, 370, 3);
		//DrawHardRain();
	}
	else
	{
		ILI9486_Draw_Image("/sun.txt", 100, 100, 370, 3);
		//DrawSun();
	}
}
//Главное окно(домашние показания)
uint8_t Print_Home_Page_In()
{
	char data[10];
	char TIME[10] = {0};
	char DATE[10] = {0};
	//вывод времени/даты
	Clock();
	sprintf(TIME,"%s:%s",T_Param.hours, T_Param.minutes);
	sprintf(DATE,"%s/%s/%s",T_Param.mounthday, T_Param.Mounth, T_Param.Year);
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(10,15);
	ILI9486_Print_String24x32(TIME,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(12,55);
	ILI9486_Print_String14x24(T_Param.weakday,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(9,80);
	ILI9486_Print_String18x32(DATE,TFT_STRING_MODE_BACKGROUND);
	wdt_reset();
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	//вывод уровня сигнала WiFi
	DrawLevelWiFi();
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	//вывод уровня сигнала NRF от газового котла
	DrawLevelNrfGasBoiler();
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	//вывод домашних показаний
	//влажность
	ILI9486_SetTextColor(0xFB80,BLACK);
	ILI9486_SetCursor(47,150);
	if (home_hum_integer < 10)
	{
		sprintf(data," %d",home_hum_integer);
	}
	else
	{
		sprintf(data,"%d",home_hum_integer);
	}
	ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(115,165);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	ILI9486_Print_String18x32("%",TFT_STRING_MODE_BACKGROUND);//ед измерения
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	//температура
	if (home_temp_sign != 0x00)
	{
		ILI9486_SetCursor(81,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//точка
		if(home_temp_integer < 10)
		{
			sprintf(data," %d",home_temp_integer);
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
			ILI9486_SetCursor(3,255);
			ILI9486_Print_String18x32(" ",TFT_STRING_MODE_BACKGROUND);
			ILI9486_SetCursor(30,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//знак -
		}
		else
		{
			sprintf(data,"%d",home_temp_integer);
			ILI9486_SetCursor(3,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//знак -
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		}
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(111,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//дробная часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	else
	{
		ILI9486_SetCursor(81,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//точка
		if(home_temp_integer < 10)
		{
			sprintf(data," %d",home_temp_integer);
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		}
		else
		{
			sprintf(data,"%d",home_temp_integer);
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		}
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(111,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	ILI9486_SetCursor(143,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_BACKGROUND);//ед измерения
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	//вывод атм. давления
	ILI9486_SetTextColor(0xF96B,BLACK);
	ILI9486_SetCursor(206,290);
	sprintf(data,"%d",pressure_home);
	ILI9486_Print_StringConsolas16x24(data,TFT_STRING_MODE_BACKGROUND);
	if ((rx_flag == 1)&&(pipe == 0))
	{
		return 0;
	}
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	ILI9486_SetCursor(248,290);
	ILI9486_Print_StringConsolas16x24("mmHg",TFT_STRING_MODE_BACKGROUND);
	return 0;
}
//Основная разметка для главного окна
void Print_Boards_Main()
{
	//отрисовка границ экрана
	ILI9486_Draw_Line(15, 1, 464, 1, WHITE);
	int16_t f = 1 - 14;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * 14;
	int16_t x = 0;
	int16_t y = 14;
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(15 - x, 15 - y, WHITE);
		ILI9486_drawPixel(15 - y, 15 - x, WHITE);
	}
	ILI9486_Draw_Line(478, 15, 478, 304, WHITE);
	f = 1 - 14;
	ddF_x = 1;
	ddF_y = -2 * 14;
	x = 0;
	y = 14;
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(464 + x, 15 - y, WHITE);
		ILI9486_drawPixel(464 + y, 15 - x, WHITE);
	}
	ILI9486_Draw_Line(1, 15, 1, 304, WHITE);
	f = 1 - 14;
	ddF_x = 1;
	ddF_y = -2 * 14;
	x = 0;
	y = 14;
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(15 - x, 304 + y, WHITE);
		ILI9486_drawPixel(15 - y, 304 + x, WHITE);
	}
	ILI9486_Draw_Line(15, 318, 464, 318, WHITE);
	f = 1 - 14;
	ddF_x = 1;
	ddF_y = -2 * 14;
	x = 0;
	y = 14;
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(464 + x, 304 + y, WHITE);
		ILI9486_drawPixel(464 + y, 304 + x, WHITE);
	}
	
	//отрисовка нижних линий где indoor и outdoor
	//правая половина
	ILI9486_Draw_Line(164, 318, 164, 220, WHITE);
	ILI9486_Draw_Line(168, 318, 168, 225, WHITE);
	ILI9486_Draw_Line(315, 220, 341, 220, WHITE);
	ILI9486_Draw_Line(314, 318, 314, 220, WHITE);
	ILI9486_Draw_Line(310, 318, 310, 225, WHITE);
	ILI9486_Draw_Line(453, 220, 478, 220, WHITE);
	
	ILI9486_Draw_Line(352, 230, 443, 230, WHITE);
	ILI9486_Draw_Line(352, 206, 443, 206, WHITE);
	f = 1 - 12;
	ddF_x = 1;
	ddF_y = -2 * 12;
	x = 0;
	y = 12;
	ILI9486_drawPixel(354-12, 218, WHITE);
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(354 - x, 218 + y, WHITE);
		ILI9486_drawPixel(354 - x, 218 - y, WHITE);
		ILI9486_drawPixel(354 - y, 218 + x, WHITE);
		ILI9486_drawPixel(354 - y, 218 - x, WHITE);
	}
	f = 1 - 12;
	ddF_x = 1;
	ddF_y = -2 * 12;
	x = 0;
	y = 12;

	ILI9486_drawPixel(441+12, 218  , WHITE);

	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(441 + x, 218 + y, WHITE);
		ILI9486_drawPixel(441 + x, 218 - y, WHITE);
		ILI9486_drawPixel(441 + y, 218 + x, WHITE);
		ILI9486_drawPixel(441 + y, 218 - x, WHITE);
	}
	/*ILI9486_SetTextColor(YELLOW, BLACK);
	ILI9486_SetCursor(349, 208);
	ILI9486_Print_String14x24("OUTDOOR",TFT_STRING_MODE_NO_BACKGROUND);
	ILI9486_SetTextColor(ORANGE, BLACK);
	ILI9486_SetCursor(40, 208);
	ILI9486_Print_String14x24("INDOOR",TFT_STRING_MODE_NO_BACKGROUND);*/
	//левая половина
	ILI9486_Draw_Line(36, 230, 127, 230, WHITE);
	ILI9486_Draw_Line(36, 206, 127, 206, WHITE);
	f = 1 - 12;
	ddF_x = 1;
	ddF_y = -2 * 12;
	x = 0;
	y = 12;
	ILI9486_drawPixel(38-12, 218, WHITE);
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(38 - x, 218 + y, WHITE);
		ILI9486_drawPixel(38 - x, 218 - y, WHITE);
		ILI9486_drawPixel(38 - y, 218 + x, WHITE);
		ILI9486_drawPixel(38 - y, 218 - x, WHITE);
	}
	f = 1 - 12;
	ddF_x = 1;
	ddF_y = -2 * 12;
	x = 0;
	y = 12;
	ILI9486_drawPixel(125+12, 218  , WHITE);
	while (x<y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		ILI9486_drawPixel(125 + x, 218 + y, WHITE);
		ILI9486_drawPixel(125 + x, 218 - y, WHITE);
		ILI9486_drawPixel(125 + y, 218 + x, WHITE);
		ILI9486_drawPixel(125 + y, 218 - x, WHITE);
	}
	ILI9486_Draw_Line(1, 220, 25, 220, WHITE);
	ILI9486_Draw_Line(137, 220, 163, 220, WHITE);
	
	ILI9486_Draw_Line(137, 216, 341, 216, WHITE);
	ILI9486_Draw_Line(1, 216, 26, 216, WHITE);
	ILI9486_Draw_Line(453, 216, 478, 216, WHITE);
	
	
	//отрисовка верхней части
	ILI9486_Draw_Line(1, 120, 478, 120, WHITE);
	ILI9486_Draw_Line(1, 124, 149, 124, WHITE);
	ILI9486_Draw_Line(330, 124, 478, 124, WHITE);
	
	ILI9486_Draw_Line(145, 1, 161, 100, WHITE);
	ILI9486_Draw_Line(332, 1, 316, 100, WHITE);
	ILI9486_Draw_Line(149, 1, 165, 96, WHITE);
	ILI9486_Draw_Line(328, 1, 312, 96, WHITE);
	
	//отрисовка части круга для ветра
	ILI9486_Draw_Circle(239, 159, 97,WHITE);
	ILI9486_Draw_Line(152, 120, 327, 120, BLACK);
	ILI9486_Draw_Line(161, 216, 317, 216, BLACK);
	
	//очистка лишних пикселей
	ILI9486_Draw_Line(1, 219, 1, 217, BLACK);
	ILI9486_Draw_Line(478, 219, 478, 217, BLACK);
	ILI9486_Draw_Line(1, 121, 1, 123, BLACK);
	ILI9486_Draw_Line(478, 121, 478, 123, BLACK);
	
	ILI9486_Draw_Line(146, 1, 148, 1, BLACK);
	ILI9486_Draw_Line(329, 1, 331, 1, BLACK);
	ILI9486_Draw_Line(165, 318, 167, 318, BLACK);
	ILI9486_Draw_Line(311, 318, 313, 318, BLACK);
	
	for(uint16_t i = 165; i <= 167; i++)
	{
		ILI9486_Draw_Line(i, 220, i, 230, BLACK);
	}
	
	for(uint16_t i = 311; i <= 313; i++)
	{
		ILI9486_Draw_Line(i, 220, i, 230, BLACK);
	}
	
	for(uint16_t i = 217; i <= 219; i++)
	{
		ILI9486_Draw_Line(160, i, 170, i, BLACK);
	}
	
	for(uint16_t i = 217; i <= 219; i++)
	{
		ILI9486_Draw_Line(310, i, 320, i, BLACK);
	}
	
	for(uint16_t i = 121; i <= 123; i++)
	{
		ILI9486_Draw_Line(140, i, 160, i, BLACK);
	}
	
	for(uint16_t i = 121; i <= 123; i++)
	{
		ILI9486_Draw_Line(320, i, 340, i, BLACK);
	}
	
	for(uint16_t i = 162; i <= 164; i++)
	{
		ILI9486_Draw_Line(i, 96, i, 100, BLACK);
	}
	
	for(uint16_t i = 313; i <= 315; i++)
	{
		ILI9486_Draw_Line(i, 96, i, 100, BLACK);
	}
	
	ILI9486_SetTextColor(YELLOW, BLACK);
	ILI9486_SetCursor(349, 208);
	ILI9486_Print_String14x24("OUTDOOR",TFT_STRING_MODE_NO_BACKGROUND);
	ILI9486_SetTextColor(0xFB80, BLACK);
	ILI9486_SetCursor(40, 208);
	ILI9486_Print_String14x24("INDOOR",TFT_STRING_MODE_NO_BACKGROUND);
}
//Окно меню статич
void Print_Menu_Page_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(40,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("ГЛАВНОЕ МЕНЮ",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(70,70);
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_Print_String32x32("Главное окно",TFT_STRING_MODE_BACKGROUND);							   
	ILI9486_SetCursor(70,110);
	ILI9486_Print_String32x32("Дата и время",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(70,150);
	ILI9486_Print_String32x32(" О проекте",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(70,190);
	ILI9486_Print_String32x32("Метеостанция",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(70,230);
	ILI9486_Print_String32x32(" Wi-Fi сеть",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(67,270);
	ILI9486_Print_String32x32("Газовый котёл",TFT_STRING_MODE_BACKGROUND);
}
//Окно меню динамич
void Print_Menu_Page()
{
	switch (up_down_count%6)
	{
		case 0: 
				ILI9486_fillTriangle(22,72,37,88,22,104,WHITE);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				ILI9486_fillTriangle(22,232,37,248,22,264,BLACK);
				ILI9486_fillTriangle(22,272,37,288,22,304,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,WHITE);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				ILI9486_fillTriangle(456,232,441,248,456,264,BLACK);
				ILI9486_fillTriangle(456,272,441,288,456,304,BLACK);
				break;
		case 1: 
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,WHITE);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				ILI9486_fillTriangle(22,232,37,248,22,264,BLACK);
				ILI9486_fillTriangle(22,272,37,288,22,304,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,WHITE);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				ILI9486_fillTriangle(456,232,441,248,456,264,BLACK);
				ILI9486_fillTriangle(456,272,441,288,456,304,BLACK);
				break;
		case 2: 
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,WHITE);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				ILI9486_fillTriangle(22,232,37,248,22,264,BLACK);
				ILI9486_fillTriangle(22,272,37,288,22,304,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,WHITE);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				ILI9486_fillTriangle(456,232,441,248,456,264,BLACK);
				ILI9486_fillTriangle(456,272,441,288,456,304,BLACK);
				break;
		case 3: 
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,WHITE);
				ILI9486_fillTriangle(22,232,37,248,22,264,BLACK);
				ILI9486_fillTriangle(22,272,37,288,22,304,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,WHITE);
				ILI9486_fillTriangle(456,232,441,248,456,264,BLACK);
				ILI9486_fillTriangle(456,272,441,288,456,304,BLACK);
				break;
		case 4:
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				ILI9486_fillTriangle(22,232,37,248,22,264,WHITE);
				ILI9486_fillTriangle(22,272,37,288,22,304,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				ILI9486_fillTriangle(456,232,441,248,456,264,WHITE);
				ILI9486_fillTriangle(456,272,441,288,456,304,BLACK);
				break;
		case 5:
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				ILI9486_fillTriangle(22,232,37,248,22,264,BLACK);
				ILI9486_fillTriangle(22,272,37,288,22,304,WHITE);
		
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				ILI9486_fillTriangle(456,232,441,248,456,264,BLACK);
				ILI9486_fillTriangle(456,272,441,288,456,304,WHITE);
				break;
	}
}
//Окно настроек времени
void Print_Page_Clock_Settings_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(0,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String32x32("   ДАТА и ВРЕМЯ  ",TFT_STRING_MODE_BACKGROUND);
	Clock();
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(112,120);
	ILI9486_Print_String32x32("ч",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(232,120);
	ILI9486_Print_String32x32("мин",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(377,120);
	ILI9486_Print_String32x32("сек",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(50,100);
	ILI9486_Print_String32x48(T_Param.hours,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(170,100);
	ILI9486_Print_String32x48(T_Param.minutes,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(315,100);
	ILI9486_Print_String32x48(T_Param.seconds,TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetCursor(100,170);
	ILI9486_Print_String32x48(T_Param.mounthday,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(165,170);
	ILI9486_Print_String32x48("/",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(205,170);
	ILI9486_Print_String32x48(T_Param.Mounth,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(275,170);
	ILI9486_Print_String32x48("/",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(315,170);
	ILI9486_Print_String32x48(T_Param.Year,TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetCursor(190,230);
	ILI9486_Print_String18x32(T_Param.weakday,TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("НАЗАД В МЕНЮ",TFT_STRING_MODE_BACKGROUND);
}
void Print_Page_Clock_Settings()
{
	Convert_to_string_Clock();
	switch (up_down_count%8)
	{
		case 0:	
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(50,100);
				ILI9486_Print_String32x48(T_Param.hours,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,WHITE);
			ILI9486_DrawRect(48,98,65,52,WHITE);
		
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
		
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
		
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
		
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
		
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
		
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 1: 
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(170,100);
				ILI9486_Print_String32x48(T_Param.minutes,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
		
			ILI9486_DrawRect(167,97,67,54,WHITE);
			ILI9486_DrawRect(168,98,65,52,WHITE);
		
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
		
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
		
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
		
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
		
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 2: 
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(315,100);
				ILI9486_Print_String32x48(T_Param.seconds,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
		
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
		
			ILI9486_DrawRect(312,97,67,54,WHITE);
			ILI9486_DrawRect(313,98,65,52,WHITE);
		
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
		
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
		
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
		
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 3: 
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(100,170);
				ILI9486_Print_String32x48(T_Param.mounthday,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
		
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
		
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
		
			ILI9486_DrawRect(97,167,67,54,WHITE);
			ILI9486_DrawRect(98,168,65,52,WHITE);
		
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
		
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
		
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 4: 
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(205,170);
				ILI9486_Print_String32x48(T_Param.Mounth,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
		
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
		
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
		
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
		
			ILI9486_DrawRect(202,167,67,54,WHITE);
			ILI9486_DrawRect(203,168,65,52,WHITE);
		
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
		
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 5: 
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(315,170);
				ILI9486_Print_String32x48(T_Param.Year,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
		
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
		
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
		
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
		
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
		
			ILI9486_DrawRect(312,167,67,54,WHITE);
			ILI9486_DrawRect(313,168,65,52,WHITE);
		
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 6:
			if (clock_change_flag == 1)
			{
				ILI9486_SetCursor(190,230);
				ILI9486_Print_String18x32(T_Param.weakday,TFT_STRING_MODE_BACKGROUND);
			}
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
		
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
		
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
		
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
		
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
		
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
		
			ILI9486_DrawRect(75,227,352,37,WHITE);
			ILI9486_DrawRect(76,228,350,35,WHITE);
		
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 7:
			ILI9486_DrawRect(47,97,67,54,BLACK);
			ILI9486_DrawRect(48,98,65,52,BLACK);
			
			ILI9486_DrawRect(167,97,67,54,BLACK);
			ILI9486_DrawRect(168,98,65,52,BLACK);
			
			ILI9486_DrawRect(312,97,67,54,BLACK);
			ILI9486_DrawRect(313,98,65,52,BLACK);
			
			ILI9486_DrawRect(97,167,67,54,BLACK);
			ILI9486_DrawRect(98,168,65,52,BLACK);
			
			ILI9486_DrawRect(202,167,67,54,BLACK);
			ILI9486_DrawRect(203,168,65,52,BLACK);
			
			ILI9486_DrawRect(312,167,67,54,BLACK);
			ILI9486_DrawRect(313,168,65,52,BLACK);
			
			ILI9486_DrawRect(75,227,352,37,BLACK);
			ILI9486_DrawRect(76,228,350,35,BLACK);
			
			ILI9486_DrawRect(37,273,414,42,WHITE);
			ILI9486_DrawRect(38,274,412,40,WHITE);
			break;
	}
}
//Окно справки
void Print_Page_About()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(0,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String32x32("    О ПРОЕКТЕ    ",TFT_STRING_MODE_BACKGROUND);
	
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("НАЗАД В МЕНЮ",TFT_STRING_MODE_BACKGROUND);
	ILI9486_DrawRect(37,273,414,42,WHITE);
	ILI9486_DrawRect(38,274,412,40,WHITE);
	/*uint8_t Frame_buffer[1024] = { 0 }; //Буфер кадра
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	
	LCD_12864_Decode_UTF8(25, 0, 1, "Об устройстве", Frame_buffer);
	LCD_12864_Decode_UTF8(2, 1, 0, "\"Домашняя метеостан-", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 2, 0, "ция (v.1.0)\"-предна-", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 3, 0, "значена для сбора и ", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 4, 0, "обработки информации ", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 5, 0, "о метео условиях за ", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 6, 0, "окном и в помещении.", Frame_buffer);
	LCD_12864_Decode_UTF8(25, 7, 1, "Назад в МЕНЮ", Frame_buffer);
	
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);*/
}
//Окно инфо о метеостанции
void Print_WeatherStation_Info_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(0,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String32x32("   МЕТЕОСТАНЦИЯ  ",TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(20,60);
	ILI9486_Print_String32x32("Последний прием:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(10,100);
	ILI9486_Print_String32x32("(Ch1)",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(20,140);
	ILI9486_Print_String32x32("Отправка в БД:",TFT_STRING_MODE_BACKGROUND);
	/*ILI9486_SetCursor(150,70);
	ILI9486_Print_String18x32("Start time:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(90,100);
	ILI9486_Print_String18x32(start_time,TFT_STRING_MODE_BACKGROUND);*/
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("НАЗАД В МЕНЮ",TFT_STRING_MODE_BACKGROUND);
	ILI9486_DrawRect(37,273,414,42,WHITE);
	ILI9486_DrawRect(38,274,412,40,WHITE);
}
void Print_WeatherStation_Info()
{
	if (strcmp(receive_time,"Нет сигнала") == 0)
	{
		ILI9486_SetCursor(155,100);
		ILI9486_Print_String32x32(receive_time,TFT_STRING_MODE_BACKGROUND);
	}
	else
	{
		ILI9486_SetCursor(155,105);
		ILI9486_Print_String18x32(receive_time,TFT_STRING_MODE_BACKGROUND);
	}
	if (strcmp(send_time,"Отправки не было") == 0)
	{
		ILI9486_SetCursor(20,180);
		ILI9486_Print_String32x32(send_time,TFT_STRING_MODE_BACKGROUND);
	}
	else
	{
		ILI9486_SetCursor(20,185);
		ILI9486_Print_String18x32(send_time,TFT_STRING_MODE_BACKGROUND);
	}
}
//Окно статус wifi
void Print_WIFI_Page()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(0,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String32x32("БЕСПРОВОДНАЯ СЕТЬ",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(20,60);
	ILI9486_Print_String32x32("Состояние:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(325,60);
	if (wifi_enable_flag == 1)
	{
		ILI9486_SetTextColor(GREEN,BLACK);
		ILI9486_Print_String32x32("ВКЛ",TFT_STRING_MODE_BACKGROUND);
	}
	else
	{
		ILI9486_SetTextColor(RED,BLACK);
		ILI9486_Print_String32x32("ОТКЛ",TFT_STRING_MODE_BACKGROUND);
	}
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(20,100);
	ILI9486_Print_String32x32("IP:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(120,100);
	ILI9486_Print_String32x32(WiFi_IP,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(20,140);
	ILI9486_Print_String32x32("SSID:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(170,140);
	ILI9486_Print_String32x32(WiFi_SSID,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(20,180);
	ILI9486_Print_String32x32("PSWD:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(170,180);
	ILI9486_Print_String32x32(WiFi_PSWD,TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("НАЗАД В МЕНЮ",TFT_STRING_MODE_BACKGROUND);
	ILI9486_DrawRect(37,273,414,42,WHITE);
	ILI9486_DrawRect(38,274,412,40,WHITE);
}
//Окно Управления газовым котлом
void Print_Gas_Boiler_Page_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(0,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String32x32("  ГАЗОВЫЙ КОТЁЛ  ",TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(20,60);
	ILI9486_Print_String32x32("Состояние:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(20,100);
	ILI9486_Print_String32x32("Уставка Т:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(415,100);
	ILI9486_Print_String32x32("°С",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(415,140);
	ILI9486_Print_String32x32("°С",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(20,140);
	ILI9486_Print_String32x32("Текущая Т:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(20,180);
	ILI9486_Print_String32x32("Последний прием:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(10,220);
	ILI9486_Print_String32x32("(Ch2)",TFT_STRING_MODE_BACKGROUND);
	
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("НАЗАД В МЕНЮ",TFT_STRING_MODE_BACKGROUND);
}
void Print_Gas_Boiler_Page()
{
	char data[10];
	ILI9486_SetCursor(300,63);
	if(gas_boiler_enable_flag == 0)
	{
		ILI9486_SetTextColor(RED,BLACK);
		ILI9486_Print_String32x32("ОТКЛ",TFT_STRING_MODE_BACKGROUND);
	}
	else
	{
		ILI9486_SetTextColor(GREEN,BLACK);
		ILI9486_Print_String32x32("ВКЛ ",TFT_STRING_MODE_BACKGROUND);
	}
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(307,140);
	ILI9486_Print_String32x32("19.2",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(307,100);
	if (gas_boiler_setpoint_change_flag == 1)
	{
		sprintf(data,"%d.%d",gas_boiler_setpoint_temp_counter/10,gas_boiler_setpoint_temp_counter%10);
	} 
	else
	{
		sprintf(data,"%d.%d",gas_boiler_setpoint_temp_integer,gas_boiler_setpoint_temp_fraction);
	}
	ILI9486_Print_String32x32(data,TFT_STRING_MODE_BACKGROUND);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	switch (up_down_count%3)
	{
		case 0:
			ILI9486_DrawRect(297,61,130,37,WHITE);
			ILI9486_DrawRect(298,62,128,35,WHITE);
			ILI9486_DrawRect(303,98,170,37,BLACK);
			ILI9486_DrawRect(304,99,168,35,BLACK);
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;	
		case 1:
			ILI9486_DrawRect(297,61,130,37,BLACK);
			ILI9486_DrawRect(298,62,128,35,BLACK);
			ILI9486_DrawRect(303,98,170,37,WHITE);
			ILI9486_DrawRect(304,99,168,35,WHITE);
			ILI9486_DrawRect(37,273,414,42,BLACK);
			ILI9486_DrawRect(38,274,412,40,BLACK);
			break;
		case 2:
			ILI9486_DrawRect(297,61,130,37,BLACK);
			ILI9486_DrawRect(298,62,128,35,BLACK);
			ILI9486_DrawRect(303,98,170,37,BLACK);
			ILI9486_DrawRect(304,99,168,35,BLACK);
			ILI9486_DrawRect(37,273,414,42,WHITE);
			ILI9486_DrawRect(38,274,412,40,WHITE);
			break;	
	}
	if (strcmp(rx_time_gas_boiler,"Нет сигнала") == 0)
	{
		ILI9486_SetCursor(155,220);
		ILI9486_Print_String32x32(rx_time_gas_boiler,TFT_STRING_MODE_BACKGROUND);
	}
	else
	{
		ILI9486_SetCursor(155,225);
		ILI9486_Print_String18x32(rx_time_gas_boiler,TFT_STRING_MODE_BACKGROUND);
	}
	
}
//-----------Графика вспомогательная для главного окна------//
void DrawWeatherVane()
{
	ILI9486_Draw_Circle(239, 159, 75,WHITE);
	ILI9486_Draw_Circle(239, 159, 76,WHITE);
	
	ILI9486_Draw_Circle(239, 159, 80,WHITE);
	ILI9486_Draw_Circle(239, 159, 81,WHITE);
	
	ILI9486_DrawRect(230, 73,  22, 22,  WHITE);
	ILI9486_DrawRect(231, 74,  20, 20,  WHITE);
	ILI9486_fillRect(232, 75,  18, 18,  BLACK);
	ILI9486_SetTextColor(GREEN, BLACK);
	ILI9486_SetCursor(233, 73);
	ILI9486_Print_String14x24("N",TFT_STRING_MODE_NO_BACKGROUND);
	
	ILI9486_DrawRect(230, 225,  22, 22,  WHITE);
	ILI9486_DrawRect(231, 226,  20, 20,  WHITE);
	ILI9486_fillRect(232, 227,  18, 18,  BLACK);
	ILI9486_SetTextColor(GREEN, BLACK);
	ILI9486_SetCursor(234, 225);
	ILI9486_Print_String14x24("S",TFT_STRING_MODE_NO_BACKGROUND);
	
	ILI9486_DrawRect(153, 149,  22, 22,  WHITE);
	ILI9486_DrawRect(154, 150,  20, 20,  WHITE);
	ILI9486_fillRect(155, 151,  18, 18,  BLACK);
	ILI9486_SetTextColor(GREEN, BLACK);
	ILI9486_SetCursor(158, 149);
	ILI9486_Print_String14x24("W",TFT_STRING_MODE_NO_BACKGROUND);
	
	ILI9486_DrawRect(304, 149,  22, 22,  WHITE);
	ILI9486_DrawRect(305, 150,  20, 20,  WHITE);
	ILI9486_fillRect(306, 151,  18, 18,  BLACK);
	ILI9486_SetTextColor(GREEN, BLACK);
	ILI9486_SetCursor(309, 149);
	ILI9486_Print_String14x24("E",TFT_STRING_MODE_NO_BACKGROUND);
	
	ILI9486_Draw_Line(194, 114, 186, 106, WHITE);
	ILI9486_Draw_Line(195, 113, 187, 105, WHITE);
	ILI9486_Draw_Line(193, 115, 185, 107, WHITE);
	
	ILI9486_Draw_Line(292, 106, 284, 114, WHITE);
	ILI9486_Draw_Line(291, 105, 283, 113, WHITE);
	ILI9486_Draw_Line(293, 107, 285, 115, WHITE);
	
	ILI9486_Draw_Line(292, 212, 284, 204, WHITE);
	ILI9486_Draw_Line(293, 211, 285, 203, WHITE);
	ILI9486_Draw_Line(291, 213, 283, 205, WHITE);
	
	ILI9486_Draw_Line(186, 212, 194, 204, WHITE);
	ILI9486_Draw_Line(185, 211, 193, 203, WHITE);
	ILI9486_Draw_Line(187, 213, 195, 205, WHITE);
	
	ILI9486_Draw_Circle(239, 159, 30,GREEN);
	ILI9486_Draw_Circle(239, 159, 29,GREEN);
	ILI9486_Draw_Circle(239, 159, 28,WHITE);
	ILI9486_Draw_Circle(239, 159, 27,WHITE);
	ILI9486_Draw_Circle(239, 159, 10,WHITE);
	ILI9486_Draw_Circle(239, 159, 9,WHITE);
	ILI9486_fillCircle(239, 159, 8,GREEN);
	
	ILI9486_Draw_Line(239, 129, 237, 149, WHITE);
	ILI9486_Draw_Line(239, 129, 241, 149, WHITE);
	ILI9486_Draw_Line(209, 159, 229, 157, WHITE);
	ILI9486_Draw_Line(209, 159, 229, 161, WHITE);
	ILI9486_Draw_Line(269, 159, 249, 157, WHITE);
	ILI9486_Draw_Line(269, 159, 249, 161, WHITE);
	ILI9486_Draw_Line(239, 189, 237, 169, WHITE);
	ILI9486_Draw_Line(239, 189, 241, 169, WHITE);
	
}
void DrawLevelWiFi()
{
	switch (wifi_enable_flag)
	{
		case 0:
			ILI9486_Draw_Image("/wi0.txt", 40, 40, 165, 2);
			break;
		case 1:
			ILI9486_Draw_Image("/wi4.txt", 40, 40, 165, 2);
			break;
	}
}
void DrawLevelNrfWeather()
{
	switch (rx_count)
	{
		case 0:
			ILI9486_Draw_Image("/nrf0.txt", 30, 30, 280, 10);
			break;
		case 1:
			ILI9486_Draw_Image("/nrf0.txt", 30, 30, 280, 10);
			break;
		case 2:
			ILI9486_Draw_Image("/nrf1.txt", 30, 30, 280, 10);
			break;
		case 3:
			ILI9486_Draw_Image("/nrf2.txt", 30, 30, 280, 10);
			break;
		case 4:
			ILI9486_Draw_Image("/nrf3.txt", 30, 30, 280, 10);
			break;
		case 5:
			ILI9486_Draw_Image("/nrf4.txt", 30, 30, 280, 10);
			break;
		case 6:
			ILI9486_Draw_Image("/nrf4.txt", 30, 30, 280, 10);
			break;
	}
}
void FindLevelNrfGasBoiler(int gas_boiler_rx_counter, int gas_boiler_rx_counter_old)
{
	if ((gas_boiler_rx_counter - gas_boiler_rx_counter_old) >= 7)
	{
		gas_boiler_rx_level = 4;
	}
	else if (((gas_boiler_rx_counter - gas_boiler_rx_counter_old) < 7) && ((gas_boiler_rx_counter - gas_boiler_rx_counter_old) >= 5))
	{
		gas_boiler_rx_level = 3;
	}
	else if (((gas_boiler_rx_counter - gas_boiler_rx_counter_old) < 5) && ((gas_boiler_rx_counter - gas_boiler_rx_counter_old) >= 3))
	{
		gas_boiler_rx_level = 2;
	}
	else if (((gas_boiler_rx_counter - gas_boiler_rx_counter_old) < 3) && ((gas_boiler_rx_counter - gas_boiler_rx_counter_old) >= 1))
	{
		gas_boiler_rx_level = 1;
	}
	else if ((gas_boiler_rx_counter - gas_boiler_rx_counter_old) < 1)
	{
		gas_boiler_rx_level = 0;
	}
}
void DrawLevelNrfGasBoiler()
{
	switch (gas_boiler_rx_level)
	{
		case 0:
		ILI9486_Draw_Image("/nrf0.txt", 30, 30, 229, 10);
		break;
		case 1:
		ILI9486_Draw_Image("/nrf1.txt", 30, 30, 229, 10);
		break;
		case 2:
		ILI9486_Draw_Image("/nrf2.txt", 30, 30, 229, 10);
		break;
		case 3:
		ILI9486_Draw_Image("/nrf3.txt", 30, 30, 229, 10);
		break;
		case 4:
		ILI9486_Draw_Image("/nrf4.txt", 30, 30, 229, 10);
		break;
	}
}
void DrawWindDirect()
{
	//sprintf(wind_direction,"%s","N");
	if (!strcmp(wind_direction, "N"))
	{
		ILI9486_fillTriangle(239, 99, 233, 124, 239, 115, GREEN);
		ILI9486_fillTriangle(239, 99, 239, 115, 245, 124, GREEN);
	}
	else if (!strcmp(wind_direction, "S"))
	{
		ILI9486_fillTriangle(233, 196, 239, 221, 239, 205, GREEN);
		ILI9486_fillTriangle(239, 221, 239, 205, 245, 196, GREEN);
	}
	else if (!strcmp(wind_direction, "E"))
	{
		ILI9486_fillTriangle(300, 159, 284, 159, 275, 153, GREEN);
		ILI9486_fillTriangle(300, 159, 284, 159, 275, 165, GREEN);
	}
	else if (!strcmp(wind_direction, "W"))
	{
		ILI9486_fillTriangle(179, 159, 195, 159, 204, 153, GREEN);
		ILI9486_fillTriangle(179, 159, 195, 159, 204, 165, GREEN);
	}
	else if (!strcmp(wind_direction, "N-E"))
	{
		ILI9486_fillTriangle(281, 117, 259, 130, 270, 128, GREEN);
		ILI9486_fillTriangle(281, 117, 268, 139, 270, 128, GREEN);
	}
	else if (!strcmp(wind_direction, "N-W"))
	{
		ILI9486_fillTriangle(197, 117, 210, 139, 208, 128, GREEN);
		ILI9486_fillTriangle(197, 117, 208, 128, 219, 130, GREEN);
	}
	else if (!strcmp(wind_direction, "S-E"))
	{
		ILI9486_fillTriangle(281, 201, 259, 188, 270, 190, GREEN);
		ILI9486_fillTriangle(281, 201, 270, 190, 268, 179, GREEN);
	}
	else if (!strcmp(wind_direction, "S-W"))
	{
		ILI9486_fillTriangle(197, 201, 219, 188, 208, 190, GREEN);
		ILI9486_fillTriangle(197, 201, 210, 179, 208, 190, GREEN);
	}
}
void Clock (void)
{		
	//Читаем время (для DS3231) - по сути функция RTC_read_time
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010000);			// передача адреса устройства, режим записи
	I2C_SendByte(0x00);				    // передача адреса памяти
	I2C_StopCondition();                // остановка i2c
	
	I2C_StartCondition();               // запуск i2c
	I2C_SendByte(0b11010001);			// передача адреса устройства, режим чтения
	sec = RTC_ConvertFromDec(I2C_ReadByte());     // чтение секунд, ACK
	min = RTC_ConvertFromDec(I2C_ReadByte());     // чтение минут, ACK
	hour = RTC_ConvertFromDec(I2C_ReadByte());    // чтение часов, ACK
	day = RTC_ConvertFromDec(I2C_ReadByte());     // чтение день недели, ACK
	date = RTC_ConvertFromDec(I2C_ReadByte());    // чтение число, ACK
	month = RTC_ConvertFromDec(I2C_ReadByte());   // чтение месяц, ACK
	year = RTC_ConvertFromDec(I2C_ReadLastByte());// чтение год, NACK
	I2C_StopCondition();                // остановка i2c
	
	Convert_to_string_Clock();
}
void Convert_to_string_Clock(void)
{
	memset(T_Param.seconds, 0, sizeof(char) * strlen(T_Param.seconds));//очистка массива
	memset(T_Param.minutes, 0, sizeof(char) * strlen(T_Param.minutes));//очистка массива
	memset(T_Param.hours, 0, sizeof(char) * strlen(T_Param.hours));//очистка массива
	memset(T_Param.mounthday, 0, sizeof(char) * strlen(T_Param.mounthday));//очистка массива
	memset(T_Param.Year, 0, sizeof(char) * strlen(T_Param.Year));//очистка массива
	memset(T_Param.weakday, 0, sizeof(char) * strlen(T_Param.weakday));//очистка массива
	memset(T_Param.Mounth, 0, sizeof(char) * strlen(T_Param.Mounth));//очистка массива
	if ((sec >= 0) && (sec <= 9)) {
		sprintf(T_Param.seconds,"0%d",sec);
	}
	else {
		sprintf(T_Param.seconds,"%d",sec);
	}
	if ((min >= 0) && (min <= 9)) {
		sprintf(T_Param.minutes,"0%d",min);
	}
	else {
		sprintf(T_Param.minutes,"%d",min);
	}
	if ((hour >= 0) && (hour <= 9)) {
		sprintf(T_Param.hours,"0%d",hour);
	}
	else {
		sprintf(T_Param.hours,"%d",hour);
	}
	if ((date >= 0) && (date <= 9)) {
		sprintf(T_Param.mounthday,"0%d",date);
	}
	else {
		sprintf(T_Param.mounthday,"%d",date);
	}
	if ((year >= 0) && (year <= 9)) {
		sprintf(T_Param.Year,"0%d",year);
	}
	else {
		sprintf(T_Param.Year,"%d",year);
	}
	switch (day)
	{
		case 1:
		sprintf(T_Param.weakday,"MONDAY   ");
		break;
		case 2:
		sprintf(T_Param.weakday,"TUESDAY  ");
		break;
		case 3:
		sprintf(T_Param.weakday,"WEDNESDAY");
		break;
		case 4:
		sprintf(T_Param.weakday,"THURSDAY ");
		break;
		case 5:
		sprintf(T_Param.weakday,"FRIDAY   ");
		break;
		case 6:
		sprintf(T_Param.weakday,"SATURDAY ");
		break;
		case 7:
		sprintf(T_Param.weakday,"SUNDAY   ");
		break;
		default:
		break;
	}
	switch (month)
	{
		case 1:
		sprintf(T_Param.Mounth,"01");
		break;
		case 2:
		sprintf(T_Param.Mounth,"02");
		break;
		case 3:
		sprintf(T_Param.Mounth,"03");
		break;
		case 4:
		sprintf(T_Param.Mounth,"04");
		break;
		case 5:
		sprintf(T_Param.Mounth,"05");
		break;
		case 6:
		sprintf(T_Param.Mounth,"06");
		break;
		case 7:
		sprintf(T_Param.Mounth,"07");
		break;
		case 8:
		sprintf(T_Param.Mounth,"08");
		break;
		case 9:
		sprintf(T_Param.Mounth,"09");
		break;
		case 10:
		sprintf(T_Param.Mounth,"10");
		break;
		case 11:
		sprintf(T_Param.Mounth,"11");
		break;
		case 12:
		sprintf(T_Param.Mounth,"12");
		break;
		default:
		break;
	}
}
//Вычисление кол-ва осадков
int RAIN_AMOUNT(char *adc_value)
{
	int RAIN;
	int R;
	if (strlen(adc_value) != 0)
	{
		sscanf(adc_value, "%d", &R);
	}
	else
	{
		return 0;
	}
	RAIN = R*0.144;
	if (RAIN > 100)
	{
		RAIN = 100;
	}
	RAIN = 100 - RAIN;
	return RAIN;
}
//Вычисление уровня заряда АКБ
int V_BAT (char *adc_value)
{
	int VBAT;
	int V;
	if (strlen(adc_value) != 0)
	{
		sscanf(adc_value, "%d", &V);
	}
	else
	{
		V = 0;
	}
	VBAT = V*0.68;
	
	return VBAT;
}
//Вычисление скорости ветра
int wind_speed (char *counter)
{
	int speed = 0;
	int n;
	if (strlen(counter) != 0)
	{
		sscanf(counter, "%d", &n);
	}
	else
	{
		n = 0;
	}
	speed = 2*314*RADIUS*n/4; //период 2 сек+2 магнита
	
	return speed;
}
//вычисление домашних показателей
void sprintf_HOME_Weath_Param(void)
{
	wdt_reset();
	//измерение температуры
	int tt = 0; 
	tt = dt_check(); 
	home_temp_sign = tt>>11;//вычисление знака температуры
	if (home_temp_sign == 0x00)
	{
		home_temp_fraction = tt & 0xF;
		home_temp_fraction = (home_temp_fraction<<1) + (home_temp_fraction<<3);// ”множаем на 10
		home_temp_fraction = (home_temp_fraction>>4);//делим на 16 или умножаем на 0.0625
		home_temp_integer = (tt&0x07FF)>>4;
		//sprintf(data,"%d,%d\r",buf[1],buf[0]);
		sprintf(temp_home_to_DB,"%d.%d",home_temp_integer,home_temp_fraction);
		
	}
	else
	{
		home_temp_fraction = ((~tt) & 0xF);
		home_temp_fraction = (home_temp_fraction<<1) + (home_temp_fraction<<3);// ”множаем на 10
		home_temp_fraction = (home_temp_fraction>>4);//делим на 16 или умножаем на 0.0625
		home_temp_integer = ((~(tt))&0x07FF)>>4;
		//sprintf(data,"-%d,%d\r",buf[1],buf[0]);
		sprintf(temp_home_to_DB,"-%d.%d",home_temp_integer,home_temp_fraction);
	}
	//измерение влажности
	uint8_t data[5] = {0};
	int home_hum;
	if (dht22_GetData(data))
	{
		//data[1]-младший бит температуры
		//data[2]-старший бит температуры
		//data[3]-младший бит влажности
		//data[4]-старший бит влажности
		home_hum = ((data[4]<<8)|data[3]) / 10;
	}
	if (home_hum < 100)
	{
		home_hum_integer = home_hum;
		sprintf(hum_home_to_DB,"%d",home_hum_integer);
	}
	//измерение атмосферного давления
	pressure_home = BMP180_calculation()*0.0075;
	sprintf(Press_home_to_DB,"%d",pressure_home);
	
	wdt_reset();
}
//первичная инициализация параметров погоды
void Weath_Param_ini()
{
	strcpy(temp_street_to_DB,"NULL");
	strcpy(hum_street_to_DB,"NULL");
	strcpy(WIND_speed_to_DB,"NULL");
	strcpy(wind_direction_to_DB,"NULL");
	strcpy(Vbat_to_DB,"NULL");
	strcpy(Rain_to_DB,"NULL");
	
	home_temp_sign = 0;
	home_temp_integer = 0;
	home_temp_fraction = 0;
	home_hum_integer = 0;

	street_temp_sign = 0;
	street_temp_integer = 0;
	street_temp_fraction = 0;
	street_hum_integer = 0;
	wind_speed_integer = 0;
	wind_speed_fraction = 0;
	rain = 0;
	V_Bat_integer = 0;
	V_Bat_fraction = 0;
	wind_direction[0] = '-';
}