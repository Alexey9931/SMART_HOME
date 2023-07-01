/*
 * DHT22.h
 *
 * Created: 20.03.2022 15:21:28
 *  Author: Alex2
 */ 


#ifndef DHT22_H_
#define DHT22_H_

#define DHT22_PORT PORTK1

#include "main.h"

uint8_t dht22_init (void);
uint8_t dht22_GetData (uint8_t *data);

#endif /* DHT22_H_ */