/*
 * WeatherStation.c
 *
 * Created: 03.06.2022 19:15:57
 *  Author: Alex2
 */ 
#include "WeatherStation.h"

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
	char weakday[4];
	char mounthday[4];
	char Mounth[4];
	char Year[4];
} T_Param;
//uint8_t Frame_buffer[1024] = { 0 }; //����� �����
extern unsigned char sec,min,hour,day,date,month,year,alarmhour,alarmmin;
extern char receive_time[20];
extern char send_time[20];
extern char start_time[20];
extern char adc_value1[6];
extern char HALL_counter[5];
extern char adc_value2[6];

extern uint8_t change_flag;
extern int8_t cnt;
extern int8_t add_cnt;
extern uint8_t receive_counter;
extern uint8_t WiFi_Flag;


uint8_t home_temp_sign;//���� ����� ����
uint8_t home_temp_integer;//����� ����� ����� ����
uint8_t home_temp_fraction;//������� ����� ����� ����

extern uint8_t street_temp_sign;//���� ����� ����
extern uint8_t street_temp_integer;//����� ����� ����� ����
extern uint8_t street_temp_fraction;//������� ����� ����� ����

//���� ���������� �� ������ �������
void Print_Hello_World()
{	
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(110,80);
	ILI9486_Print_String18x32("You are welcome!",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(50,150);
	ILI9486_Print_String18x32("The Smart Home System",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(100,220);
	ILI9486_Print_String18x32("is with you now!",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
}
//���� ��������
void Print_Download()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(110,80);
	ILI9486_Print_String18x32("Please wait, the",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(10,120);
	ILI9486_Print_String18x32("download is in progress...",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_DrawRect(50, 200,  383, 50,  WHITE);
	for(uint16_t i = 54; i < 429 ; i++)
	{
		ILI9486_Draw_Line(i, 204, i, 245, WHITE);
		_delay_ms(20);
		wdt_reset();
	}
}
//����������� ������� ����
void Print_Static_Home_Page()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	//����� ������
	Print_Boards_Main();
}
//������� ����
void Print_Home_Page()
{
	char data[10];
	//����� �������
	DrawWeatherVane();
	//����� �������/����
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(10,20);
	//ILI9486_Print_String32x32("12:34",TFT_STRING_MODE_NO_BACKGROUND);
	//����� �������� ���������
	ILI9486_SetTextColor(ORANGE,BLACK);
	if (home_temp_sign != 0x00) 
	{	
		sprintf(data,"%d",home_temp_integer);
		if(home_temp_integer < 10)
		{
			ILI9486_SetCursor(35,250);
			ILI9486_Print_String32x48("-",TFT_STRING_MODE_NO_BACKGROUND);//���� -
			ILI9486_SetCursor(65,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		else
		{
			ILI9486_SetCursor(3,250);
			ILI9486_Print_String32x48("-",TFT_STRING_MODE_NO_BACKGROUND);//���� -
			ILI9486_SetCursor(30,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		ILI9486_SetCursor(87,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_NO_BACKGROUND);//�����
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(110,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//������� �����
	}
	else
	{
		sprintf(data,"%d",home_temp_integer);
		if(home_temp_integer < 10)
		{
			ILI9486_SetCursor(65,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		else
		{
			ILI9486_SetCursor(30,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		ILI9486_SetCursor(87,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_NO_BACKGROUND);//�����
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(110,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
	}
	ILI9486_SetCursor(143,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_NO_BACKGROUND);//�� ���������
	
	//����� ������� ���������
	ILI9486_SetTextColor(YELLOW,BLACK);
	if (home_temp_sign != 0x00)
	{
		sprintf(data,"%d",home_temp_integer);
		if(home_temp_integer < 10)
		{
			ILI9486_SetCursor(349,250);
			ILI9486_Print_String32x48("-",TFT_STRING_MODE_NO_BACKGROUND);//���� -
			ILI9486_SetCursor(379,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		else
		{
			ILI9486_SetCursor(317,250);
			ILI9486_Print_String32x48("-",TFT_STRING_MODE_NO_BACKGROUND);//���� -
			ILI9486_SetCursor(344,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		ILI9486_SetCursor(401,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_NO_BACKGROUND);//�����
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(424,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//������� �����
	}
	else
	{
		sprintf(data,"%d",home_temp_integer);
		if(home_temp_integer < 10)
		{
			ILI9486_SetCursor(379,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		else
		{
			ILI9486_SetCursor(344,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
		}
		ILI9486_SetCursor(401,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_NO_BACKGROUND);//�����
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(424,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_NO_BACKGROUND);//����� �����
	}
	ILI9486_SetCursor(457,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_NO_BACKGROUND);//�� ���������

	/*uint8_t Frame_buffer[1024] = { 0 }; //����� �����
	char TIME[10] = {0};
	
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	//-----------����� �������/����-------------------------------//
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
	//-----------����� ���������� WiFi-------------------------------//
	if (WiFi_Flag == 1)
	{
		LCD_12864_Decode_UTF8(100, 0, 0, "Wi-Fi", Frame_buffer);
	}
	//-----------����� ������ ������ �������----------------------//
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
	//-----------����� ������� �����������-----------------------//
	LCD_12864_Decode_UTF8(30, 4, 0, "�C", Frame_buffer);
	LCD_12864_Decode_UTF8(57, 4, 0, "%", Frame_buffer);
	LCD_12864_Print_5_8(46, 4, 0, hum_street, Frame_buffer);
	if (strchr(temp_street,'-') != 0) {LCD_12864_Print_5_8(0, 4, 0, temp_street, Frame_buffer);}
	else {LCD_12864_Print_5_8(7, 4, 0, temp_street, Frame_buffer);}
	LCD_12864_Print_4_6(44, 2, 0, Vbat, Frame_buffer);
	LCD_12864_Print_4_6(26, 5, 0, WIND_speed, Frame_buffer);
	LCD_12864_Decode_UTF8(45, 5, 0, "�/�", Frame_buffer);
	LCD_12864_Decode_UTF8(30, 6, 0, "������", Frame_buffer);
	LCD_12864_Draw_line(27, 52, 24, 52, Frame_buffer);
	LCD_12864_Draw_line(24, 52, 24, 59, Frame_buffer);
	LCD_12864_Draw_line(24, 59, 28, 59, Frame_buffer);
	LCD_12864_Draw_pixel(27, 58, Frame_buffer);
	LCD_12864_Draw_pixel(27, 60, Frame_buffer);
	LCD_12864_Draw_pixel(26, 57, Frame_buffer);
	LCD_12864_Draw_pixel(26, 61, Frame_buffer);
	if (RAIN_AMOUNT(adc_value2) <= 10)  {LCD_12864_Decode_UTF8(30, 7, 0, "��������.", Frame_buffer);}
	else if ((RAIN_AMOUNT(adc_value2) > 10) && (RAIN_AMOUNT(adc_value2) <= 50))  {LCD_12864_Decode_UTF8(30, 7, 0, "������", Frame_buffer);}
    else if ((RAIN_AMOUNT(adc_value2) > 50) && (RAIN_AMOUNT(adc_value2) <= 75)) { LCD_12864_Decode_UTF8(30, 7, 0, "���������", Frame_buffer);}
	else if ((RAIN_AMOUNT(adc_value2) > 75) && (RAIN_AMOUNT(adc_value2) <= 100))  {LCD_12864_Decode_UTF8(30, 7, 0, "�������", Frame_buffer);}
	else {LCD_12864_Decode_UTF8(30, 7, 0, "��� ������", Frame_buffer);}
	//-----------����� ����������� � ����-----------------------//
	LCD_12864_Decode_UTF8(95, 4, 0, "�C", Frame_buffer);
	if (strchr(temp_home,'-') != 0) {LCD_12864_Print_5_8(65, 4, 0, temp_home, Frame_buffer);}
	else {LCD_12864_Print_5_8(72, 4, 0, temp_home, Frame_buffer);}
	LCD_12864_Decode_UTF8(122, 4, 0, "%", Frame_buffer);
	LCD_12864_Print_5_8(111, 4, 0, hum_home, Frame_buffer);
	LCD_12864_Print_4_6(86, 2, 0, Press_home, Frame_buffer);
	LCD_12864_Decode_UTF8(101, 2, 0, "��", Frame_buffer);
	LCD_12864_Decode_UTF8(114, 2, 0, "Hg", Frame_buffer);
	
	//-----------����� �������������� �����-----------------------//
	LCD_12864_Decode_UTF8(2, 2, 0, "�����", Frame_buffer);
	LCD_12864_Decode_UTF8(65, 2, 0, "���", Frame_buffer);
	LCD_12864_Draw_line(0, 16, 128, 16, Frame_buffer);
	LCD_12864_Draw_line(0, 24, 32, 24, Frame_buffer);
	LCD_12864_Draw_line(32, 17, 32, 24, Frame_buffer);
	LCD_12864_Draw_line(0, 17, 0, 24, Frame_buffer);
	LCD_12864_Draw_line(64, 24, 83, 24, Frame_buffer);
	LCD_12864_Draw_line(83, 17, 83, 24, Frame_buffer);
	LCD_12864_Draw_line(63, 17, 63, 41, Frame_buffer);
	LCD_12864_Draw_line(63, 41, 128, 41, Frame_buffer);
	//-----------����� ���������� ���-----------------------//
	DrawBattery(Frame_buffer);
	//-----------����� ���������� �������-----------------------//
	DrawWeatherVane(Frame_buffer);
	//-----------����� �������� ������-----------------------//
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
//������������ ������������ ��������� �������� ����
void Update_Home_Page()
{
	ILI9486_fillRect(3, 250, 138, 60, BLACK);//����� ����� ����
	ILI9486_fillRect(317, 250, 138, 60, BLACK);//����� ����� ����
}
//�������� �������� ��� �������� ����
void Print_Boards_Main()
{
	//��������� ������ ������
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
	
	//��������� ������ ����� ��� indoor � outdoor
	//������ ��������
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
	ILI9486_SetTextColor(YELLOW, BLACK);
	ILI9486_SetCursor(349, 208);
	ILI9486_Print_String14x24("OUTDOOR",TFT_STRING_MODE_NO_BACKGROUND);
	ILI9486_SetTextColor(ORANGE, BLACK);
	ILI9486_SetCursor(40, 208);
	ILI9486_Print_String14x24("INDOOR",TFT_STRING_MODE_NO_BACKGROUND);
	//����� ��������
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
	
	
	//��������� ������� �����
	ILI9486_Draw_Line(1, 120, 478, 120, WHITE);
	ILI9486_Draw_Line(1, 124, 149, 124, WHITE);
	ILI9486_Draw_Line(330, 124, 478, 124, WHITE);
	
	ILI9486_Draw_Line(145, 1, 161, 100, WHITE);
	ILI9486_Draw_Line(332, 1, 316, 100, WHITE);
	ILI9486_Draw_Line(149, 1, 165, 96, WHITE);
	ILI9486_Draw_Line(328, 1, 312, 96, WHITE);
	
	//��������� ����� ����� ��� �����
	ILI9486_Draw_Circle(239, 159, 97,WHITE);
	ILI9486_Draw_Line(152, 120, 327, 120, BLACK);
	ILI9486_Draw_Line(161, 216, 317, 216, BLACK);
	
	//������� ������ ��������
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
}
//���� ����
void Print_Menu_Page()
{
	uint8_t Frame_buffer[1024] = { 0 }; //����� �����
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	LCD_12864_Decode_UTF8(30, 0, 1, "������� ����", Frame_buffer);
	switch (cnt%4)
	{
		case 0: LCD_12864_Decode_UTF8(15, 2, 1, "������� ����-����", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 3, 0, "��������� �������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 4, 0, "�������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 5, 0, "���.����", Frame_buffer);
				break;
		case 1: LCD_12864_Decode_UTF8(15, 2, 0, "������� ����-����", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 3, 1, "��������� �������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 4, 0, "�������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 5, 0, "���.����", Frame_buffer);
				break;
		case 2: LCD_12864_Decode_UTF8(15, 2, 0, "������� ����-����", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 3, 0, "��������� �������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 4, 1, "�������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 5, 0, "���.����", Frame_buffer);
				break;
		case 3: LCD_12864_Decode_UTF8(15, 2, 0, "������� ����-����", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 3, 0, "��������� �������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 4, 0, "�������", Frame_buffer);
				LCD_12864_Decode_UTF8(15, 5, 1, "���.����", Frame_buffer);
				break;
	}
	
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);
}
//���� �������� �������
void Print_Page_Clock_Settings()
{
	uint8_t Frame_buffer[1024] = { 0 }; //����� �����
	Convert_to_string_Clock();
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	
	LCD_12864_Decode_UTF8(15, 0, 1, "��������� �������", Frame_buffer);
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
				LCD_12864_Decode_UTF8(25, 7, 0, "����� � ����", Frame_buffer);
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
				LCD_12864_Decode_UTF8(25, 7, 0, "����� � ����", Frame_buffer);
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
				LCD_12864_Decode_UTF8(25, 7, 0, "����� � ����", Frame_buffer);
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
				LCD_12864_Decode_UTF8(25, 7, 0, "����� � ����", Frame_buffer);
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
				LCD_12864_Decode_UTF8(25, 7, 0, "����� � ����", Frame_buffer);
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
				LCD_12864_Decode_UTF8(25, 7, 0, "����� � ����", Frame_buffer);
				break;
		case 6:
				LCD_12864_Print_11_16(32, 2, 0, T_Param.hours, Frame_buffer);
				LCD_12864_Print_11_16(62, 2, 0, T_Param.minutes, Frame_buffer);
				LCD_12864_Decode_UTF8(28, 5, 0, T_Param.weakday, Frame_buffer);
				LCD_12864_Decode_UTF8(49, 5, 0, T_Param.mounthday, Frame_buffer);
				LCD_12864_Decode_UTF8(70, 5, 0, T_Param.Mounth, Frame_buffer);
				LCD_12864_Decode_UTF8(95, 5, 0, T_Param.Year, Frame_buffer);
				LCD_12864_Decode_UTF8(25, 7, 1, "����� � ����", Frame_buffer);
				break;
	}
	LCD_12864_Decode_UTF8(55, 3, 0, "�", Frame_buffer);
	LCD_12864_Decode_UTF8(85, 3, 0, "���", Frame_buffer);
	LCD_12864_Decode_UTF8(42, 5, 0, ",", Frame_buffer);
	LCD_12864_Decode_UTF8(63, 5, 0, "/", Frame_buffer);
	LCD_12864_Decode_UTF8(89, 5, 0, "/", Frame_buffer);

	
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);
}
//���� �������
void Print_Page_About()
{
	uint8_t Frame_buffer[1024] = { 0 }; //����� �����
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	
	LCD_12864_Decode_UTF8(25, 0, 1, "�� ����������", Frame_buffer);
	LCD_12864_Decode_UTF8(2, 1, 0, "\"�������� ���������-", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 2, 0, "��� (v.1.0)\"-������-", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 3, 0, "������� ��� ����� � ", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 4, 0, "��������� ���������� ", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 5, 0, "� ����� �������� �� ", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 6, 0, "����� � � ���������.", Frame_buffer);
	LCD_12864_Decode_UTF8(25, 7, 1, "����� � ����", Frame_buffer);
	
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);
}
//���� ��� ����
void Print_Page_Dop_Info()
{
	uint8_t Frame_buffer[1024] = { 0 }; //����� �����
	LCD_12864_GrapnicMode(1);
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
	
	LCD_12864_Decode_UTF8(7, 0, 1, "�������������� ����", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 1, 0, "����� �������:", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 2, 0, start_time, Frame_buffer);
	//LCD_12864_Decode_UTF8(0, 3, 0, "������� ����������:", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 3, 0, "����� ����.������:", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 4, 0, receive_time, Frame_buffer);
	LCD_12864_Decode_UTF8(0, 5, 0, "����� �������� � ��:", Frame_buffer);
	LCD_12864_Decode_UTF8(0, 6, 0, send_time, Frame_buffer);
	
	LCD_12864_Decode_UTF8(25, 7, 1, "����� � ����", Frame_buffer);
	
	LCD_12864_Draw_bitmap(Frame_buffer);
	LCD_12864_GrapnicMode(0);
}
//-----------������� ��������������� ��� �������� ����------//
void DrawSun(uint8_t *Frame_buffer)
{
	LCD_12864_Draw_circle_filled(110, 53, 5, Frame_buffer);
	LCD_12864_Draw_line(110, 45, 110, 43, Frame_buffer);
	LCD_12864_Draw_line(118, 53, 120, 53, Frame_buffer);
	LCD_12864_Draw_line(102, 53, 100, 53, Frame_buffer);
	LCD_12864_Draw_line(110, 61, 110, 63, Frame_buffer);
	LCD_12864_Draw_line(104, 48, 102, 46, Frame_buffer);
	LCD_12864_Draw_line(116, 48, 118, 46, Frame_buffer);
	LCD_12864_Draw_line(116, 58, 118, 60, Frame_buffer);
	LCD_12864_Draw_line(104, 58, 102, 60, Frame_buffer);
}
void DrawSunWithClouds(uint8_t *Frame_buffer)
{
	//������
	LCD_12864_Draw_circle(110, 53, 5, Frame_buffer);
	LCD_12864_Draw_line(110, 45, 110, 43, Frame_buffer);
	LCD_12864_Draw_line(118, 53, 120, 53, Frame_buffer);
	LCD_12864_Draw_line(110, 61, 110, 63, Frame_buffer);
	LCD_12864_Draw_line(116, 48, 118, 46, Frame_buffer);
	LCD_12864_Draw_line(116, 58, 118, 60, Frame_buffer);
	//������
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
	//������� ����� ������
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
	//�������� ������
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
	//������1
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
	//������2
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

	ILI9486_fillTriangle(239, 99, 233, 124, 239, 115, GREEN);
	ILI9486_fillTriangle(239, 99, 239, 115, 245, 124, GREEN);
	
	
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
	//������ ����� (��� DS3231) - �� ���� ������� RTC_read_time
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010000);			// �������� ������ ����������, ����� ������
	I2C_SendByte(0x00);				    // �������� ������ ������
	I2C_StopCondition();                // ��������� i2c
	
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(0b11010001);			// �������� ������ ����������, ����� ������
	sec = RTC_ConvertFromDec(I2C_ReadByte());     // ������ ������, ACK
	min = RTC_ConvertFromDec(I2C_ReadByte());     // ������ �����, ACK
	hour = RTC_ConvertFromDec(I2C_ReadByte());    // ������ �����, ACK
	day = RTC_ConvertFromDec(I2C_ReadByte());     // ������ ���� ������, ACK
	date = RTC_ConvertFromDec(I2C_ReadByte());    // ������ �����, ACK
	month = RTC_ConvertFromDec(I2C_ReadByte());   // ������ �����, ACK
	year = RTC_ConvertFromDec(I2C_ReadLastByte());// ������ ���, NACK
	I2C_StopCondition();                // ��������� i2c
	
	Convert_to_string_Clock();
}
void Convert_to_string_Clock(void)
{
	memset(T_Param.seconds, 0, sizeof(char) * strlen(T_Param.seconds));//������� �������
	memset(T_Param.minutes, 0, sizeof(char) * strlen(T_Param.minutes));//������� �������
	memset(T_Param.hours, 0, sizeof(char) * strlen(T_Param.hours));//������� �������
	memset(T_Param.mounthday, 0, sizeof(char) * strlen(T_Param.mounthday));//������� �������
	memset(T_Param.Year, 0, sizeof(char) * strlen(T_Param.Year));//������� �������
	memset(T_Param.weakday, 0, sizeof(char) * strlen(T_Param.weakday));//������� �������
	memset(T_Param.Mounth, 0, sizeof(char) * strlen(T_Param.Mounth));//������� �������
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
		sprintf(T_Param.weakday,"��");
		break;
		case 2:
		sprintf(T_Param.weakday,"��");
		break;
		case 3:
		sprintf(T_Param.weakday,"��");
		break;
		case 4:
		sprintf(T_Param.weakday,"��");
		break;
		case 5:
		sprintf(T_Param.weakday,"��");
		break;
		case 6:
		sprintf(T_Param.weakday,"��");
		break;
		case 7:
		sprintf(T_Param.weakday,"��");
		break;
		default:
		break;
	}
	switch (month)
	{
		case 1:
		sprintf(T_Param.Mounth,"���");
		break;
		case 2:
		sprintf(T_Param.Mounth,"���");
		break;
		case 3:
		sprintf(T_Param.Mounth,"���");
		break;
		case 4:
		sprintf(T_Param.Mounth,"���");
		break;
		case 5:
		sprintf(T_Param.Mounth,"���");
		break;
		case 6:
		sprintf(T_Param.Mounth,"���");
		break;
		case 7:
		sprintf(T_Param.Mounth,"���");
		break;
		case 8:
		sprintf(T_Param.Mounth,"���");
		break;
		case 9:
		sprintf(T_Param.Mounth,"���");
		break;
		case 10:
		sprintf(T_Param.Mounth,"���");
		break;
		case 11:
		sprintf(T_Param.Mounth,"���");
		break;
		case 12:
		sprintf(T_Param.Mounth,"���");
		break;
		default:
		break;
	}
}
//���������� ���-�� �������
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
//���������� ������ ������ ���
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
//���������� �������� �����
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
	speed = 2*314*RADIUS*n/4; //������ 2 ���+2 �������
	
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
		//data[1]-������� ��� �����������
		//data[2]-������� ��� �����������
		//data[3]-������� ��� ���������
		//data[4]-������� ��� ���������
		home_temp = ((data[2]<<8)|data[1]);
		if ((home_temp & 0x8000) == 0x8000) home_temp = ~(home_temp & 0x7FFF);
		home_hum = ((data[4]<<8)|data[3]) / 10;
	}
	sprintf(temp_home,"%d.%d",home_temp/10, abs(home_temp)%10);
	sprintf(hum_home,"%d",home_hum);
	pressure_home = BMP180_calculation()*0.0075;
	sprintf(Press_home,"%d",pressure_home);*/
	
	int tt=0; //��������� �� ������� ����������� � ����� ����
	tt = dt_check(); //������� �����������

	home_temp_sign = tt>>11;//���������� ����� �����������
	if (home_temp_sign == 0x00)
	{
		home_temp_fraction = tt & 0xF;
		home_temp_fraction = (home_temp_fraction<<1) + (home_temp_fraction<<3);// �������� �� 10
		home_temp_fraction = (home_temp_fraction>>4);//����� �� 16 ��� �������� �� 0.0625
		home_temp_integer = (tt&0x07FF)>>4;
		//sprintf(data,"%d,%d\r",buf[1],buf[0]);
	}
	else
	{
		home_temp_fraction = ((~tt) & 0xF);
		home_temp_fraction = (home_temp_fraction<<1) + (home_temp_fraction<<3);// �������� �� 10
		home_temp_fraction = (home_temp_fraction>>4);//����� �� 16 ��� �������� �� 0.0625
		home_temp_integer = ((~(tt))&0x07FF)>>4;
		//sprintf(data,"-%d,%d\r",buf[1],buf[0]);
	}
	wdt_reset();
}