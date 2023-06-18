/*
 * main.h
 *
 * Created: 03.02.2022 17:52:19
 *  Author: Alex2
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <avr/wdt.h>

#define ButtonOK PORTE4
#define ButtonHome PORTE5
#define ButtonUp PORTE6
#define ButtonDown PORTE7
#define LED PORTL7
#define DHT22_PORT PORTK1
//#define IK_RX PORTD3
#define BUZZER PORTL6
//#define	SW_ENC PORTD3
//#define DT_ENC PORTD4
//#define CLK_ENC PORTD5

#include "NRF24.h"
#include "TWI.h"
#include "usart.h"
#include "WeatherStation.h"
#include "RTC_DS3231.h"
#include "DHT22.h"
#include "BMP180.h"
#include "ILI9486.h"
#include "DS18B20.h"
#include "SD.h"
#include "pff.h"
#include "diskio.h"
#include "integer.h"
#include "HTU21D.h"

uint8_t spi_send_recv(uint8_t data);

#endif /* MAIN_H_ */