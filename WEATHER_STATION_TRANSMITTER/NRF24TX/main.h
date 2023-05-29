/*
 * main.h
 *
 * Created: 03.02.2022 17:52:19
 *  Author: Alex2
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define GERKON1	 PORTB0
#define GERKON2  PORTD0
#define GERKON3  PORTD1
#define GERKON4  PORTD4
#define GERKON5  PORTB6
#define GERKON6  PORTB7
#define GERKON7  PORTC2
#define GERKON8  PORTC3

#define TRANZISTOR  PORTD5
#define LED         PORTD6

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NRF24.h"
#include "usart.h"
#include "adc.h"
//#include "DHT22.h"
#include "TWI.h"
#include "DS18B20.h"
#include "HTU21D.h"

#endif /* MAIN_H_ */