/*
 * ILI9486.c
 *
 * Created: 05.03.2023 13:55:35
 *  Author: Alex2
 */ 
#include "ILI9486.h"
#include "Fonts.h"

unsigned int X_SIZE = 0;
unsigned int Y_SIZE = 0;
unsigned long dtt=0;

uint16_t POINT_COLOR;
uint16_t BACK_COLOR;
uint16_t cursor_x,cursor_y;
uint8_t    textsize;

void ILI9486_ini(void)
{
	ILI9486_reset();
	_delay_ms(300);
	wdt_reset();
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x01);
	CS_IDLE;
	_delay_ms(150);
	wdt_reset();
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x28);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x3A);
	CD_DATA;
	ILI9486_Write8(0x55);
	CS_IDLE;

	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xC0);
	CD_DATA;
	ILI9486_Write8(0x0D);
	CD_DATA;
	ILI9486_Write8(0x0D);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xC1);
	CD_DATA;
	ILI9486_Write8(0x43);
	CD_DATA;
	ILI9486_Write8(0x00);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xC2);
	CD_DATA;
	ILI9486_Write8(0x00);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xC5);
	CD_DATA;
	ILI9486_Write8(0x00);
	CD_DATA;
	ILI9486_Write8(0x48);
	CD_DATA;
	ILI9486_Write8(0x00);
	CD_DATA;
	ILI9486_Write8(0x48);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xB1);
	CD_DATA;
	ILI9486_Write8(0xC0);
	CD_DATA;
	ILI9486_Write8(0x00);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xB4);
	CD_DATA;
	ILI9486_Write8(0x00);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xB6);
	CD_DATA;
	ILI9486_Write8(0x02);
	CD_DATA;
	ILI9486_Write8(0x02);
	CD_DATA;
	ILI9486_Write8(0x3B);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xE0);
	CD_DATA;
	ILI9486_Write8(0x0F);
	CD_DATA;
	ILI9486_Write8(0x21);
	CD_DATA;
	ILI9486_Write8(0x1C);
	CD_DATA;
	ILI9486_Write8(0x0B);
	CD_DATA;
	ILI9486_Write8(0x0E);
	CD_DATA;
	ILI9486_Write8(0x08);
	CD_DATA;
	ILI9486_Write8(0x49);
	CD_DATA;
	ILI9486_Write8(0x98);
	CD_DATA;
	ILI9486_Write8(0x38);
	CD_DATA;
	ILI9486_Write8(0x09);
	CD_DATA;
	ILI9486_Write8(0x11);
	CD_DATA;
	ILI9486_Write8(0x03);
	CD_DATA;
	ILI9486_Write8(0x14);
	CD_DATA;
	ILI9486_Write8(0x10);
	CD_DATA;
	ILI9486_Write8(0x00);
	CS_IDLE;

	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0xE1);
	CD_DATA;
	ILI9486_Write8(0x0F);
	CD_DATA;
	ILI9486_Write8(0x2F);
	CD_DATA;
	ILI9486_Write8(0x2B);
	CD_DATA;
	ILI9486_Write8(0x0C);
	CD_DATA;
	ILI9486_Write8(0x0E);
	CD_DATA;
	ILI9486_Write8(0x06);
	CD_DATA;
	ILI9486_Write8(0x47);
	CD_DATA;
	ILI9486_Write8(0x76);
	CD_DATA;
	ILI9486_Write8(0x37);
	CD_DATA;
	ILI9486_Write8(0x07);
	CD_DATA;
	ILI9486_Write8(0x11);
	CD_DATA;
	ILI9486_Write8(0x04);
	CD_DATA;
	ILI9486_Write8(0x23);
	CD_DATA;
	ILI9486_Write8(0x1E);
	CD_DATA;
	ILI9486_Write8(0x00);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x11); // Sleep out, also SW reset
	CS_IDLE;
	_delay_ms(150);
	wdt_reset();
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x29);
	CS_IDLE;
	
	ILI9486_SetRotation(0);	
}
void ILI9486_SendCommand(uint16_t cmd)
{
	CD_COMMAND;//лапка в состоянии посылки команды
	ILI9486_Write16(cmd);
	CD_DATA;
}
void ILI9486_SendData(uint16_t dt)
{
	CD_DATA;//лапка в состоянии посылки данных
	ILI9486_Write16(dt);
}
void ILI9486_reset(void)
{
	setWriteDir();
	CS_IDLE;
	RD_IDLE;
	WR_IDLE;
	RESET_IDLE;
	_delay_ms(50);
	RESET_ACTIVE;
	_delay_ms(100);
	RESET_IDLE;
	_delay_ms(100);
	ILI9486_Write_Cmd_Data(0xB0,0x00);
}
void ILI9486_Write_Cmd_Data(uint16_t cmd, uint16_t dat)
{
	CS_ACTIVE;
	ILI9486_SendCommand(cmd);
	ILI9486_SendData(dat);
	CS_IDLE;
}
void ILI9486_Write8(unsigned char dt)
{
	DATA_PORT=dt;
	WR_STROBE;
}
void ILI9486_Write16(uint16_t dat)
{
	uint8_t h = (dat)>>8, l = dat; 
	ILI9486_Write8(h); 
	ILI9486_Write8(l);
}
unsigned char ILI9486_Read8()
{
	unsigned char dst;
	RD_IDLE;
	RD_ACTIVE;
	dst = DATA_PIN;
	RD_IDLE;
	return dst;
}
unsigned long ILI9486_ReadReg(unsigned char r)
{
	long int id;
	unsigned char x;
	CS_ACTIVE;//выбор дисплея
	//CD_COMMAND;//лапка в состоянии посылки команды
	CD_COMMAND;
	uint8_t h = (r)>>8, l = r; 
	ILI9486_Write8(h); 
	ILI9486_Write8(l); 
	CD_DATA;
	setReadDir();
	_delay_us(1);
	
	_delay_us(1);
	x = ILI9486_Read8();
	id |= x;
	//id = x << 8;
	_delay_us(1);
	x = ILI9486_Read8();
	//id |= x;
	
	
	RD_IDLE;
	CS_IDLE;
	setWriteDir();
	_delay_us(150);//stabilization time
	return id;
}
void ILI9486_SetRotation(unsigned char r)
{
	CS_ACTIVE;
	ILI9486_SendCommand(0x36);
	switch(r)
	{
		case 0:
			ILI9486_Write8(0x48);
			X_SIZE = 320;
			Y_SIZE = 480;
			break;
		case 1:
			ILI9486_Write8(0x28);
			X_SIZE = 480;
			Y_SIZE = 320;
			break;
		case 2:
			ILI9486_Write8(0x98);
			X_SIZE = 320;
			Y_SIZE = 480;
			break;
		case 3:
			ILI9486_Write8(0xF8);
			X_SIZE = 480;
			Y_SIZE = 320;
			break;
	}
	CS_IDLE;
}
void ILI9486_WriteRegister32(unsigned char r, unsigned long d)
{
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(r);
	CD_DATA;
	_delay_us(1);
	ILI9486_Write8(d>>24);
	_delay_us(1);
	ILI9486_Write8(d>>16);
	_delay_us(1);
	ILI9486_Write8(d>>8);
	_delay_us(1);
	ILI9486_Write8(d);
	CS_IDLE;
}
void ILI9486_SetAddrWindow(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x2A);
	CD_DATA;
	ILI9486_Write8(x1>>8);
	CD_DATA;
	ILI9486_Write8(x1);
	CD_DATA;
	ILI9486_Write8(x2>>8);
	CD_DATA;
	ILI9486_Write8(x2);
	CS_IDLE;
	
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x2B);
	CD_DATA;
	ILI9486_Write8(y1>>8);
	CD_DATA;
	ILI9486_Write8(y1);
	CD_DATA;
	ILI9486_Write8(y2>>8);
	CD_DATA;
	ILI9486_Write8(y2);
	CS_IDLE;
}
//работа с графикой
void ILI9486_drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	ILI9486_SetAddrWindow(x, y, x, y);
	ILI9486_Write_Cmd_Data(0x2C, color);
}
void ILI9486_FillScreen(uint16_t color)
{
	uint16_t i,j;
	ILI9486_SetAddrWindow(0,0,X_SIZE-1,Y_SIZE-1);
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x2C);
	CD_DATA;
	for(i=0;i<X_SIZE;i++)
	{
		for (j=0;j<Y_SIZE;j++)
		{
			CD_DATA;
			ILI9486_Write8(color>>8);
			CD_DATA;
			ILI9486_Write8(color & 0xff);
		}
	}
	CS_IDLE;
}
void ILI9486_fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	unsigned int end;
	if (w < 0) {
		w = -w;
		x -= w;
	}                           //+ve w
	end = x + w;
	if (x < 0)
	x = 0;
	if (end > X_SIZE)
	end = X_SIZE;
	w = end - x;
	if (h < 0) {
		h = -h;
		y -= h;
	}                           //+ve h
	end = y + h;
	if (y < 0)
	y = 0;
	if (end > Y_SIZE)
	end = Y_SIZE;
	h = end - y;

	ILI9486_SetAddrWindow(x, y, x + w - 1, y + h - 1);
	CS_ACTIVE;
	CD_COMMAND;
	ILI9486_Write8(0x2C);
	CD_DATA;
	if (h > w) {
		end = h;
		h = w;
		w = end;
	}
	while (h-- > 0) {
		end = w;
		do {
			CD_DATA;
			ILI9486_Write8(color>>8);
			CD_DATA;
			ILI9486_Write8(color & 0xff);
		} while (--end != 0);
	}
	CS_IDLE;
}
void ILI9486_DrawFastVLine(uint16_t x, uint16_t y,uint16_t h, uint16_t color)
{
	if ((x >= X_SIZE) || (y >= Y_SIZE || h < 1))
	{
		return;
	}
	if ((y + h - 1) >= Y_SIZE)
	{
		h = Y_SIZE - y;
	}

	ILI9486_fillRect(x, y, 1, h, color);
}
void ILI9486_DrawFastHLine(uint16_t x, uint16_t y,uint16_t w, uint16_t color)
{
	if ((x >= X_SIZE) || (y >= Y_SIZE || w < 1))
	{
		return;
	}
	if ((x + w - 1) >=X_SIZE)
	{
		w =X_SIZE - x;
	}
	ILI9486_fillRect(x, y, w, 1, color);
}
void ILI9486_Draw_Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
{
	// Update in subclasses if desired!
	if (x0 == x1) {
		if (y0 > y1)
		swap(y0, y1);
		ILI9486_DrawFastVLine(x0, y0, y1 - y0 + 1, color);
		} else if (y0 == y1) {
		if (x0 > x1)
		swap(x0, x1);
		ILI9486_DrawFastHLine(x0, y0, x1 - x0 + 1, color);
		} else {
		ILI9486_writeLine(x0, y0, x1, y1, color);
	}
}
void ILI9486_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) 
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
		} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			ILI9486_drawPixel(y0, x0, color);
			} else {
			ILI9486_drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}
void ILI9486_Draw_Circle(int16_t x0, int16_t y0, int16_t r,int16_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	ILI9486_drawPixel(x0  , y0+r, color);
	ILI9486_drawPixel(x0  , y0-r, color);
	ILI9486_drawPixel(x0+r, y0  , color);
	ILI9486_drawPixel(x0-r, y0  , color);

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

		ILI9486_drawPixel(x0 + x, y0 + y, color);
		ILI9486_drawPixel(x0 - x, y0 + y, color);
		ILI9486_drawPixel(x0 + x, y0 - y, color);
		ILI9486_drawPixel(x0 - x, y0 - y, color);
		ILI9486_drawPixel(x0 + y, y0 + x, color);
		ILI9486_drawPixel(x0 - y, y0 + x, color);
		ILI9486_drawPixel(x0 + y, y0 - x, color);
		ILI9486_drawPixel(x0 - y, y0 - x, color);
	}
}
void ILI9486_fillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color)
{
	ILI9486_DrawFastVLine(x0, y0-r, 2*r+1, color);
	ILI9486_fillCircleHelper(x0, y0, r, 3, 0, color);
}
void ILI9486_fillCircleHelper(int16_t x0, int16_t y0, int16_t r,uint8_t cornername, int16_t delta, uint16_t color)
{

	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			ILI9486_DrawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
			ILI9486_DrawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			ILI9486_DrawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
			ILI9486_DrawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}
void ILI9486_DrawRect(int16_t x, int16_t y,  int16_t w, int16_t h,  uint16_t color)
{
	ILI9486_DrawFastHLine(x, y, w, color);
	ILI9486_DrawFastHLine(x, y+h-1, w, color);
	ILI9486_DrawFastVLine(x, y, h, color);
	ILI9486_DrawFastVLine(x+w-1, y, h, color);
}
void ILI9486_drawRGBBitmap(int x, int y, uint16_t *bitmap,int w, int h) 
{
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			ILI9486_drawPixel(x + i, y+j, bitmap[j * w + i]);
		}
	}
}
void ILI9486_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
{
  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1);
    swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1);
    swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1);
    swap(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    ILI9486_DrawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2)
    last = y1; // Include y1 scanline
  else
    last = y1 - 1; // Skip it

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      swap(a, b);
    ILI9486_DrawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if (a > b)
      swap(a, b);
    ILI9486_DrawFastHLine(a, y, b - a + 1, color);
  }
}
//работа с текстом 
void ILI9486_SetTextColor(uint16_t c, uint16_t b)
{
	POINT_COLOR   = c;
	BACK_COLOR = b;
}
void ILI9486_Print_Char14x24(uint16_t x,uint16_t y,uint8_t data ,uint8_t mode)
{
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/1424.txt");*/
	
	if((x>X_SIZE-14)||(y>Y_SIZE-24)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[4*14*24/8+10] = {0};
	uint8_t bytes[14*24/8+1] = {0};
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	WORD s1;
	uint8_t result;

	/*pf_lseek(((uint32_t)data-65)*4*14*24/8); //Установим курсор чтения на 0
	pf_read(tem,4*14*24/8-1,&s1);
	for (int m = 0; m < 14*24/8; m++)
	{
		if (data != ' ')
		{		
			l = 0;
			while (tem[p] != ',')
			{
				arr[l] = tem[p];
				p++;
				l++;
			}
			p += 2;
			sscanf(arr, "%x", &bytes[m]);
		}
		else
		{
			bytes[m] = 0x00;
		}
	}
	*/
	ILI9486_SetAddrWindow(x,y,x+16-1,y+24-1);
	for(i=0;i<24/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<14;k++)
			{
				if (data == ' ')
				{
					temp = 0x00;
				}
				else
				{
					temp=Consolas14x24[(data-65)*(24/8)*14+k*(24/8)+i];
				}
				//temp=bytes[k*(24/8)+i];   
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
void ILI9486_Print_Char18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/1832.txt");*/
	
	if((x>X_SIZE-18)||(y>Y_SIZE-32)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[4*18*32/8+10] = {0};
	uint8_t bytes[18*32/8+1] = {0};
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	WORD s1;
	uint8_t result;
/*
	pf_lseek(((uint32_t)data-' ')*4*18*32/8); //Установим курсор чтения на 0
	pf_read(tem,4*18*32/8-1,&s1);
	for (int m = 0; m < 18*32/8; m++)
	{
		if (data != ' ')
		{
			l = 0;
			while (tem[p] != ',')
			{
				arr[l] = tem[p];
				p++;
				l++;
			}
			p += 2;
			sscanf(arr, "%x", &bytes[m]);
		}
		else
		{
			bytes[m] = 0x00;
		}
	}
*/	
	ILI9486_SetAddrWindow(x,y,x+18-1,y+32-1);
	for(i=0;i<32/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<18;k++)
			{
				temp=Consolas18x32[(data-' ')*(32/8)*18+k*(32/8)+i];
				//temp=read_symbol_from_SD((data-' ')*(32/8)*18+k*(32/8)+i);
				//temp=bytes[k*(32/8)+i];
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
	// Завершаем работу с файлом
	pf_mount(0x00);

}
void ILI9486_Print_ConsolasChar16x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/1832.txt");*/
	
	if((x>X_SIZE-16)||(y>Y_SIZE-24)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[4*18*32/8+10] = {0};
	uint8_t bytes[18*32/8+1] = {0};
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	WORD s1;
	uint8_t result;
/*
	pf_lseek(((uint32_t)data-' ')*4*18*32/8); //Установим курсор чтения на 0
	pf_read(tem,4*18*32/8-1,&s1);
	for (int m = 0; m < 18*32/8; m++)
	{
		if (data != ' ')
		{
			l = 0;
			while (tem[p] != ',')
			{
				arr[l] = tem[p];
				p++;
				l++;
			}
			p += 2;
			sscanf(arr, "%x", &bytes[m]);
		}
		else
		{
			bytes[m] = 0x00;
		}
	}
*/	
	ILI9486_SetAddrWindow(x,y,x+16-1,y+24-1);
	for(i=0;i<24/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<16;k++)
			{
				temp=Consolas16x24[(data-32)*(24/8)*16+k*(24/8)+i];
				//temp=read_symbol_from_SD((data-' ')*(32/8)*18+k*(32/8)+i);
				//temp=bytes[k*(32/8)+i];
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);

}
void ILI9486_Print_Char32x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	if((x>X_SIZE-32)||(y>Y_SIZE-32)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[4*32*32/8+10] = {0};
	uint8_t bytes[32*32/8+1] = {0};
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	

	ILI9486_SetAddrWindow(x,y,x+32-1,y+32-1);
	for(i=0;i<32/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<32;k++)
			{
				temp=Syffaen32x32[(data-32)*(32/8)*32+k*(32/8)+i];
				//temp=read_symbol_from_SD((data-' ')*(32/8)*18+k*(32/8)+i);
				//temp=bytes[k*(32/8)+i];
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
}
void ILI9486T_Print_Char8x16(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
	uint8_t temp;
	uint8_t pos,t;
	//uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;
	if(x>X_SIZE-8||y>Y_SIZE-16)return;
	num=num-' ';
	ILI9486_SetAddrWindow(x,y,y+16-1, x+8-1);
	if(!mode)
	{
		for(pos=0;pos<16;pos++)
		{
			temp=asc2_1608[(uint16_t)num*16+pos];
			for(t=0;t<8;t++)
			{
				if(temp&0x01)
				{
					POINT_COLOR=colortemp;
				}
				else
				{
					POINT_COLOR=BACK_COLOR;
				}
				ILI9486_drawPixel(x+t,y+pos,POINT_COLOR);
				temp>>=1;
			}
		}
	}
	else
	{
		for(pos=0;pos<16;pos++)
		{
			temp=asc2_1608[(uint16_t)num*16+pos];
			for(t=0;t<8;t++)
			{
				if(temp&0x01)ILI9486_drawPixel(x+t,y+pos,POINT_COLOR);
				temp>>=1;
			}
		}
	}
	POINT_COLOR=colortemp;
}
void ILI9486_Print_Char24x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/2432.txt");*/
	
	if((x>X_SIZE-24)||(y>Y_SIZE-32)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[4*24*32/8+10] = {0};
	uint8_t bytes[24*32/8+1] = {0};
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	WORD s1;
	uint8_t result;
/*
	pf_lseek(((uint32_t)data-48)*4*24*32/8); //Установим курсор чтения на 0
	pf_read(tem,4*24*32/8-1,&s1);
	for (int m = 0; m < 24*32/8; m++)
	{
		l = 0;
		while (tem[p] != ',')
		{
			arr[l] = tem[p];
			p++;
			l++;
		}
		p += 2;
		sscanf(arr, "%x", &bytes[m]);
	}
	*/
	ILI9486_SetAddrWindow(x,y,x-1,y-1);
	for(i=0;i<32/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<24;k++)
			{
				temp=Font24x32_Clock[(data-48)*(32/8)*24+k*(32/8)+i];
				//temp=bytes[k*(32/8)+i];
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
void ILI9486_Print_Char32x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/3248.txt");*/
	
	if((x>X_SIZE-32)||(y>Y_SIZE-48)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[778] = {0};//6*32*48/8+10
	uint8_t bytes[193] = {0};//32*48/8+1
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	WORD s1;
	uint8_t result;

	/*pf_lseek(((uint32_t)data-45)*4*32*48/8); //Установим курсор чтения на 0
	pf_read(tem,767,&s1);//6*32*48/8-1
	for (int m = 0; m < 32*48/8; m++)
	{
		if (data != ' ')
		{
			l = 0;
			while (tem[p] != ',')
			{
				arr[l] = tem[p];
				p++;
				l++;
			}
			p += 2;
			sscanf(arr, "%x", &bytes[m]);
		}
		else
		{
			bytes[m] = 0x00;
		}
	}
	*/
	ILI9486_SetAddrWindow(x,y,x-1,y-1);
	for(i=0;i<48/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<32;k++)
			{
				if (data == ' ')
				{
					temp = 0x00;
				}
				else
				{
					temp=Font32x48_Num[(data-45)*(48/8)*32+k*(48/8)+i];
				}
				//temp=bytes[k*(48/8)+i];
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
void ILI9486_Print_Char40x40(uint16_t x,uint16_t y,uint8_t data,uint8_t mode)
{
	if((x>X_SIZE-40)||(y>Y_SIZE-40)) return;
	uint8_t i,j,k,temp;
	uint8_t tem[778] = {0};//6*32*48/8+10
	uint8_t bytes[193] = {0};//32*48/8+1
	char arr[10] = {0};
	int l = 0;
	int p = 0;
	WORD s1;
	uint8_t result;

	ILI9486_SetAddrWindow(x,y,x-1,y-1);
	for(i=0;i<40/8;i++)
	{
		for(j=0;j<8;j++)
		{
			for(k=0;k<40;k++)
			{
				if (data == ' ')
				{
					temp = 0x00;
				}
				else
				{
					temp=Syffaen40x40[(data-192)*(40/8)*40+k*(40/8)+i];
				}
				//temp=bytes[k*(48/8)+i];
				wdt_reset();
				if(mode==TFT_STRING_MODE_BACKGROUND)
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
					else
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),BACK_COLOR);
					}
				}
				else
				{
					if(temp&(0x01<<j))
					{
						ILI9486_drawPixel(x+k,y+(8*i+j),POINT_COLOR);
					}
				}
			}
		}
	}
}
void ILI9486_SetCursor(int16_t x, int16_t y)
{
	cursor_x = y;
	cursor_y = x;
}
void ILI9486_SetTextSize(uint8_t s)
{
	textsize = s ;
}
void ILI9486_Print_String40x40(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=33;
	uint8_t font_h=40;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_Char40x40(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String18x32(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=18;
	uint8_t font_h=32;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_Char18x32(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_StringConsolas16x24(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=14;
	uint8_t font_h=24;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_ConsolasChar16x24(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String32x32(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=28;
	uint8_t font_h=32;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_Char32x32(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String24x32(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=25;
	uint8_t font_h=25;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_Char24x32(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String32x48(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=32;
	uint8_t font_h=40;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_Char32x48(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String14x24(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=14;
	uint8_t font_h=24;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486_Print_Char14x24(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String8x16(uint8_t *string,uint8_t TFT_STRING_MODE)
{
	uint8_t i=0;
	uint8_t font_w=8;
	uint8_t font_h=16;

	while(*(string+i)!='\0')
	{

		if(*(string+i)==0)
		{
			return;
		}

		if(*(string+i)=='\n')
		{
			cursor_x+=font_h;
			cursor_y=0;
			string++;
		}

		if(cursor_y>X_SIZE-font_w)
		{
			cursor_y=0;cursor_x+=font_h;
		}

		if(cursor_x>Y_SIZE-font_h)
		{
			cursor_x=cursor_y=0;
		}

		ILI9486T_Print_Char8x16(cursor_y,cursor_x,*(string+i),TFT_STRING_MODE);
		cursor_y+=font_w;
		i++;
	}
}
void ILI9486_Print_String(uint8_t *string , uint8_t TFT_STRING_MODE)
{
	switch(textsize)
	{
		case 1 : ILI9486_Print_String8x16(string, TFT_STRING_MODE);
		break;
		case 2 : ILI9486_Print_String14x24(string, TFT_STRING_MODE);
		break;
		case 3 : ILI9486_Print_String18x32(string, TFT_STRING_MODE);
		break;

	}
}
void ILI9486_Print_Number(long  Number, uint8_t TFT_STRING_MODE)
{

	long Temp =1;

	if (Number <= 0)
	{
		switch(textsize)
		{
			case 1 : ILI9486T_Print_Char8x16(cursor_y,cursor_x,'0', TFT_STRING_MODE);  cursor_y+=8;
			break;
			case 2 : ILI9486_Print_Char14x24(cursor_y,cursor_x,'0', TFT_STRING_MODE); cursor_y+=14;
			break;
			case 3 : ILI9486_Print_Char18x32(cursor_y,cursor_x,'0', TFT_STRING_MODE); cursor_y+=18;
			break;

		}
	}
	else
	{
		while (Number!=0)
		{
			uint8_t Reminder = Number % 10;
			Number = Number / 10 ;
			Temp = Temp * 10 + Reminder ;
		}
		while (Temp != 1)
		{
			uint8_t Reminder2 = Temp % 10 ;

			switch(textsize)
			{
				case 1 : ILI9486T_Print_Char8x16(cursor_y,cursor_x,Reminder2+48, TFT_STRING_MODE);   cursor_y+=8;
				break;
				case 2 : ILI9486_Print_Char14x24(cursor_y,cursor_x,Reminder2+48, TFT_STRING_MODE);  cursor_y+=14;
				break;
				case 3 : ILI9486_Print_Char18x32(cursor_y,cursor_x,Reminder2+48, TFT_STRING_MODE);  cursor_y+=18;
				break;

			}
			Temp = Temp / 10 ;
		}
	}


}

void ILI9486_Draw_Image(char *filename, uint32_t width, uint32_t height, uint32_t x_pos, uint32_t y_pos)
{
	FATFS fs;
	WORD s1;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open(filename);

	uint8_t array[10+(6*width*height/10)];
	int l, p = 0;
	for (int k = 0; k < 10; k++)
	{
		memset(array, '\0', sizeof(uint8_t) * strlen(array));//очистка массива
		char arr[10] = {};
		p = 0;
		pf_read(array,6*width*height/10,&s1);
		wdt_reset();
		for (int m = 0; m < width*height/10; m++)
		{
			l = 0;
			while (array[p] != ',')
			{
				arr[l] = array[p];
				p++;
				l++;
			}
			p += 2;
			ILI9486_drawPixel(x_pos+m%width, y_pos+(width/10)*k+(m/height), (uint16_t)strtol(arr,NULL,16));
			wdt_reset();
		}
	}
	pf_mount(0x00);
}
