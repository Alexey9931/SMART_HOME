/*
 * WeatherStation.h
 *
 * Created: 03.06.2022 19:16:10
 *  Author: Alex2
 */ 


#ifndef WEATHERSTATION_H_
#define WEATHERSTATION_H_

#include "main.h"

void Print_Hello_World();
void Print_Download();
void Print_Home_Page_Out();
void Print_Home_Page_WeatherForecast();
void Print_Static_Home_Page();
void Print_Home_Page_In();
void Print_Boards_Main();
void Print_Menu_Page_Static();
void Print_Menu_Page();
void Print_Page_Clock_Settings_Static();
void Print_Page_Clock_Settings();
void Print_Page_About();
void Print_Page_Dop_Info();
void Print_Page_Dop_Info_Static();
void Clock (void);
void Convert_to_string_Clock(void);
int RAIN_AMOUNT(char *adc_value);
int V_BAT (char *adc_value);
int wind_speed (char *counter);
void DrawWeatherVane();
void DrawLevelWiFi();
void DrawLevelNrf();
void DrawWindDirect();
void sprintf_Home_Weath_Param(void);


#endif /* WEATHERSTATION_H_ */