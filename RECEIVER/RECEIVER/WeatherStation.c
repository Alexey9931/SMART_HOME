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

extern uint8_t clock_change_flag;
extern int8_t up_down_count;
extern int8_t add_cnt;
extern uint8_t receive_counter;
extern uint8_t WiFi_Flag;
extern int8_t clock_setting_count;


uint8_t home_temp_sign;//���� ����� ����
uint8_t home_temp_integer;//����� ����� ����� ����
uint8_t home_temp_fraction;//������� ����� ����� ����
int home_hum_integer;//����� ����� ����� �����
int pressure_home;//����� ����� ��������

extern uint8_t street_temp_sign;//���� ����� ����
extern uint8_t street_temp_integer;//����� ����� ����� ����
extern uint8_t street_temp_fraction;//������� ����� ����� ����
extern int street_hum_integer;//����� ����� ����� �����
extern uint8_t wind_speed_integer;//����� ����� �� �����
extern uint8_t wind_speed_fraction;//������� ����� �� �����
extern uint8_t rain; //����� ����� �������

//���� ���������� �� ������ �������
void Print_Hello_World()
{	
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,80);
	ILI9486_Print_String32x32("��� ������������",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(20,150);
	ILI9486_Print_String32x32("������� ����� ���!",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	//ILI9486_SetCursor(100,220);
	//ILI9486_Print_String32x24("is with you now!",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
}
//���� ��������
void Print_Download()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(120,50);
	ILI9486_Print_String32x32("����������",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(120,90);
	ILI9486_Print_String32x32("���������,",TFT_STRING_MODE_NO_BACKGROUND);
	wdt_reset();
	ILI9486_SetCursor(80,130);
	ILI9486_Print_String32x32("��� ��������...",TFT_STRING_MODE_NO_BACKGROUND);
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
	//������� ��� ������� ������
	ILI9486_SetTextColor(0xB7F9,BLACK);
	ILI9486_SetCursor(160,40);
	ILI9486_Print_StringConsolas16x24("WiFi",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(223,40);
	ILI9486_Print_StringConsolas16x24("Ch2",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(275,40);
	ILI9486_Print_StringConsolas16x24("Ch1",TFT_STRING_MODE_BACKGROUND);
	//����� ������
	Print_Boards_Main();
	//����� ������� �������
	DrawWeatherVane();
	//����� ���� ������
	ILI9486_Draw_Image("/wsp.txt", 30, 30, 170, 255);//�� �����
	ILI9486_Draw_Image("/pres.txt", 30, 30, 172, 285);//��������
	ILI9486_Draw_Image("/rain.txt", 30, 30, 330, 90);//������
	
	//����� ������ ������� WiFi (���� ���� ��������)
	//DrawLevelWiFi();
	ILI9486_Draw_Image("/wi4.txt", 40, 40, 165, 2);
	//����� ������� ������ ������� NRF (� ����������� ��� �����)-���� ���� ����
	ILI9486_Draw_Image("/nrf0.txt", 30, 30, 229, 10);
	
	//����� ������� ��� �������� �����
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
//������� ���� (������� ���������)
void Print_Home_Page_Out()
{
	char data[10];
	//����� �������� �����
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
	memset(data, 0, sizeof(char) * strlen(data));//������� �������
	sprintf(data,"%d",wind_speed_fraction);
	ILI9486_Print_StringConsolas16x24(data,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(270,260);
	ILI9486_Print_StringConsolas16x24("m/",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(294,260);
	ILI9486_Print_StringConsolas16x24("s",TFT_STRING_MODE_BACKGROUND);
	memset(data, 0, sizeof(char) * strlen(data));//������� �������
	//����� ������� ���������
	ILI9486_SetTextColor(YELLOW,BLACK);
	if (street_temp_sign != 0x00)
	{
		ILI9486_SetCursor(395,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//�����
		if(street_temp_integer < 10)
		{
			sprintf(data," %d",street_temp_integer);
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
			ILI9486_SetCursor(346,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//���� -
		}
		else
		{
			sprintf(data,"%d",street_temp_integer);
			ILI9486_SetCursor(317,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//���� -
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		}
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
		sprintf(data,"%d",street_temp_fraction);
		ILI9486_SetCursor(424,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//������� �����
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
	}
	else
	{
		ILI9486_SetCursor(395,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//�����
		if(street_temp_integer < 10)
		{
			sprintf(data," %d",street_temp_integer);
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		}
		else
		{
			sprintf(data,"%d",street_temp_integer);
			ILI9486_SetCursor(334,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		}
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
		sprintf(data,"%d",street_temp_fraction);
		ILI9486_SetCursor(424,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
	}
	ILI9486_SetCursor(457,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_BACKGROUND);//�� ���������
	//���������
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
	memset(data, 0, sizeof(char) * strlen(data));//������� �������
	ILI9486_SetCursor(429,165);
	ILI9486_Print_String18x32("%",TFT_STRING_MODE_BACKGROUND);//�� ���������
	//����� �������
	DrawWindDirect();
	//����� ���-�� �������
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
	memset(data, 0, sizeof(char) * strlen(data));//������� �������
	//����� ������ ������� NRF
	DrawLevelNrf();
}
//������� ������ �� ������� ����
void Print_Home_Page_WeatherForecast()
{
	//����� �������� ������
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
//������� ����(�������� ���������)
void Print_Home_Page_In()
{
	char data[10];
	char TIME[10] = {0};
	char DATE[10] = {0};
	//����� �������/����
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
	//����� �������� ���������
	//���������
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
	memset(data, 0, sizeof(char) * strlen(data));//������� �������
	ILI9486_Print_String18x32("%",TFT_STRING_MODE_BACKGROUND);//�� ���������
	//�����������
	if (home_temp_sign != 0x00)
	{
		ILI9486_SetCursor(79,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//�����
		if(home_temp_integer < 10)
		{
			sprintf(data," %d",home_temp_integer);
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
			ILI9486_SetCursor(3,255);
			ILI9486_Print_String18x32(" ",TFT_STRING_MODE_BACKGROUND);
			ILI9486_SetCursor(30,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//���� -
		}
		else
		{
			sprintf(data,"%d",home_temp_integer);
			ILI9486_SetCursor(3,255);
			ILI9486_Print_String18x32("-",TFT_STRING_MODE_BACKGROUND);//���� -
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		}
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(110,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//������� �����
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
	}
	else
	{
		ILI9486_SetCursor(79,250);
		ILI9486_Print_String32x48(".",TFT_STRING_MODE_BACKGROUND);//�����
		if(home_temp_integer < 10)
		{
			sprintf(data," %d",home_temp_integer);
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		}
		else
		{
			sprintf(data,"%d",home_temp_integer);
			ILI9486_SetCursor(19,250);
			ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		}
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
		sprintf(data,"%d",home_temp_fraction);
		ILI9486_SetCursor(110,250);
		ILI9486_Print_String32x48(data,TFT_STRING_MODE_BACKGROUND);//����� �����
		memset(data, 0, sizeof(char) * strlen(data));//������� �������
	}
	ILI9486_SetCursor(143,268);
	ILI9486_Print_String18x32("C",TFT_STRING_MODE_BACKGROUND);//�� ���������
	//����� ���. ��������
	ILI9486_SetTextColor(0xF96B,BLACK);
	ILI9486_SetCursor(206,290);
	sprintf(data,"%d",pressure_home);
	ILI9486_Print_StringConsolas16x24(data,TFT_STRING_MODE_BACKGROUND);
	memset(data, 0, sizeof(char) * strlen(data));//������� �������
	ILI9486_SetCursor(248,290);
	ILI9486_Print_StringConsolas16x24("mmHg",TFT_STRING_MODE_BACKGROUND);
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
	/*ILI9486_SetTextColor(YELLOW, BLACK);
	ILI9486_SetCursor(349, 208);
	ILI9486_Print_String14x24("OUTDOOR",TFT_STRING_MODE_NO_BACKGROUND);
	ILI9486_SetTextColor(ORANGE, BLACK);
	ILI9486_SetCursor(40, 208);
	ILI9486_Print_String14x24("INDOOR",TFT_STRING_MODE_NO_BACKGROUND);*/
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
	
	ILI9486_SetTextColor(YELLOW, BLACK);
	ILI9486_SetCursor(349, 208);
	ILI9486_Print_String14x24("OUTDOOR",TFT_STRING_MODE_NO_BACKGROUND);
	ILI9486_SetTextColor(0xFB80, BLACK);
	ILI9486_SetCursor(40, 208);
	ILI9486_Print_String14x24("INDOOR",TFT_STRING_MODE_NO_BACKGROUND);
}
//���� ���� ������
void Print_Menu_Page_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(40,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("������� ����",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,70);
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_Print_String40x40("������� ����",TFT_STRING_MODE_BACKGROUND);							   
	ILI9486_SetCursor(40,110);
	ILI9486_Print_String40x40("���� � �����",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,150);
	ILI9486_Print_String40x40(" � �������",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,190);
	ILI9486_Print_String40x40("��� ��������",TFT_STRING_MODE_BACKGROUND);
}
//���� ���� �������
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
//���� �������� �������
void Print_Page_Clock_Settings_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(40,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("���� � �����",TFT_STRING_MODE_BACKGROUND);
	Clock();
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(112,120);
	ILI9486_Print_String32x32("�",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(232,120);
	ILI9486_Print_String32x32("���",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(377,120);
	ILI9486_Print_String32x32("���",TFT_STRING_MODE_BACKGROUND);
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
	ILI9486_Print_String40x40("����� � ����",TFT_STRING_MODE_BACKGROUND);
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
//���� �������
void Print_Page_About()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(80,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("� �������",TFT_STRING_MODE_BACKGROUND);
	
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("����� � ����",TFT_STRING_MODE_BACKGROUND);
	ILI9486_DrawRect(37,273,414,42,WHITE);
	ILI9486_DrawRect(38,274,412,40,WHITE);
	/*uint8_t Frame_buffer[1024] = { 0 }; //����� �����
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
	LCD_12864_GrapnicMode(0);*/
}
//���� ��� ����
void Print_Page_Dop_Info_Static()
{
	ILI9486_FillScreen(BLACK);
	ILI9486_SetRotation(1);
	ILI9486_SetCursor(40,20);
	ILI9486_SetTextColor(BLACK,WHITE);
	ILI9486_Print_String40x40("��� ��������",TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(150,70);
	ILI9486_Print_String18x32("Start time:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(90,100);
	ILI9486_Print_String18x32(start_time,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,130);
	ILI9486_Print_String18x32("Outdoor transmitter:",TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(40,190);
	ILI9486_Print_String18x32("Time of sending to DB:",TFT_STRING_MODE_BACKGROUND);
	
	ILI9486_SetTextColor(WHITE,BLACK);
	ILI9486_SetCursor(40,270);
	ILI9486_Print_String40x40("����� � ����",TFT_STRING_MODE_BACKGROUND);
	ILI9486_DrawRect(37,273,414,42,WHITE);
	ILI9486_DrawRect(38,274,412,40,WHITE);
}
void Print_Page_Dop_Info()
{
	ILI9486_SetCursor(50,160);
	ILI9486_Print_String18x32(receive_time,TFT_STRING_MODE_BACKGROUND);
	ILI9486_SetCursor(50,220);
	ILI9486_Print_String18x32(send_time,TFT_STRING_MODE_BACKGROUND);
}
//-----------������� ��������������� ��� �������� ����------//
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
	FATFS fs;
	WORD s1;
	asm("nop");
	pf_mount(&fs); //��������� FAT
	pf_open("/wi4.txt");

	uint8_t array1[1610];
	int l, p = 0;
	for (int k = 0; k < 10; k++)
	{
		memset(array1, 0, sizeof(uint8_t) * strlen(array1));//������� �������
		char arr[10] = {};
		p = 0;
		pf_read(array1,960,&s1);
		wdt_reset();
		for (int m = 0; m < 160; m++)
		{
			l = 0;
			while (array1[p] != ',')
			{
				arr[l] = array1[p];
				p++;
				l++;
			}
			p += 2;
			ILI9486_drawPixel(160+m%40, 5+4*k+(m/40), (uint16_t)strtol(arr,NULL,16));
			wdt_reset();
		}
	}
	pf_mount(0x00);
}
void DrawLevelNrf()
{
	switch (receive_counter)
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
	//��������� �����������
	int tt = 0; 
	tt = dt_check(); 
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
	//��������� ���������
	/*long int hum = 0;
	hum = HTU21D_get_humidity();
	hum = (hum*125)/65536 - 6;
	home_hum_integer = hum;*/
	//��������� ������������ ��������
	pressure_home = BMP180_calculation()*0.0075;
	
	wdt_reset();
}