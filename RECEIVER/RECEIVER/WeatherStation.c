/*
 * WeatherStation.c
 *
 * Created: 03.06.2022 19:15:57
 *  Author: Alex2
 */ 
#include "WeatherStation.h"
#include "Images.h"

#define RADIUS 0.139

char temp_street[6] = {0};
char hum_street[5] = {0};
char temp_home[6] = {0};
char hum_home[5] = {0};
char WIND_speed[5] = {0};
char wind_direction[6];
char Vbat[5] = {0};
char Rain[6] = {0};
char Press_home[6] = {0};

char temp_street_to_DB[6] = {0};
char hum_street_to_DB[5] = {0};
char WIND_speed_to_DB[5] = {0};
char wind_direction_to_DB[6] = {0};
char Vbat_to_DB[5] = {0};
char Rain_to_DB[6] = {0};

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
extern char adc_value1[6];
extern char HALL_counter[5];
extern char adc_value2[6];

extern uint8_t change_flag;
extern int8_t up_down_count;
extern int8_t add_cnt;
extern uint8_t receive_counter;
extern uint8_t WiFi_Flag;


uint8_t home_temp_sign;//знак домаш темп
uint8_t home_temp_integer;//целая часть домаш темп
uint8_t home_temp_fraction;//дробная часть домаш темп
int home_hum_integer;//целая часть домаш влажн

extern uint8_t street_temp_sign;//знак уличн темп
extern uint8_t street_temp_integer;//целая часть уличн темп
extern uint8_t street_temp_fraction;//дробная часть уличн темп

extern uint8_t wind_speed_integer;//целая часть ск ветра
extern uint8_t wind_speed_fraction;//дробная часть ск ветра

//Окно приветсвия на экране дисплея
void Print_Hello_World()
{	
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,80);
	ILI9486_Print_String32x32("Вас приветствует",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(20,150);
	ILI9486_Print_String32x32("система Умный Дом!",TFT_STRING_MODE_NO_BACKGROUND);
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
	ILI9486_SetCursor(80,130);
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
	//вывод границ
	Print_Boards_Main();
	//вывод статики флюгера
	DrawWeatherVane();
	//вывод надписи для скорости ветра
	ILI9486_SetTextColor(BLUE,BLACK);
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
	ILI9486_Print_String14x24("D",TFT_STRING_MODE_BACKGROUND);
}
//Главное окно (уличные показания)
void Print_Home_Page_Out()
{
	char data[10];
	//вывод скорости ветра
	ILI9486_SetTextColor(LIGHTBLUE,BLACK);
	wind_speed_integer = 9;
	wind_speed_fraction = 44;
	if(wind_speed_integer < 10)
	{
		sprintf(data," %d",wind_speed_integer);
		ILI9486_SetCursor(171,280);
		ILI9486_Print_String18x32(data,TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(205,280);
		ILI9486_Print_String18x32(".",TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(222,280);
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
		sprintf(data,"%d",wind_speed_fraction);
		ILI9486_Print_String18x32(data,TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(258,280);
		ILI9486_Print_String18x32("m/",TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(292,280);
		ILI9486_Print_String18x32("s",TFT_STRING_MODE_BACKGROUND);
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	else
	{
		sprintf(data,"%d",wind_speed_integer);
		ILI9486_SetCursor(171,280);
		ILI9486_Print_String18x32(data,TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(205,280);
		ILI9486_Print_String18x32(".",TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(222,280);
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
		sprintf(data,"%d",wind_speed_fraction);
		ILI9486_Print_String18x32(data,TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(258,280);
		ILI9486_Print_String18x32("m/",TFT_STRING_MODE_BACKGROUND);
		ILI9486_SetCursor(292,280);
		ILI9486_Print_String18x32("s",TFT_STRING_MODE_BACKGROUND);
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	//вывод уличных показаний
	ILI9486_SetTextColor(YELLOW,BLACK);
	if (street_temp_sign != 0x00)
	{
		ILI9486_SetCursor(395,250);
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
		ILI9486_SetCursor(424,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//дробная часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	else
	{
		ILI9486_SetCursor(395,250);
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
		ILI9486_SetCursor(424,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	ILI9486_SetCursor(457,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_BACKGROUND);//ед измерения
	//влажность
	ILI9486_SetCursor(361,150);
	ILI9486_Print_String32x48("30",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(429,165);
	ILI9486_Print_String18x32("%",TFT_STRING_MODE_BACKGROUND);//ед измерения
	//вывод флюгера
	DrawWindDirect();
	//вывод прогноза погоды
	DrawSun();

	/*uint8_t Frame_buffer[1024] = { 0 }; //Буфер кадра
	char TIME[10] = {0};
	
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	//-----------Вывод времени/даты-------------------------------//
	Clock();
	sprintf(TIME,"%s:%s",T_Param.hours, T_Param.minutes);
	LCD_12864_Print_11_16(2, 0, 0, TIME, Frame_buffer);
	LCD_12864_Decode_UTF8(63, 0, 0, T_Param.seconds, Frame_buffer);
	LCD_12864_Decode_UTF8(61, 1, 0, T_Param.weakday, Frame_buffer);
	LCD_12864_Decode_UTF8(73, 1, 0, ",", Frame_buffer);
	LCD_12864_Decode_UTF8(76, 1, 0, T_Param.mounthday, Frame_buffer);
	LCD_12864_Decode_UTF8(88, 1, 0, "/", Frame_buffer);
	LCD_12864_Decode_UTF8(93, 1, 0, T_Param.Mounth, Frame_buffer);
	LCD_12864_Decode_UTF8(111, 1, 0, "/", Frame_buffer);
	LCD_12864_Decode_UTF8(116, 1, 0, T_Param.Year, Frame_buffer);
	//-----------Вывод индикатора WiFi-------------------------------//
	if (WiFi_Flag == 1)
	{
		LCD_12864_Decode_UTF8(100, 0, 0, "Wi-Fi", Frame_buffer);
	}
	//-----------Вывод уровня приема сигнала----------------------//
	LCD_12864_Draw_line(82, 6, 82, 0, Frame_buffer);
	LCD_12864_Draw_line(79, 0, 85, 0, Frame_buffer);
	LCD_12864_Draw_pixel(80, 1, Frame_buffer);
	LCD_12864_Draw_pixel(81, 2, Frame_buffer);
	LCD_12864_Draw_pixel(82, 3, Frame_buffer);
	LCD_12864_Draw_pixel(84, 1, Frame_buffer);
	LCD_12864_Draw_pixel(83, 2, Frame_buffer);
		for (uint8_t k = 1; k < receive_counter+1; k++)
		{
			LCD_12864_Draw_line(82 + 2*k, 6, 82 + 2*k, 7-k, Frame_buffer);
		}
	//-----------Вывод уличных показателей-----------------------//
	LCD_12864_Decode_UTF8(30, 4, 0, "°C", Frame_buffer);
	LCD_12864_Decode_UTF8(57, 4, 0, "%", Frame_buffer);
	LCD_12864_Print_5_8(46, 4, 0, hum_street, Frame_buffer);
	if (strchr(temp_street,'-') != 0) {LCD_12864_Print_5_8(0, 4, 0, temp_street, Frame_buffer);}
	else {LCD_12864_Print_5_8(7, 4, 0, temp_street, Frame_buffer);}
	LCD_12864_Print_4_6(44, 2, 0, Vbat, Frame_buffer);
	LCD_12864_Print_4_6(26, 5, 0, WIND_speed, Frame_buffer);
	LCD_12864_Decode_UTF8(45, 5, 0, "м/с", Frame_buffer);
	LCD_12864_Decode_UTF8(30, 6, 0, "осадки", Frame_buffer);
	LCD_12864_Draw_line(27, 52, 24, 52, Frame_buffer);
	LCD_12864_Draw_line(24, 52, 24, 59, Frame_buffer);
	LCD_12864_Draw_line(24, 59, 28, 59, Frame_buffer);
	LCD_12864_Draw_pixel(27, 58, Frame_buffer);
	LCD_12864_Draw_pixel(27, 60, Frame_buffer);
	LCD_12864_Draw_pixel(26, 57, Frame_buffer);
	LCD_12864_Draw_pixel(26, 61, Frame_buffer);
	if (RAIN_AMOUNT(adc_value2) <= 10)  {LCD_12864_Decode_UTF8(30, 7, 0, "отсутств.", Frame_buffer);}
	else if ((RAIN_AMOUNT(adc_value2) > 10) && (RAIN_AMOUNT(adc_value2) <= 50))  {LCD_12864_Decode_UTF8(30, 7, 0, "слабые", Frame_buffer);}
    else if ((RAIN_AMOUNT(adc_value2) > 50) && (RAIN_AMOUNT(adc_value2) <= 75)) { LCD_12864_Decode_UTF8(30, 7, 0, "умеренные", Frame_buffer);}
	else if ((RAIN_AMOUNT(adc_value2) > 75) && (RAIN_AMOUNT(adc_value2) <= 100))  {LCD_12864_Decode_UTF8(30, 7, 0, "сильные", Frame_buffer);}
	else {LCD_12864_Decode_UTF8(30, 7, 0, "нет данных", Frame_buffer);}
	//-----------Вывод показателей в доме-----------------------//
	LCD_12864_Decode_UTF8(95, 4, 0, "°C", Frame_buffer);
	if (strchr(temp_home,'-') != 0) {LCD_12864_Print_5_8(65, 4, 0, temp_home, Frame_buffer);}
	else {LCD_12864_Print_5_8(72, 4, 0, temp_home, Frame_buffer);}
	LCD_12864_Decode_UTF8(122, 4, 0, "%", Frame_buffer);
	LCD_12864_Print_5_8(111, 4, 0, hum_home, Frame_buffer);
	LCD_12864_Print_4_6(86, 2, 0, Press_home, Frame_buffer);
	LCD_12864_Decode_UTF8(101, 2, 0, "мм", Frame_buffer);
	LCD_12864_Decode_UTF8(114, 2, 0, "Hg", Frame_buffer);
	
	//-----------Вывод разделительных линий-----------------------//
	LCD_12864_Decode_UTF8(2, 2, 0, "улица", Frame_buffer);
	LCD_12864_Decode_UTF8(65, 2, 0, "дом", Frame_buffer);
	LCD_12864_Draw_line(0, 16, 128, 16, Frame_buffer);
	LCD_12864_Draw_line(0, 24, 32, 24, Frame_buffer);
	LCD_12864_Draw_line(32, 17, 32, 24, Frame_buffer);
	LCD_12864_Draw_line(0, 17, 0, 24, Frame_buffer);
	LCD_12864_Draw_line(64, 24, 83, 24, Frame_buffer);
	LCD_12864_Draw_line(83, 17, 83, 24, Frame_buffer);
	LCD_12864_Draw_line(63, 17, 63, 41, Frame_buffer);
	LCD_12864_Draw_line(63, 41, 128, 41, Frame_buffer);
	//-----------Вывод индикатора АКБ-----------------------//
	DrawBattery(Frame_buffer);
	//-----------Вывод индикатора флюгера-----------------------//
	DrawWeatherVane(Frame_buffer);
	//-----------Вывод прогноза погоды-----------------------//
	if ((wind_speed (HALL_counter) == 0) && (RAIN_AMOUNT(adc_value2) <= 10))
	{
		DrawSun(Frame_buffer);
	}
	else if ((wind_speed (HALL_counter) > 1000))
	{
		DrawClouds(Frame_buffer);
	}
	else if ((wind_speed (HALL_counter) > 0) && (RAIN_AMOUNT(adc_value2) <= 10))
	{
		DrawSunWithClouds(Frame_buffer);
	}
	else if ((RAIN_AMOUNT(adc_value2) > 15) && (RAIN_AMOUNT(adc_value2) <= 100))
	{
		DrawCloudsWithRain(Frame_buffer);
	}
	else
	{
		DrawSun(Frame_buffer);
	}
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);*/
}
//Главное окно(домашние показания)
void Print_Home_Page_In()
{
	char data[10];
	char TIME[10] = {0};
	char DATE[10] = {0};
	//вывод времени/даты
	Clock();
	sprintf(TIME,"%s:%s",T_Param.hours, T_Param.minutes);
	sprintf(DATE,"%s/%s/%s",T_Param.mounthday, T_Param.Mounth, T_Param.Year);
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(10,15);
	ILI9486_Print_String24x32(TIME,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(12,55);
	ILI9486_Print_String14x24(T_Param.weakday,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(9,80);
	ILI9486_Print_String18x32(DATE,TFT_STRING_MODE_BACKGROUND);
	wdt_reset();
	//вывод домашних показаний
	//температура
	ILI9486_SetTextColor(ORANGE,BLACK);
	if (home_temp_sign != 0x00)
	{
		ILI9486_SetCursor(79,250);
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
		ILI9486_SetCursor(110,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//дробная часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	else
	{
		ILI9486_SetCursor(79,250);
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
		ILI9486_SetCursor(110,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//целая часть
		memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	}
	ILI9486_SetCursor(143,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_BACKGROUND);//ед измерения
	//влажность
	ILI9486_SetCursor(47,150);
	sprintf(data,"%d",home_hum_integer);
	ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(115,165);
	memset(data, 0, sizeof(char) * strlen(data));//очистка массива
	ILI9486_Print_String18x32("%",TFT_STRING_MODE_BACKGROUND);//ед измерения
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
	ILI9486_SetTextColor(ORANGE, BLACK);
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
	ILI9486_SetCursor(40,70);
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_Print_String40x40("Главное окно",TFT_STRING_MODE_BACKGROUND);							   
	ILI9486_SetCursor(40,110);
	ILI9486_Print_String40x40("Дата и время",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,150);
	ILI9486_Print_String40x40(" О проекте",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,190);
	ILI9486_Print_String40x40("Доп сведения",TFT_STRING_MODE_BACKGROUND);
}
//Окно меню динамич
void Print_Menu_Page()
{
	switch (up_down_count%4)
	{
		case 0: 
				ILI9486_fillTriangle(22,72,37,88,22,104,WHITE);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,WHITE);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				break;
		case 1: 
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,WHITE);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,WHITE);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				break;
		case 2: 
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,WHITE);
				ILI9486_fillTriangle(22,192,37,208,22,224,BLACK);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,WHITE);
				ILI9486_fillTriangle(456,192,441,208,456,224,BLACK);
				break;
		case 3: 
				ILI9486_fillTriangle(22,72,37,88,22,104,BLACK);
				ILI9486_fillTriangle(22,112,37,128,22,144,BLACK);
				ILI9486_fillTriangle(22,152,37,168,22,184,BLACK);
				ILI9486_fillTriangle(22,192,37,208,22,224,WHITE);
				
				ILI9486_fillTriangle(456,72,441,88,456,104,BLACK);
				ILI9486_fillTriangle(456,112,441,128,456,144,BLACK);
				ILI9486_fillTriangle(456,152,441,168,456,184,BLACK);
				ILI9486_fillTriangle(456,192,441,208,456,224,WHITE);
		break;
	}
}
//Окно настроек времени
void Print_Page_Clock_Settings_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(40,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("ДАТА и ВРЕМЯ",TFT_STRING_MODE_BACKGROUND);
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
	switch (up_down_count%8)
	{
		case 0:	
			/*if(change_flag)
			{
				hour = add_cnt;
			}*/
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
			/*if(change_flag)
			{
				min = add_cnt;
			}*/
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
			/*if(change_flag)
			{
				day = add_cnt;
			}*/
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
			/*if(change_flag)
			{
				date = add_cnt;
			}*/
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
			/*if(change_flag)
			{
				month = add_cnt;
			}*/
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
			/*if(change_flag)
			{
				year = add_cnt;
			}*/
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
			ILI9486_DrawRect(457,97,67,54,BLACK);
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
	/*uint8_t cnt;
	uint8_t Frame_buffer[1024] = { 0 }; //Буфер кадра
	Convert_to_string_Clock();
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	
	LCD_12864_Decode_UTF8(15, 0, 1, "НАСТРОЙКА ВРЕМЕНИ", Frame_buffer);
	switch (cnt%7)
	{
		case 0:	if(change_flag)
				{
					hour = add_cnt;
				}
				LCD_12864_Print_11_16(32, 2, 1, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 0, "Назад в МЕНЮ", Frame_buffer);
				break;	
		case 1: if(change_flag)
				{
					min = add_cnt;
				}
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 1, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 0, "Назад в МЕНЮ", Frame_buffer);
				break;
		case 2: if(change_flag)
				{
					day = add_cnt;
				}
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 1, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 0, "Назад в МЕНЮ", Frame_buffer);
				break;
		case 3: if(change_flag)
				{
					date = add_cnt;
				}
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 1, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 0, "Назад в МЕНЮ", Frame_buffer);
				break;
		case 4: if(change_flag)
				{
					month = add_cnt;
				}
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 1, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 0, "Назад в МЕНЮ", Frame_buffer);
				break;
		case 5: if(change_flag)
				{
					year = add_cnt;
				}
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 1, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 0, "Назад в МЕНЮ", Frame_buffer);
				break;
		case 6:
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 1, "Назад в МЕНЮ", Frame_buffer);
				break;
	}
	LCD_12864_Decode_UTF8(55, 3, 0, "ч", Frame_buffer);
	LCD_12864_Decode_UTF8(85, 3, 0, "мин", Frame_buffer);
	LCD_12864_Decode_UTF8(42, 5, 0, ",", Frame_buffer);
	LCD_12864_Decode_UTF8(63, 5, 0, "/", Frame_buffer);
	LCD_12864_Decode_UTF8(89, 5, 0, "/", Frame_buffer);

	
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);*/
}
//Окно справки
void Print_Page_About()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(80,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("О ПРОЕКТЕ",TFT_STRING_MODE_BACKGROUND);
	
	
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
//Окно доп инфо
void Print_Page_Dop_Info()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(40,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("ДОП СВЕДЕНИЯ",TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(150,70);
	ILI9486_Print_String18x32("Start time:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(90,100);
	ILI9486_Print_String18x32(start_time,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,130);
	ILI9486_Print_String18x32("Outdoor transmitter:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(50,160);
	ILI9486_Print_String18x32(receive_time,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,190);
	ILI9486_Print_String18x32("Time of sending to DB:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(50,220);
	ILI9486_Print_String18x32(send_time,TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("НАЗАД В МЕНЮ",TFT_STRING_MODE_BACKGROUND);
	ILI9486_DrawRect(37,273,414,42,WHITE);
	ILI9486_DrawRect(38,274,412,40,WHITE);
}
//-----------Графика вспомогательная для главного окна------//
void DrawSun()
{
	FATFS fs;
	WORD s1;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/sun.txt");

	uint8_t array1[6010];
	uint16_t sun[1000];
	int l, p = 0;
	for (int k = 0; k < 10; k++)
	{
		memset(array1, 0, sizeof(uint8_t) * strlen(array1));//очистка массива
		char arr[10] = {};
		p = 0;
		pf_read(array1,6000,&s1);
		wdt_reset();
		for (int m = 0; m < 1000; m++)
		{
			l = 0;
			while (array1[p] != ',')
			{
				arr[l] = array1[p];
				p++;
				l++;
			}
			p += 2;
			sun[m+1000*k] = (uint16_t)strtol(arr,NULL,16);
			ILI9486_drawPixel(350+m%100, 5+10*k+(m/100), (uint16_t)strtol(arr,NULL,16));
			wdt_reset();
		}
	}
	pf_mount(0x00);
}
void DrawSunWithClouds(uint8_t *Frame_buffer)
{
	//солнце
	LCD_12864_Draw_circle(110, 53, 5, Frame_buffer);
	LCD_12864_Draw_line(110, 45, 110, 43, Frame_buffer);
	LCD_12864_Draw_line(118, 53, 120, 53, Frame_buffer);
	LCD_12864_Draw_line(110, 61, 110, 63, Frame_buffer);
	LCD_12864_Draw_line(116, 48, 118, 46, Frame_buffer);
	LCD_12864_Draw_line(116, 58, 118, 60, Frame_buffer);
	//облако
	LCD_12864_Draw_line(90, 57, 106, 57, Frame_buffer);
	LCD_12864_Draw_pixel(89, 56, Frame_buffer);
	LCD_12864_Draw_pixel(88, 55, Frame_buffer);
	LCD_12864_Draw_line(87, 52, 87, 54, Frame_buffer);
	/*LCD_12864_Draw_pixel(87, 54, Frame_buffer);
	LCD_12864_Draw_pixel(87, 53, Frame_buffer);
	LCD_12864_Draw_pixel(87, 52, Frame_buffer);*/
	LCD_12864_Draw_pixel(88, 51, Frame_buffer);
	LCD_12864_Draw_line(89, 50, 93, 50, Frame_buffer);
	/*LCD_12864_Draw_pixel(89, 50, Frame_buffer);
	LCD_12864_Draw_pixel(90, 50, Frame_buffer);
	LCD_12864_Draw_pixel(91, 50, Frame_buffer);
	LCD_12864_Draw_pixel(92, 50, Frame_buffer);
	LCD_12864_Draw_pixel(93, 50, Frame_buffer);*/
	LCD_12864_Draw_pixel(94, 51, Frame_buffer);
	LCD_12864_Draw_line(92, 46, 92, 49, Frame_buffer);
	/*LCD_12864_Draw_pixel(92, 49, Frame_buffer);
	LCD_12864_Draw_pixel(92, 48, Frame_buffer);
	LCD_12864_Draw_pixel(92, 47, Frame_buffer);
	LCD_12864_Draw_pixel(92, 46, Frame_buffer);*/
	LCD_12864_Draw_pixel(93, 45, Frame_buffer);
	LCD_12864_Draw_line(94, 44, 100, 44, Frame_buffer);
	/*LCD_12864_Draw_pixel(94, 44, Frame_buffer);
	LCD_12864_Draw_pixel(95, 44, Frame_buffer);
	LCD_12864_Draw_pixel(96, 44, Frame_buffer);
	LCD_12864_Draw_pixel(97, 44, Frame_buffer);
	LCD_12864_Draw_pixel(98, 44, Frame_buffer);
	LCD_12864_Draw_pixel(99, 44, Frame_buffer);
	LCD_12864_Draw_pixel(100, 44, Frame_buffer);*/
	
	LCD_12864_Draw_pixel(101, 45, Frame_buffer);
	LCD_12864_Draw_line(102, 46, 102, 48, Frame_buffer);
	/*LCD_12864_Draw_pixel(102, 46, Frame_buffer);
	LCD_12864_Draw_pixel(102, 47, Frame_buffer);
	LCD_12864_Draw_pixel(102, 48, Frame_buffer);*/
	LCD_12864_Draw_pixel(101, 49, Frame_buffer);
	LCD_12864_Draw_line(102, 48, 106, 48, Frame_buffer);
	/*LCD_12864_Draw_pixel(102, 48, Frame_buffer);
	LCD_12864_Draw_pixel(103, 48, Frame_buffer);
	LCD_12864_Draw_pixel(104, 48, Frame_buffer);
	LCD_12864_Draw_pixel(105, 48, Frame_buffer);
	LCD_12864_Draw_pixel(106, 48, Frame_buffer);*/
	
	LCD_12864_Draw_pixel(107, 56, Frame_buffer);
	LCD_12864_Draw_pixel(108, 55, Frame_buffer);
	LCD_12864_Draw_line(109, 51, 109, 54, Frame_buffer);
	/*LCD_12864_Draw_pixel(109, 54, Frame_buffer);
	LCD_12864_Draw_pixel(109, 53, Frame_buffer);
	LCD_12864_Draw_pixel(109, 52, Frame_buffer);
	LCD_12864_Draw_pixel(109, 51, Frame_buffer);*/
	LCD_12864_Draw_pixel(108, 50, Frame_buffer);
	LCD_12864_Draw_pixel(107, 49, Frame_buffer);
	//очистка куска солнца
	LCD_12864_Clean_pixel(106, 50, Frame_buffer);
	for(uint8_t i = 51; i <= 55;i++)
	{
		LCD_12864_Clean_pixel(105, i, Frame_buffer);
	}
	/*LCD_12864_Clean_pixel(105, 51, Frame_buffer);
	LCD_12864_Clean_pixel(105, 52, Frame_buffer);
	LCD_12864_Clean_pixel(105, 53, Frame_buffer);
	LCD_12864_Clean_pixel(105, 54, Frame_buffer);
	LCD_12864_Clean_pixel(105, 55, Frame_buffer);*/
	LCD_12864_Clean_pixel(106, 56, Frame_buffer);
	LCD_12864_Draw_pixel(106, 48, Frame_buffer);
	LCD_12864_Draw_pixel(105, 48, Frame_buffer);
	//закраска солнца
	LCD_12864_Draw_line(108, 49, 113, 49, Frame_buffer);
	LCD_12864_Draw_line(109, 50, 114, 50, Frame_buffer);
	LCD_12864_Draw_line(110, 51, 115, 51, Frame_buffer);
	LCD_12864_Draw_line(110, 52, 115, 52, Frame_buffer);
	LCD_12864_Draw_line(110, 53, 115, 53, Frame_buffer);
	LCD_12864_Draw_line(110, 54, 115, 54, Frame_buffer);
	LCD_12864_Draw_line(109, 55, 114, 55, Frame_buffer);
	LCD_12864_Draw_line(108, 56, 113, 56, Frame_buffer);
	LCD_12864_Draw_line(107, 57, 112, 57, Frame_buffer);
}
void DrawClouds(uint8_t *Frame_buffer)
{
	//облако№1
	LCD_12864_Draw_line(90, 57, 106, 57, Frame_buffer);
	LCD_12864_Draw_pixel(89, 56, Frame_buffer);
	LCD_12864_Draw_pixel(88, 55, Frame_buffer);
	LCD_12864_Draw_line(87, 52, 87, 54, Frame_buffer);
	/*LCD_12864_Draw_pixel(87, 54, Frame_buffer);
	LCD_12864_Draw_pixel(87, 53, Frame_buffer);
	LCD_12864_Draw_pixel(87, 52, Frame_buffer);*/
	LCD_12864_Draw_pixel(88, 51, Frame_buffer);
	LCD_12864_Draw_line(89, 50, 93, 50, Frame_buffer);
	/*LCD_12864_Draw_pixel(89, 50, Frame_buffer);
	LCD_12864_Draw_pixel(90, 50, Frame_buffer);
	LCD_12864_Draw_pixel(91, 50, Frame_buffer);
	LCD_12864_Draw_pixel(92, 50, Frame_buffer);
	LCD_12864_Draw_pixel(93, 50, Frame_buffer);*/
	LCD_12864_Draw_pixel(94, 51, Frame_buffer);
	LCD_12864_Draw_line(92, 46, 92, 49, Frame_buffer);
	/*LCD_12864_Draw_pixel(92, 49, Frame_buffer);
	LCD_12864_Draw_pixel(92, 48, Frame_buffer);
	LCD_12864_Draw_pixel(92, 47, Frame_buffer);
	LCD_12864_Draw_pixel(92, 46, Frame_buffer);*/
	LCD_12864_Draw_pixel(93, 45, Frame_buffer);
	LCD_12864_Draw_line(94, 44, 100, 44, Frame_buffer);
	/*LCD_12864_Draw_pixel(94, 44, Frame_buffer);
	LCD_12864_Draw_pixel(95, 44, Frame_buffer);
	LCD_12864_Draw_pixel(96, 44, Frame_buffer);
	LCD_12864_Draw_pixel(97, 44, Frame_buffer);
	LCD_12864_Draw_pixel(98, 44, Frame_buffer);
	LCD_12864_Draw_pixel(99, 44, Frame_buffer);
	LCD_12864_Draw_pixel(100, 44, Frame_buffer);*/
	LCD_12864_Draw_pixel(101, 45, Frame_buffer);
	LCD_12864_Draw_line(102, 46, 102, 48, Frame_buffer);
	/*LCD_12864_Draw_pixel(102, 46, Frame_buffer);
	LCD_12864_Draw_pixel(102, 47, Frame_buffer);
	LCD_12864_Draw_pixel(102, 48, Frame_buffer);*/
	LCD_12864_Draw_pixel(101, 49, Frame_buffer);
	LCD_12864_Draw_line(102, 48, 106, 48, Frame_buffer);
	/*LCD_12864_Draw_pixel(102, 48, Frame_buffer);
	LCD_12864_Draw_pixel(103, 48, Frame_buffer);
	LCD_12864_Draw_pixel(104, 48, Frame_buffer);
	LCD_12864_Draw_pixel(105, 48, Frame_buffer);
	LCD_12864_Draw_pixel(106, 48, Frame_buffer);*/
	LCD_12864_Draw_pixel(107, 56, Frame_buffer);
	LCD_12864_Draw_pixel(108, 55, Frame_buffer);
	LCD_12864_Draw_line(109, 51, 109, 54, Frame_buffer);
	/*LCD_12864_Draw_pixel(109, 54, Frame_buffer);
	LCD_12864_Draw_pixel(109, 53, Frame_buffer);
	LCD_12864_Draw_pixel(109, 52, Frame_buffer);
	LCD_12864_Draw_pixel(109, 51, Frame_buffer);*/
	LCD_12864_Draw_pixel(108, 50, Frame_buffer);
	LCD_12864_Draw_pixel(107, 49, Frame_buffer);
	//облако№2
	LCD_12864_Draw_line(110, 55, 121, 55, Frame_buffer);
	LCD_12864_Draw_line(122, 51, 122, 54, Frame_buffer);
	/*LCD_12864_Draw_pixel(122, 54, Frame_buffer);
	LCD_12864_Draw_pixel(122, 53, Frame_buffer);
	LCD_12864_Draw_pixel(122, 52, Frame_buffer);
	LCD_12864_Draw_pixel(122, 51, Frame_buffer);*/
	LCD_12864_Draw_pixel(121, 50, Frame_buffer);
	LCD_12864_Draw_line(118, 49, 120, 49, Frame_buffer);
	/*LCD_12864_Draw_pixel(120, 49, Frame_buffer);
	LCD_12864_Draw_pixel(119, 49, Frame_buffer);
	LCD_12864_Draw_pixel(118, 49, Frame_buffer);*/
	LCD_12864_Draw_pixel(117, 50, Frame_buffer);
	LCD_12864_Draw_line(118, 47, 118, 48, Frame_buffer);
	/*LCD_12864_Draw_pixel(118, 48, Frame_buffer);
	LCD_12864_Draw_pixel(118, 47, Frame_buffer);*/
	LCD_12864_Draw_line(114, 46, 117, 46, Frame_buffer);
	/*LCD_12864_Draw_pixel(117, 46, Frame_buffer);
	LCD_12864_Draw_pixel(116, 46, Frame_buffer);
	LCD_12864_Draw_pixel(115, 46, Frame_buffer);
	LCD_12864_Draw_pixel(114, 46, Frame_buffer);*/
	LCD_12864_Draw_line(113, 47, 113, 49, Frame_buffer);
	/*LCD_12864_Draw_pixel(113, 47, Frame_buffer);
	LCD_12864_Draw_pixel(113, 48, Frame_buffer);
	LCD_12864_Draw_pixel(113, 49, Frame_buffer);*/
	LCD_12864_Draw_pixel(114, 50, Frame_buffer);
	LCD_12864_Draw_line(111, 49, 112, 49, Frame_buffer);
	/*LCD_12864_Draw_pixel(112, 49, Frame_buffer);
	LCD_12864_Draw_pixel(111, 49, Frame_buffer);*/
	LCD_12864_Draw_pixel(110, 50, Frame_buffer);
}
void DrawCloudsWithRain(uint8_t *Frame_buffer)
{
	DrawClouds(Frame_buffer);

	LCD_12864_Draw_line(91, 59, 89, 61, Frame_buffer);
	LCD_12864_Draw_line(94, 59, 92, 61, Frame_buffer);
	LCD_12864_Draw_line(97, 59, 95, 61, Frame_buffer);
	LCD_12864_Draw_line(100, 59, 98, 61, Frame_buffer);
	LCD_12864_Draw_line(103, 59, 101, 61, Frame_buffer);
	LCD_12864_Draw_line(106, 59, 104, 61, Frame_buffer);
	
	LCD_12864_Draw_line(111, 57, 110, 58, Frame_buffer);
	LCD_12864_Draw_line(114, 57, 113, 58, Frame_buffer);
	LCD_12864_Draw_line(117, 57, 116, 58, Frame_buffer);
	LCD_12864_Draw_line(120, 57, 119, 58, Frame_buffer);
}
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
void DrawWindDirect()
{
	sprintf(wind_direction,"%s","N");
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
void DrawBattery(uint8_t *Frame_buffer)
{
	LCD_12864_Draw_line(34, 28, 40, 28, Frame_buffer);
	LCD_12864_Draw_line(34, 19, 34, 27, Frame_buffer);
	LCD_12864_Draw_line(40, 19, 40, 27, Frame_buffer);
	LCD_12864_Draw_line(34, 19, 36, 19, Frame_buffer);
	LCD_12864_Draw_line(38, 19, 40, 19, Frame_buffer);
	LCD_12864_Draw_line(36, 18, 36, 19, Frame_buffer);
	LCD_12864_Draw_line(38, 18, 38, 19, Frame_buffer);
	LCD_12864_Draw_line(36, 18, 38, 18, Frame_buffer);
	uint8_t v_count;
	if (V_BAT(adc_value1) >= 400) {
		v_count = 6;
		/*LCD_12864_Draw_line(36, 26, 38, 26, Frame_buffer);
		LCD_12864_Draw_line(36, 25, 38, 25, Frame_buffer);
		LCD_12864_Draw_line(36, 24, 38, 24, Frame_buffer);
		LCD_12864_Draw_line(36, 23, 38, 23, Frame_buffer);
		LCD_12864_Draw_line(36, 22, 38, 22, Frame_buffer);
		LCD_12864_Draw_line(36, 21, 38, 21, Frame_buffer);*/
	}
	else if ((V_BAT(adc_value1) >= 390) && (V_BAT(adc_value1) < 400)) {
		v_count = 5;
		/*LCD_12864_Draw_line(36, 26, 38, 26, Frame_buffer);
		LCD_12864_Draw_line(36, 25, 38, 25, Frame_buffer);
		LCD_12864_Draw_line(36, 24, 38, 24, Frame_buffer);
		LCD_12864_Draw_line(36, 23, 38, 23, Frame_buffer);
		LCD_12864_Draw_line(36, 22, 38, 22, Frame_buffer);*/
	}
	else if ((V_BAT(adc_value1) >= 380) && (V_BAT(adc_value1) < 390)) {
		v_count = 4;
		/*LCD_12864_Draw_line(36, 26, 38, 26, Frame_buffer);
		LCD_12864_Draw_line(36, 25, 38, 25, Frame_buffer);
		LCD_12864_Draw_line(36, 24, 38, 24, Frame_buffer);
		LCD_12864_Draw_line(36, 23, 38, 23, Frame_buffer);*/
	}
	else if ((V_BAT(adc_value1) >= 370) && (V_BAT(adc_value1) < 380)) {
		v_count = 3;
		//LCD_12864_Draw_line(36, 26, 38, 26, Frame_buffer);
		//LCD_12864_Draw_line(36, 25, 38, 25, Frame_buffer);
		//LCD_12864_Draw_line(36, 24, 38, 24, Frame_buffer);
	}
	else if ((V_BAT(adc_value1) >= 360) && (V_BAT(adc_value1) < 370)) {
		v_count = 2;
		//LCD_12864_Draw_line(36, 26, 38, 26, Frame_buffer);
		//LCD_12864_Draw_line(36, 25, 38, 25, Frame_buffer);
	}
	else if ((V_BAT(adc_value1) >= 350) && (V_BAT(adc_value1) < 360)) {
		v_count = 1;
		//LCD_12864_Draw_line(36, 26, 38, 26, Frame_buffer);
	}
	else {v_count = 0;}
	for (uint8_t i = 0; i < v_count; i++)
	{
		LCD_12864_Draw_line(36, 26 - i, 38, 26 - i, Frame_buffer);
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
void sprintf_HOME_Weath_Param(void)
{
	wdt_reset();
	uint8_t data[5] = {0};
	int pressure_home = 0;
	int home_temp, home_hum;

	/*if (dht22_GetData(data))
	{
		//data[1]-младший бит температуры
		//data[2]-старший бит температуры
		//data[3]-младший бит влажности
		//data[4]-старший бит влажности
		home_temp = ((data[2]<<8)|data[1]);
		if ((home_temp & 0x8000) == 0x8000) home_temp = ~(home_temp & 0x7FFF);
		home_hum = ((data[4]<<8)|data[3]) / 10;
	}
	sprintf(temp_home,"%d.%d",home_temp/10, abs(home_temp)%10);
	sprintf(hum_home,"%d",home_hum);
	pressure_home = BMP180_calculation()*0.0075;
	sprintf(Press_home,"%d",pressure_home);*/
	
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
	}
	else
	{
		home_temp_fraction = ((~tt) & 0xF);
		home_temp_fraction = (home_temp_fraction<<1) + (home_temp_fraction<<3);// ”множаем на 10
		home_temp_fraction = (home_temp_fraction>>4);//делим на 16 или умножаем на 0.0625
		home_temp_integer = ((~(tt))&0x07FF)>>4;
		//sprintf(data,"-%d,%d\r",buf[1],buf[0]);
	}
	//измерение влажности
	/*long int hum = 0;
	hum = HTU21D_get_humidity();
	hum = (hum*125)/65536 - 6;
	home_hum_integer = hum;*/
	
	wdt_reset();
}