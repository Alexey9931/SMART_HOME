/*
 * main.h
 *
 * Created: 03.02.2022 17:52:19
 *  Author: Alex2
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define LED_TX				PORTD6
#define LED_RX				PORTD5
#define LED_BOILER_STATUS   PORTD4
#define LED_WIFI			PORTC1
#define MOSFET				PORTB0
#define BUTTON_OK		    PORTD3
#define BUTTON_UP		    PORTC2
#define BUTTON_DOWN		    PORTC3

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NRF24.h"
#include "TWI.h"
#include "DS18B20.h"
#include "MAX7219.h"

#endif /* MAIN_H_ */