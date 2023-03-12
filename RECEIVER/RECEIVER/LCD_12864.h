/*
 * LCD_12864.h
 *
 * Created: 12.05.2022 20:45:14
 *  Author: Alex2
 */ 


#ifndef LCD_12864_H_
#define LCD_12864_H_

#include "main.h"

#define RS PORTD7
#define RW PORTD6
#define E PORTD5
//#define PSB PORTB1

void LCD_12864_ini(void);
void str_LCD_12864 (char *str1);
void LCD_12864_GrapnicMode(uint8_t enable);
void LCD_12864_Draw_bitmap(const unsigned char *bitmap);
void LCD_12864_Draw_pixel(uint8_t x, uint8_t y, uint8_t *Frame_buffer);
void LCD_12864_Clean_pixel(uint8_t x, uint8_t y, uint8_t *Frame_buffer);
void clear_LCD_12864(void);
void setpos_LCD_12864(unsigned char x, unsigned y);
void LCD_12864_send_data(uint8_t data);
void LCD_12864_send_command(uint8_t command);
void strob(void);
void LCD_12864_print_symbol(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer);
void LCD_12864_print_symbol_11_16(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer);
void LCD_12864_print_symbol_7_11(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer);
void LCD_12864_print_symbol_4_6(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer);
void LCD_12864_Print_jpeg(uint8_t *Frame_buffer);
void LCD_12864_Clean(uint8_t *Frame_buffer);
void LCD_12864_Clean_Frame_buffer(uint8_t *Frame_buffer);
void LCD_12864_Decode_UTF8(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer);
void LCD_12864_Print_11_16(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer);
void LCD_12864_Print_7_11(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer);
void LCD_12864_Print_4_6(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer);
void LCD_12864_Inversion(uint16_t x_start, uint16_t x_end);
void LCD_12864_Draw_line(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t *Frame_buffer);
void LCD_12864_Draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *Frame_buffer);
void LCD_12864_Draw_rectangle_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *Frame_buffer);
void LCD_12864_Draw_circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t *Frame_buffer);
void LCD_12864_Draw_circle_filled(int16_t x, int16_t y, int16_t radius, uint8_t *Frame_buffer);
void LCD_12864_Draw_triangle_filled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t *Frame_buffer);

#endif /* LCD_12864_H_ */