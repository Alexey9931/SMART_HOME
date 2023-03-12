
#ifndef _BMP180_H_
#define _BMP180_H_


#include "main.h"

uint16_t Read(uint8_t address);
void BMP180_Calibration (void);
int32_t BMP180_get_temper(void);
int32_t BMP180_get_pressure();
int32_t BMP180_calculation () ;
//void bmp180CalcAltitude(int32_t pressure);


#endif /* INC_BMP180_H_ */
