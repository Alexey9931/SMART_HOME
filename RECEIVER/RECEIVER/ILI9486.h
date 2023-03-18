/*
 * ILI9486.h
 *
 * Created: 05.03.2023 13:55:47
 *  Author: Alex2
 */ 


#ifndef ILI9486_H_
#define ILI9486_H_

#include "main.h"

#define swap(a,b) {int16_t t=a;a=b;b=t;}
#define DATA_DDR DDRA
#define DATA_PORT PORTA
#define DATA_PIN PINA
#define COMMAND_DDR DDRC
#define COMMAND_PORT PORTC

#define LCD_CS PORTC6//Chip Select
#define LCD_CD PORTC5//Command/Data
#define LCD_WR PORTC4//LCD Write
#define LCD_RD PORTC3//LCD Read
#define LCD_RESET PORTC7//LCD Reset
#define RESET_IDLE COMMAND_PORT|=(1<<LCD_RESET)
#define CS_IDLE COMMAND_PORT|=(1<<LCD_CS)
#define WR_IDLE COMMAND_PORT|=(1<<LCD_WR)
#define RD_IDLE COMMAND_PORT|=(1<<LCD_RD)
#define RESET_ACTIVE COMMAND_PORT&=~(1<<LCD_RESET)
#define CS_ACTIVE COMMAND_PORT&=~(1<<LCD_CS)
#define WR_ACTIVE COMMAND_PORT&=~(1<<LCD_WR)
#define RD_ACTIVE COMMAND_PORT&=~(1<<LCD_RD)
#define CD_COMMAND COMMAND_PORT&=~(1<<LCD_CD)
#define CD_DATA COMMAND_PORT|=(1<<LCD_CD)
#define BLACK 0x0000
#define BLUE 0x001F
#define LIGHTBLUE 0x3D9F
#define RED 0xF800
#define ORANGE 0xFBA2
#define GREEN 0x1FE0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define setReadDir() DATA_DDR=0x00
#define setWriteDir() DATA_DDR=0xFF
#define WR_STROBE {WR_ACTIVE;WR_IDLE;}
	
	
#define TFT_STRING_MODE_NO_BACKGROUND	0x01
#define TFT_STRING_MODE_BACKGROUND	0x00

void ILI9486_ini(void);
void ILI9486_SendCommand(uint16_t cmd);
void ILI9486_SendData(uint16_t dt);
void ILI9486_reset(void);
void ILI9486_Write_Cmd_Data(uint16_t cmd, uint16_t dat);
void ILI9486_Write8(unsigned char dt);
unsigned char ILI9486_Read8();
unsigned long ILI9486_ReadReg(unsigned char r);
void ILI9486_SetRotation(unsigned char r);
void ILI9486_WriteRegister32(unsigned char r, unsigned long d);
void ILI9486_SetAddrWindow(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void ILI9486_drawPixel(uint16_t x, uint16_t y, uint16_t color);
void ILI9486_FillScreen(uint16_t color);
void ILI9486_fillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int color);
void ILI9486_DrawFastVLine(uint16_t x, uint16_t y,uint16_t h, uint16_t color);
void ILI9486_DrawFastHLine(uint16_t x, uint16_t y,uint16_t w, uint16_t color);
void ILI9486_Draw_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void ILI9486_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void ILI9486_Draw_Circle(int16_t x0, int16_t y0, int16_t r,int16_t color);
void ILI9486_fillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color);
void ILI9486_fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, int16_t delta, uint16_t color);
void ILI9486_DrawRect(int16_t x, int16_t y,  int16_t w, int16_t h,  uint16_t color);
void ILI9486_drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap,int16_t w, int16_t h);
void ILI9486_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

void ILI9486_SetTextColor(uint16_t c, uint16_t b);
void ILI9486_Print_Char14x24(uint16_t x,uint16_t y,uint8_t data ,uint8_t mode);
void ILI9486_Print_Char18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void ILI9486_Print_Char8x16(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
void ILI9486_Print_Char24x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void ILI9486_Print_Char32x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void ILI9486_SetCursor(int16_t x, int16_t y);
void ILI9486_SetTextSize(uint8_t s);
void ILI9486_Print_String18x32(uint8_t *string,uint8_t TFT_STRING_MODE);
void ILI9486_Print_String14x24(uint8_t *string,uint8_t TFT_STRING_MODE);
void ILI9486_Print_String8x16(uint8_t *string,uint8_t TFT_STRING_MODE);
void ILI9486_Print_String24x32(uint8_t *string,uint8_t TFT_STRING_MODE);
void ILI9486_Print_String32x48(uint8_t *string,uint8_t TFT_STRING_MODE);
void ILI9486_Print_String(uint8_t *string , uint8_t TFT_STRING_MODE);
void ILI9486_Print_Number(long  Number, uint8_t TFT_STRING_MODE);



#endif /* ILI9486_H_ */