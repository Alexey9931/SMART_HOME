/*
 * SD.h
 *
 * Created: 30.05.2022 23:46:34
 *  Author: Alex2
 */ 


#ifndef SD_H_
#define SD_H_

#include "main.h"

/*
#define MOSI_SD 2 //PORTC
#define MISO_SD 1 //PORTC
#define SCK_SD 3  //PORTC
#define SS_SD 0   //PORTB

void SPI_SendByte (unsigned char byte);
unsigned char SPI_ReceiveByte(void);
unsigned char SD_cmd (char dt0,char dt1,char dt2,char dt3,char dt4,char dt5);
unsigned char SD_Init(void);
*/
uint8_t read_symbol_from_SD (uint16_t symbol);

#endif /* SD_H_ */