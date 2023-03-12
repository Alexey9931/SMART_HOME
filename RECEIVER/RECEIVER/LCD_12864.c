/*
 * LCD_12864.c
 *
 * Created: 12.05.2022 20:45:00
 *  Author: Alex2
 */ 
#include "LCD_12864.h"

//extern uint8_t Frame_buffer[1024]; //Буфер кадра
extern uint8_t ST7920_width; //Ширина дисплея в пикселях
extern uint8_t ST7920_height; //Высота дисплея в пикселях

/*-----------------------------------Шрифт 5*7----------------------------------*/
/*__flash const uint8_t Font5_7[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //0/ --> space     20
	0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, //1/ --> !         21
	0x00, 0x07, 0x00, 0x07, 0x00, 0x00, //2/ --> "         22 и т.д.
	0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14, //3/ --> #
	0x00, 0x24, 0x2A, 0x7F, 0x2A, 0x12, //4/ --> $
	0x00, 0x23, 0x13, 0x08, 0x64, 0x62, //5/ --> %
	0x00, 0x36, 0x49, 0x55, 0x22, 0x40, //6/ --> &
	0x00, 0x00, 0x05, 0x03, 0x00, 0x00, //7/ --> '
	0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, //8/ --> (
	0x00, 0x41, 0x22, 0x1C, 0x00, 0x00, //9/ --> )
	0x00, 0x14, 0x08, 0x3E, 0x08, 0x14, //10/ --> *
	0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, //11/ --> +
	0x00, 0xA0, 0x60, 0x00, 0x00, 0x00, //12/ --> ,
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08, //13/ --> -
	0x00, 0x60, 0x60, 0x00, 0x00, 0x00, //14/ --> .
	0x00, 0x20, 0x10, 0x08, 0x04, 0x02, //15/ --> /
	0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E, //16/ --> 0
	0x00, 0x00, 0x42, 0x7F, 0x40, 0x00, //17/ --> 1
	0x00, 0x42, 0x61, 0x51, 0x49, 0x46, //18/ --> 2
	0x00, 0x21, 0x41, 0x45, 0x4B, 0x31, //19/ --> 3
	0x00, 0x18, 0x14, 0x12, 0x7F, 0x10, //20/ --> 4
	0x00, 0x27, 0x45, 0x45, 0x45, 0x39, //21/ --> 5
	0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30, //22/ --> 6
	0x00, 0x01, 0x71, 0x09, 0x05, 0x03, //23/ --> 7
	0x00, 0x36, 0x49, 0x49, 0x49, 0x36, //24/ --> 8
	0x00, 0x06, 0x49, 0x49, 0x29, 0x1E, //25/ --> 9
	0x00, 0x6C, 0x6C, 0x00, 0x00, 0x00, //26/ --> :
	0x00, 0x00, 0x56, 0x36, 0x00, 0x00, //27/ --> ;
	0x00, 0x08, 0x14, 0x22, 0x41, 0x00, //28/ --> <
	0x00, 0x24, 0x24, 0x24, 0x24, 0x24, //29/ --> =
	0x00, 0x00, 0x41, 0x22, 0x14, 0x08, //30/ --> >
	0x00, 0x02, 0x01, 0x51, 0x09, 0x06, //31/ --> ?
	0x00, 0x32, 0x49, 0x79, 0x41, 0x3E, //32/ --> @
	0x00, 0x7E, 0x11, 0x11, 0x11, 0x7E, //33/ --> A
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, //34/ --> B
	0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, //35/ --> C
	0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C, //36/ --> D
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, //37/ --> E
	0x00, 0x7F, 0x09, 0x09, 0x09, 0x01, //38/ --> F
	0x00, 0x3E, 0x41, 0x49, 0x49, 0x3A, //39/ --> G
	0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, //40/ --> H
	0x00, 0x00, 0x41, 0x7F, 0x41, 0x00, //41/ --> I
	0x00, 0x20, 0x40, 0x41, 0x3F, 0x01, //42/ --> J
	0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, //43/ --> K
	0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, //44/ --> L
	0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, //45/ --> M
	0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F, //46/ --> N
	0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, //47/ --> O
	0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, //48/ --> P
	0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E, //49/ --> Q
	0x00, 0x7F, 0x09, 0x19, 0x29, 0x46, //50/ --> R
	0x00, 0x46, 0x49, 0x49, 0x49, 0x31, //51/ --> S
	0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, //52/ --> T
	0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, //53/ --> U
	0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F, //54/ --> V
	0x00, 0x3F, 0x40, 0x60, 0x40, 0x3F, //55/ --> W
	0x00, 0x63, 0x14, 0x08, 0x14, 0x63, //56/ --> X
	0x00, 0x07, 0x08, 0x70, 0x08, 0x07, //57/ --> Y
	0x00, 0x61, 0x51, 0x49, 0x45, 0x43, //58/ --> Z
	0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, //59/ --> [
	0x00, 0x15, 0x16, 0x7C, 0x16, 0x15, //60/ --> '\'
	0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, //61/ --> ]
	0x00, 0x04, 0x02, 0x01, 0x02, 0x04, //62/ --> ^
	0x00, 0x40, 0x40, 0x40, 0x40, 0x40, //63/ --> _
	0x00, 0x01, 0x02, 0x04, 0x00, 0x00, //64/ --> `
	0x00, 0x20, 0x54, 0x54, 0x54, 0x78, //65/ --> a
	0x00, 0x7F, 0x44, 0x44, 0x44, 0x38, //66/ --> b
	0x00, 0x38, 0x44, 0x44, 0x44, 0x00, //67/ --> c
	0x00, 0x38, 0x44, 0x44, 0x48, 0x7F, //68/ --> d
	0x00, 0x38, 0x54, 0x54, 0x54, 0x18, //69/ --> e
	0x00, 0x10, 0x7E, 0x11, 0x01, 0x02, //70/ --> f
	0x00, 0x0C, 0x52, 0x52, 0x52, 0x3E, //71/ --> g
	0x00, 0x7F, 0x08, 0x04, 0x04, 0x78, //72/ --> h
	0x00, 0x00, 0x44, 0x7D, 0x40, 0x00, //73/ --> i
	0x00, 0x20, 0x40, 0x40, 0x3D, 0x00, //74/ --> j
	0x00, 0x7F, 0x10, 0x28, 0x44, 0x00, //75/ --> k
	0x00, 0x00, 0x41, 0x7F, 0x40, 0x00, //76/ --> l
	0x00, 0x7C, 0x04, 0x18, 0x04, 0x78, //77/ --> m
	0x00, 0x7C, 0x08, 0x04, 0x04, 0x78, //78/ --> n
	0x00, 0x38, 0x44, 0x44, 0x44, 0x38, //79/ --> o
	0x00, 0x7C, 0x14, 0x14, 0x14, 0x08, //80/ --> p
	0x00, 0x08, 0x14, 0x14, 0x18, 0x7C, //81/ --> q
	0x00, 0x7C, 0x08, 0x04, 0x04, 0x08, //82/ --> r
	0x00, 0x48, 0x54, 0x54, 0x54, 0x20, //83/ --> s
	0x00, 0x04, 0x3F, 0x44, 0x40, 0x20, //84/ --> t
	0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C, //85/ --> u
	0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C, //86/ --> v
	0x00, 0x1E, 0x20, 0x10, 0x20, 0x1E, //87/ --> w
	0x00, 0x22, 0x14, 0x08, 0x14, 0x22, //88/ --> x
	0x00, 0x06, 0x48, 0x48, 0x48, 0x3E, //89/ --> y
	0x00, 0x44, 0x64, 0x54, 0x4C, 0x44, //90/ --> z
	0x00, 0x08, 0x36, 0x41, 0x00, 0x00, //91/ --> {
	0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, //92/ --> |
	0x00, 0x41, 0x36, 0x08, 0x00, 0x00, //93/ --> }
	0x00, 0x08, 0x08, 0x2A, 0x1C, 0x08, //94/ --> ~
	0x00, 0x7E, 0x11, 0x11, 0x11, 0x7E, //95/ --> А
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x33, //96/ --> Б
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x36, //97/ --> В
	0x00, 0x7F, 0x01, 0x01, 0x01, 0x03, //98/ --> Г
	0x00, 0xE0, 0x51, 0x4F, 0x41, 0xFF, //99/ --> Д
	0x00, 0x7F, 0x49, 0x49, 0x49, 0x41, //100/ --> Е
	0x00, 0x77, 0x08, 0x7F, 0x08, 0x77, //101/ --> Ж
	0x00, 0x41, 0x49, 0x49, 0x49, 0x36, //102/ --> З
	0x00, 0x7F, 0x10, 0x08, 0x04, 0x7F, //103/ --> И
	0x00, 0x7C, 0x21, 0x12, 0x09, 0x7C, //104/ --> Й
	0x00, 0x7F, 0x08, 0x14, 0x22, 0x41, //105/ --> К
	0x00, 0x20, 0x41, 0x3F, 0x01, 0x7F, //106/ --> Л
	0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F, //107/ --> М
	0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F, //108/ --> Н
	0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E, //109/ --> О
	0x00, 0x7F, 0x01, 0x01, 0x01, 0x7F, //110/ --> П
	0x00, 0x7F, 0x09, 0x09, 0x09, 0x06, //111/ --> Р
	0x00, 0x3E, 0x41, 0x41, 0x41, 0x22, //112/ --> С
	0x00, 0x01, 0x01, 0x7F, 0x01, 0x01, //113/ --> Т
	0x00, 0x47, 0x28, 0x10, 0x08, 0x07, //114/ --> У
	0x00, 0x1C, 0x22, 0x7F, 0x22, 0x1C, //115/ --> Ф
	0x00, 0x63, 0x14, 0x08, 0x14, 0x63, //116/ --> Х
	0x00, 0x7F, 0x40, 0x40, 0x40, 0xFF, //117/ --> Ц
	0x00, 0x07, 0x08, 0x08, 0x08, 0x7F, //118/ --> Ч
	0x00, 0x7F, 0x40, 0x7F, 0x40, 0x7F, //119/ --> Ш
	0x00, 0x7F, 0x40, 0x7F, 0x40, 0xFF, //120/ --> Щ
	0x00, 0x01, 0x7F, 0x48, 0x48, 0x30, //121/ --> Ъ
	0x00, 0x7F, 0x48, 0x30, 0x00, 0x7F, //122/ --> Ы
	0x00, 0x00, 0x7F, 0x48, 0x48, 0x30, //123/ --> Э
	0x00, 0x22, 0x41, 0x49, 0x49, 0x3E, //124/ --> Ь
	0x00, 0x7F, 0x08, 0x3E, 0x41, 0x3E, //125/ --> Ю
	0x00, 0x46, 0x29, 0x19, 0x09, 0x7f, //126/ --> Я
	0x00, 0x20, 0x54, 0x54, 0x54, 0x78, //127/ --> a
	0x00, 0x3c, 0x4a, 0x4a, 0x49, 0x31, //128/ --> б
	0x00, 0x7c, 0x54, 0x54, 0x28, 0x00, //129/ --> в
	0x00, 0x7c, 0x04, 0x04, 0x04, 0x0c, //130/ --> г
	0x00, 0xe0, 0x54, 0x4c, 0x44, 0xfc, //131/ --> д
	0x00, 0x38, 0x54, 0x54, 0x54, 0x18, //132/ --> е
	0x00, 0x6c, 0x10, 0x7c, 0x10, 0x6c, //133/ --> ж
	0x00, 0x44, 0x44, 0x54, 0x54, 0x28, //134/ --> з
	0x00, 0x7c, 0x20, 0x10, 0x08, 0x7c, //135/ --> и
	0x00, 0x7c, 0x41, 0x22, 0x11, 0x7c, //136/ --> й
	0x00, 0x7c, 0x10, 0x28, 0x44, 0x00, //137/ --> к
	0x00, 0x20, 0x44, 0x3c, 0x04, 0x7c, //138/ --> л
	0x00, 0x7c, 0x08, 0x10, 0x08, 0x7c, //139/ --> м
	0x00, 0x7c, 0x10, 0x10, 0x10, 0x7c, //140/ --> н
	0x00, 0x38, 0x44, 0x44, 0x44, 0x38, //141/ --> о
	0x00, 0x7c, 0x04, 0x04, 0x04, 0x7c, //142/ --> п
	0x00, 0x7C, 0x14, 0x14, 0x14, 0x08, //143/ --> р
	0x00, 0x38, 0x44, 0x44, 0x44, 0x28, //144/ --> с
	0x00, 0x04, 0x04, 0x7c, 0x04, 0x04, //145/ --> т
	0x00, 0x0C, 0x50, 0x50, 0x50, 0x3C, //146/ --> у
	0x00, 0x30, 0x48, 0xfc, 0x48, 0x30, //147/ --> ф
	0x00, 0x44, 0x28, 0x10, 0x28, 0x44, //148/ --> х
	0x00, 0x7c, 0x40, 0x40, 0x40, 0xfc, //149/ --> ц
	0x00, 0x0c, 0x10, 0x10, 0x10, 0x7c, //150/ --> ч
	0x00, 0x7c, 0x40, 0x7c, 0x40, 0x7c, //151/ --> ш
	0x00, 0x7c, 0x40, 0x7c, 0x40, 0xfc, //152/ --> щ
	0x00, 0x04, 0x7c, 0x50, 0x50, 0x20, //153/ --> ъ
	0x00, 0x7c, 0x50, 0x50, 0x20, 0x7c, //154/ --> ы
	0x00, 0x7c, 0x50, 0x50, 0x20, 0x00, //155/ --> э
	0x00, 0x28, 0x44, 0x54, 0x54, 0x38, //156/ --> ь
	0x00, 0x7c, 0x10, 0x38, 0x44, 0x38, //157/ --> ю
	0x00, 0x08, 0x54, 0x34, 0x14, 0x7c, //158/ --> я
	0x00, 0x7E, 0x4B, 0x4A, 0x4B, 0x42, //159/ --> Ё
	0x00, 0x38, 0x55, 0x54, 0x55, 0x18, //160/ --> ё
	0x00, 0x00, 0x06, 0x09, 0x09, 0x06, //161/ --> °
};
__flash const uint8_t Font4_6[] = {	0xFC, 0x84, 0x84, 0xFC, //0
									0x00, 0x08, 0xFC, 0x00, //1
									0x88, 0xC4, 0xA4, 0x9C, //2
									0x44, 0x84, 0x94, 0x7C, //3
									0x60, 0x50, 0xFC, 0x40, //4
									0x58, 0x94, 0x94, 0x64, //5
									0x78, 0x94, 0x94, 0x64, //6
									0x00, 0x04, 0xF4, 0x0C, //7
									0x6C, 0x94, 0x94, 0x6C, //8
									0x98, 0xA4, 0xA4, 0x78, //9
									0x00, 0x80, 0x00, 0x00  //.
									};
__flash const uint8_t Font7_11[] = {0x80, 0xC0, 0x60, 0x20, 0x60, 0xC0, 0x80, 0x3F, 0x7F, 0xC0, 0x80, 0xC0, 0x7F, 0x3F,  //0
									0x00, 0x20, 0x20, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x80, 0x80, 0xFF, 0xFF, 0x80, 0x80,  //1
									0x80, 0xC0, 0x60, 0x60, 0xE0, 0xC0, 0x00, 0x80, 0xC0, 0xE0, 0xD8, 0xCF, 0xC3, 0xE0,  //2
									0x80, 0x40, 0x60, 0x60, 0xE0, 0xC0, 0x00, 0xC0, 0xC0, 0x84, 0x86, 0x8F, 0x7D, 0x38,  //3
									0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x00, 0x38, 0x34, 0x32, 0x31, 0xFF, 0xFF, 0x30,  //4
									0x00, 0xC0, 0x60, 0x60, 0x60, 0x60, 0x20, 0xC2, 0xC3, 0xC3, 0x83, 0x86, 0x4E, 0x3C,  //5
									0x00, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x20, 0x3E, 0x7F, 0xC3, 0x82, 0xC6, 0x7C, 0x38,  //6
									0xC0, 0x60, 0x60, 0x60, 0x60, 0xE0, 0x20, 0x00, 0x00, 0x80, 0x70, 0x0E, 0x01, 0x00,  //7
									0xC0, 0xE0, 0x20, 0x20, 0x20, 0xE0, 0xC0, 0x71, 0XFB, 0x87, 0x86, 0x8E, 0xFF, 0x79,  //8
									0x80, 0xC0, 0x60, 0x20, 0x60, 0xC0, 0x80, 0x83, 0x87, 0xCC, 0x48, 0x78, 0x3F, 0x0F,  //9
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00,  //.
									 };
__flash const uint8_t Font5_8[] = { 0x78, 0x86, 0x81, 0x61, 0x1E, //0
									0x00, 0x04, 0xE2, 0x1F, 0x00, //1
									0xC0, 0xA2, 0x91, 0x89, 0x06, //2
									0x40, 0x82, 0x89, 0x89, 0x76,//3
									0x30, 0x28, 0xE4, 0x3E, 0x23,//4
									0x4C, 0x8B, 0x85, 0x45, 0x39,//5
									0x7C, 0x92, 0x89, 0x49, 0x32,//6
									0x00, 0xC1, 0x31, 0x0D, 0x03,//7
									0x60, 0x96, 0x89, 0x89, 0x76,//8
									0x4C, 0x92, 0x91, 0x49, 0x3E,//9
									0x00, 0xC0, 0xC0, 0x00, 0x00, //.
									0x00, 0x10, 0x10, 0x10, 0x00 //-
									 };								 
__flash const uint8_t Font11_16[] = {	0x00, 0xE0, 0xF8, 0xFC, 0x3E, 0x07, 0x03, 0x01, 0xE1, 0xFE, 0xFC, 0x3F, 0x7F, 0x87, 0x80, 0x80, 0xE0, 0x7C, 0x3F, 0x1F, 0x07, 0x00,  //0
										0x00, 0x00, 0x00, 0x00, 0x02, 0xE2, 0xFE, 0xFF, 0x1F, 0x03, 0x00, 0x80, 0x80, 0xC0, 0xF8, 0xFF, 0xFF, 0x87, 0x80, 0x00, 0x00, 0x00,  //1
										0x00, 0x00, 0x00, 0x0C, 0x06, 0x03, 0x03, 0x87, 0xFF, 0x7E, 0x1C, 0x80, 0xC0, 0xE0, 0xF0, 0xE8, 0xE4, 0xE3, 0xE1, 0xF0, 0x18, 0x00,  //2
										0x00, 0x00, 0x00, 0x80, 0x84, 0x82, 0xC3, 0xC3, 0x7F, 0x3F, 0x1E, 0x60, 0xE0, 0xC0, 0xC0, 0x80, 0x81, 0x43, 0x7F, 0x3F, 0x1E, 0x00,  //3
										0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0xF0, 0xF8, 0xFC, 0x1E, 0x10, 0x1C, 0x1A, 0x19, 0x18, 0xF8, 0xFE, 0xFF, 0x1F, 0x19, 0x08,  //4
										0x00, 0x00, 0x40, 0x78, 0xF6, 0xE7, 0xE7, 0xC7, 0x87, 0x07, 0x03, 0x60, 0xE0, 0xC0, 0x80, 0x80, 0x41, 0x7F, 0x3F, 0x0F, 0x00, 0x00,  //5
										0x00, 0xC0, 0xE0, 0xF8, 0x78, 0x5C, 0x4E, 0xC6, 0xC3, 0x81, 0x01, 0x3F, 0x7F, 0xFF, 0x81, 0x80, 0xC0, 0xF0, 0x7F, 0x3F, 0x0F, 0x00,  //6
										0x08, 0x0E, 0x07, 0x07, 0x07, 0x87, 0x67, 0x1F, 0x07, 0x01, 0x00, 0x80, 0x60, 0x10, 0x0C, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //7
										0x00, 0x00, 0x3C, 0x7E, 0xFF, 0xE3, 0xC1, 0xC1, 0x7F, 0x3E, 0x1C, 0x3C, 0x7E, 0xFF, 0x83, 0x80, 0x81, 0xC3, 0x7F, 0x7F, 0x1E, 0x00,  //8
										0x00, 0xF0, 0xFC, 0xFE, 0x0F, 0x03, 0x01, 0xC1, 0xFF, 0xFE, 0xFC, 0x80, 0xC1, 0xC3, 0x63, 0x72, 0x3A, 0x1E, 0x0F, 0x07, 0x03, 0x00,  //9
										0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,  //:
									};
__flash const uint8_t JPEG[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFF, 0xFF, 0xFF, 0x02, 0x02, 0x02, 0x0E, 0x1E, 0x3E, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x17, 0xB7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xB7, 0x17, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3F, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0x3F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};									
*//*----------Инициализация--------------------*/
void LCD_12864_ini(void)
{
	_delay_ms(40);
	LCD_12864_send_command(0xC);//display on
	_delay_us(100);
	clear_LCD_12864();
	LCD_12864_send_command(6);//курсор сдвигается вправо
	_delay_ms(10);
}
/*----------Включение графического режима--------------------*/
void LCD_12864_GrapnicMode(uint8_t enable)   // 1-enable, 0-disable
{
	if (enable) 
	{
		LCD_12864_send_command(0x34); //Переходим в extended instruction
		_delay_ms(1);	
	    LCD_12864_send_command(0x36); //вкл графич. режим
		_delay_ms(1);	
	}
	else
	{
		LCD_12864_send_command(0x30); //Переходим в basic instruction
		_delay_ms(1);
	}
}
/*----------Вывод изображения на экран дисплея--------------------*/
void LCD_12864_Draw_bitmap(const unsigned char *bitmap) {
	/// Функция вывода изображения на дисплей
	/// Работает с памятью ST7920.
	/// \param *bitmap - изображение 128*64. т.е. Буфер из 1024 элементов.
	uint8_t x, y;
	uint16_t i = 0;
	uint8_t Temp, Db;

	for (y = 0; y < 64; y++) {
		for (x = 0; x < 8; x++) {
			if (y < 32) {
				LCD_12864_send_command(0x80 | y);				//y(0-31)
				LCD_12864_send_command(0x80 | x);				//x(0-8)
				} else {
				LCD_12864_send_command(0x80 | (y - 32));		//y(0-31)
				LCD_12864_send_command(0x88 | x);				//x(0-8)
			}

			i = ((y / 8) * 128) + (x * 16);
			Db = y % 8;

			Temp = (((bitmap[i] >> Db) & 0x01) << 7) | (((bitmap[i + 1] >> Db) & 0x01) << 6) | (((bitmap[i + 2] >> Db) & 0x01) << 5)
			| (((bitmap[i + 3] >> Db) & 0x01) << 4) | (((bitmap[i + 4] >> Db) & 0x01) << 3) | (((bitmap[i + 5] >> Db) & 0x01) << 2)
			| (((bitmap[i + 6] >> Db) & 0x01) << 1) | (((bitmap[i + 7] >> Db) & 0x01) << 0);
			LCD_12864_send_data(Temp);

			Temp = (((bitmap[i + 8] >> Db) & 0x01) << 7) | (((bitmap[i + 9] >> Db) & 0x01) << 6) | (((bitmap[i + 10] >> Db) & 0x01) << 5)
			| (((bitmap[i + 11] >> Db) & 0x01) << 4) | (((bitmap[i + 12] >> Db) & 0x01) << 3) | (((bitmap[i + 13] >> Db) & 0x01) << 2)
			| (((bitmap[i + 14] >> Db) & 0x01) << 1) | (((bitmap[i + 15] >> Db) & 0x01) << 0);

			LCD_12864_send_data(Temp);
		}
	}
}
/*----------Рисование пикселя--------------------*/
void LCD_12864_Draw_pixel(uint8_t x, uint8_t y, uint8_t *Frame_buffer) {
	/// Функция рисования точки.
	/// param\ x - координата по X(от 0 до 127)
	/// paran\ y - координата по Y(от 0 до 63)
	if ((y < ST7920_height) && (x < ST7920_width)) {
		Frame_buffer[(x) + ((y / 8) * 128)] |= 0x01 << y % 8;
	}
}
/*---------------------Удаление пикселя на экране----------------------------*/
void LCD_12864_Clean_pixel(uint8_t x, uint8_t y, uint8_t *Frame_buffer) {
	/// Функция удаления точки.
	/// param\ x - координата по X(от 0 до 127) 
	/// paran\ y - координата по Y(от 0 до 63)
	if (y < ST7920_height && x < ST7920_width) {
		Frame_buffer[(x) + ((y / 8) * 128)] &= 0xFE << y % 8;
	}
}
/*----------Очистка--------------------*/
void clear_LCD_12864(void)
{
	LCD_12864_send_command(0x1);//clear
	_delay_ms(10);
}
/*----------Позиционирование символьного текста--------------------*/
void setpos_LCD_12864(unsigned char x, unsigned y)
{
	uint8_t command;
	switch (y)
	{
		case 0: command = 0x80; break;
		case 1: command = 0x90; break;
		case 2: command = 0x88; break;
		case 3: command = 0x98; break;
	}
	command = command | x;
	LCD_12864_send_command(command);
}
/*----------Вывод символьного текста строкой--------------------*/
void str_LCD_12864 (char *str1)
{
	wchar_t n;
	for(n=0;str1[n]!='\0';n++)
	LCD_12864_send_data(str1[n]);
}
/*----------Отправка данных--------------------*/
void LCD_12864_send_data(uint8_t data)
{
	PORTD |= (1<<RS);
    //передача 5 единиц
	PORTD |= (1<<RW);
	strob();
	strob();
	strob();
	strob();
	strob();
	PORTD &= ~(1<<RW);//r/w=0
	strob();
	PORTD |= (1<<RW);//rs=1
	strob();
	PORTD &= ~(1<<RW);
	strob();
	for (int k = 0; k < 12; k++)
	{
		if ((k == 4)||(k == 5)||(k == 6)||(k == 7))
		{
			PORTD &= ~(1<<RW);
			strob();
		}
		else
		{
			switch(data & 0x80)
			{
				case 0x80: PORTD |= (1<<RW);
						strob();
						break;
				case 0: PORTD &= ~(1<<RW);
						strob();	
						break;
			}
			data = data << 1;
		}
	}
	//передача 4 нулей
	PORTD &= ~(1<<RW);
	strob();
	strob();
	strob();
	strob();
	
	PORTD &= ~(1<<RS);
}
/*----------Отправка команды--------------------*/
void LCD_12864_send_command(uint8_t command)
{
	PORTD |= (1<<RS);
    //передача 5 единиц
	PORTD |= (1<<RW);
	strob();
	strob();
	strob();
	strob();
	strob();
	PORTD &= ~(1<<RW);//r/w=0
	strob();
	PORTD &= ~(1<<RW);//rs=0
	strob();
	PORTD &= ~(1<<RW);
	strob();
	for (int k = 0; k < 12; k++)
	{
		if ((k == 4)||(k == 5)||(k == 6)||(k == 7))
		{
			PORTD &= ~(1<<RW);
			strob();
		}
		else
		{
			switch(command & 0x80)
			{
				case 0x80: PORTD |= (1<<RW);
				strob();
				break;
				case 0: PORTD &= ~(1<<RW);
				strob();
				break;
			}
			command = command << 1;
		}
	}
	//передача 4 нулей
	PORTD &= ~(1<<RW);
	strob();
	strob();
	strob();
	strob();
	
	PORTD &= ~(1<<RS);
}
/*----------Тактовый импульс--------------------*/
void strob(void)
{
	_delay_us(1);
	PORTD |= (1<<E);
	_delay_us(1);
	PORTD &= ~(1<<E);
	_delay_us(1);
}
/*---------------------Функция вывода символа 5*7 на дисплей-----------------------------*/
void LCD_12864_print_symbol(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer) {
	/// Функция вывода символа на дисплей
	/// \param x положение по х (от 0 до 1023)
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	/// \param symbol - код символа
	/// \param inversion - инверсия. 1 - вкл, 0 - выкл.
	for (int i = 0; i <= 6; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font5_7[(symbol * 6) + i];//read_symbol_from_SD ((symbol * 6) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font5_7[(symbol * 6) + i];//read_symbol_from_SD ((symbol * 6) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
}
/*---------------------Функция вывода символа 4*6 на дисплей-----------------------------*/
void LCD_12864_print_symbol_4_6(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer) {
	/// Функция вывода символа на дисплей
	/// \param x положение по х (от 0 до 1023)
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	/// \param symbol - код символа
	/// \param inversion - инверсия. 1 - вкл, 0 - выкл.
	for (int i = 0; i <= 3; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font4_6[(symbol * 4) + i];//read_symbol_from_SD ((symbol * 4) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font4_6[(symbol * 4) + i];//read_symbol_from_SD ((symbol * 4) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
}
/*---------------------Функция вывода символа 5*8 на дисплей-----------------------------*/
void LCD_12864_print_symbol_5_8(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer) {
	/// Функция вывода символа на дисплей
	/// \param x положение по х (от 0 до 1023)
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	/// \param symbol - код символа
	/// \param inversion - инверсия. 1 - вкл, 0 - выкл.
	for (int i = 0; i <= 4; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font5_8[(symbol * 5) + i];//read_symbol_from_SD ((symbol * 4) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font5_8[(symbol * 5) + i];//read_symbol_from_SD ((symbol * 4) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
}
/*---------------------Функция вывода символа 11*16 для отображения времени-----------------------------*/
void LCD_12864_print_symbol_11_16(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer) {
	/// Функция вывода символа на дисплей
	/// \param x положение по х (от 0 до 1023)
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	/// \param symbol - код символа
	/// \param inversion - инверсия. 1 - вкл, 0 - выкл.
	for (int i = 0; i <= 10; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font11_16[(symbol * 22) + i];//read_symbol_from_SD ((symbol * 22) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font11_16[(symbol * 22) + i];//read_symbol_from_SD ((symbol * 22) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
	x += 117;
	for (int i = 11; i <= 21; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font11_16[(symbol * 22) + i];//read_symbol_from_SD ((symbol * 22) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font11_16[(symbol * 22) + i];//read_symbol_from_SD ((symbol * 22) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
}
/*---------------------Функция вывода символа 7*11 -----------------------------*/
void LCD_12864_print_symbol_7_11(uint16_t x, uint16_t symbol, uint8_t inversion, uint8_t *Frame_buffer) {
	/// Функция вывода символа на дисплей
	/// \param x положение по х (от 0 до 1023)
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	/// \param symbol - код символа
	/// \param inversion - инверсия. 1 - вкл, 0 - выкл.
	for (int i = 0; i <= 6; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font7_11[(symbol * 14) + i];//read_symbol_from_SD ((symbol * 14) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font7_11[(symbol * 14) + i];//read_symbol_from_SD ((symbol * 14) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
	x += 121;
	for (int i = 7; i <= 13; i++) {
		if (inversion) {
			Frame_buffer[i + x - 1] = ~Font7_11[(symbol * 14) + i];//read_symbol_from_SD ((symbol * 14) + i);
			//Frame_buffer[i + x - 1] = ~Font[(symbol * 6) + i];
			} else {
			Frame_buffer[i + x - 1] = Font7_11[(symbol * 14) + i];//read_symbol_from_SD ((symbol * 14) + i);
			//Frame_buffer[i + x - 1] = Font[(symbol * 6) + i];
		}
	}
}
/*---------------------Функция вывода строки 7*11-----------------------------*/
void LCD_12864_Print_7_11(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer) {
	/// Функция декодирования UTF-8 в набор символов и последующее занесение в буфер кадра
	/// \param x - координата по х. От 0 до 127
	/// \param y - координата по y. от 0 до 7
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/7_11.txt");*/
	
	x = x + y * 128;
	uint16_t symbol = 0;
	for (int i = 0; i < strlen(tx_buffer); i++)
	{
		symbol = tx_buffer[i];
		if (inversion)
		{
			if(symbol != 46) LCD_12864_print_symbol_7_11(x, symbol - 48, 1, Frame_buffer);
			else LCD_12864_print_symbol_7_11(x, 10, 1, Frame_buffer);
		}
		else
		{
			if(symbol != 46) LCD_12864_print_symbol_7_11(x, symbol - 48, 0, Frame_buffer);
			else LCD_12864_print_symbol_7_11(x, 10, 0, Frame_buffer);
		}
		x = x + 8;
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
/*---------------------Функция вывода строки 4*6-----------------------------*/
void LCD_12864_Print_4_6(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer) {
	/// Функция декодирования UTF-8 в набор символов и последующее занесение в буфер кадра
	/// \param x - координата по х. От 0 до 127
	/// \param y - координата по y. от 0 до 7
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/4_6.txt");*/
	
	x = x + y * 128;
	uint16_t symbol = 0;
	for (int i = 0; i < strlen(tx_buffer); i++)
	{
		symbol = tx_buffer[i];
		if (inversion)
		{
			if(symbol != 46) LCD_12864_print_symbol_4_6(x, symbol - 48, 1, Frame_buffer);
			else LCD_12864_print_symbol_4_6(x, 10, 1, Frame_buffer);
		}
		else
		{
			if(symbol != 46) LCD_12864_print_symbol_4_6(x, symbol - 48, 0, Frame_buffer);
			else LCD_12864_print_symbol_4_6(x, 10, 0, Frame_buffer);
		}
		x = x + 5;
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
/*---------------------Функция вывода строки 5*8-----------------------------*/
void LCD_12864_Print_5_8(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer) {
	/// Функция декодирования UTF-8 в набор символов и последующее занесение в буфер кадра
	/// \param x - координата по х. От 0 до 127
	/// \param y - координата по y. от 0 до 7
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/4_6.txt");*/
	
	x = x + y * 128;
	uint16_t symbol = 0;
	for (int i = 0; i < strlen(tx_buffer); i++)
	{
		symbol = tx_buffer[i];
		if (inversion)
		{
			if(symbol == 46) LCD_12864_print_symbol_5_8(x, 10, 1, Frame_buffer);
			else if (symbol == 45) LCD_12864_print_symbol_5_8(x, 11, 1, Frame_buffer);
			else LCD_12864_print_symbol_5_8(x, symbol - 48, 1, Frame_buffer);
		}
		else
		{
			if(symbol == 46) LCD_12864_print_symbol_5_8(x, 10, 0, Frame_buffer);
			else if (symbol == 45) LCD_12864_print_symbol_5_8(x, 11, 0, Frame_buffer);
			else LCD_12864_print_symbol_5_8(x, symbol - 48, 0, Frame_buffer);
		}
		x = x + 6;
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
/*---------------------Функция вывода строки 11*16-----------------------------*/
void LCD_12864_Print_11_16(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer) {
	/// Функция декодирования UTF-8 в набор символов и последующее занесение в буфер кадра
	/// \param x - координата по х. От 0 до 127
	/// \param y - координата по y. от 0 до 7
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/Cloc.txt");*/
	
	x = x + y * 128;
	uint16_t symbol = 0;
	for (int i = 0; i < strlen(tx_buffer); i++)
	{
			symbol = tx_buffer[i];
			if (inversion)
			{
				LCD_12864_print_symbol_11_16(x, symbol - 48, 1, Frame_buffer); 
			}
			else
			{
				LCD_12864_print_symbol_11_16(x, symbol - 48, 0, Frame_buffer); 
			}
			x = x + 12;
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
/*----------------Функция декодирования UTF-8 в набор символов 5*7-----------------*/
void LCD_12864_Decode_UTF8(uint16_t x, uint8_t y, uint8_t inversion, char *tx_buffer, uint8_t *Frame_buffer) {
	/// Функция декодирования UTF-8 в набор символов и последующее занесение в буфер кадра
	/// \param x - координата по х. От 0 до 127
	/// \param y - координата по y. от 0 до 7
	/// Дисплей поделен по страницам(строка из 8 пикселей)
	/// 1 строка: x = 0;
	///	2 строка: x = 128;
	/// 3 строка: x = 256;
	/// 4 строка: x = 384;
	/// 5 строка: x = 512;
	/// 6 строка: x = 640;
	/// 7 строка: x = 786;
	/// 8 строка: x = 896;
	
	//Начинаем работу с файловой системой для считывания массива шрифтов
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/Font.txt");*/
	
	x = x + y * 128;
	uint16_t symbol = 0;
	for (int i = 0; i < strlen(tx_buffer); i++) 
	{
		if ((tx_buffer[i] < 192)&&(tx_buffer[i]!=168)&&(tx_buffer[i]!=184)&&(tx_buffer[i]!=176)) 
		{   //Английский текст и символы. Если до русского текста, то [i] <0xD0. Но в font добавлен знак "°" 
			symbol = tx_buffer[i];
			if (inversion) 
			{
				LCD_12864_print_symbol(x, symbol - 32, 1, Frame_buffer); //Таблица UTF-8. Basic Latin. С "пробел" до "z". Инверсия вкл.
			} 
			else 
			{
				LCD_12864_print_symbol(x, symbol - 32, 0, Frame_buffer); //Таблица UTF-8. Basic Latin. С "пробел" до "z". Инверсия выкл.
			}
			x = x + 6;			
		}
		else 
		{ //Русский текст
			symbol = tx_buffer[i];
			if ((symbol <= 255) && (symbol >= 192)) 
			{
				if (inversion) 
				{
					LCD_12864_print_symbol(x, symbol - 97, 1, Frame_buffer); //Таблица UTF-8. Кириллица. С буквы "А" до "п". Инверсия вкл.
				} 
				else 
				{
					LCD_12864_print_symbol(x, symbol - 97, 0, Frame_buffer); //Таблица UTF-8. Кириллица. С буквы "А" до "п". Инверсия выкл.
				}
				x = x + 6;
			} 			
			if (symbol == 168) 
			{
				if (inversion) 
				{
					LCD_12864_print_symbol(x, 159, 1, Frame_buffer); ////Таблица UTF-8. Кириллица. Буква "Ё". Инверсия вкл.
				} 
				else 
				{
					LCD_12864_print_symbol(x, 159, 0, Frame_buffer); ////Таблица UTF-8. Кириллица. Буква "Ё". Инверсия выкл.
				}
				x = x + 6;
			} 
		    if (symbol == 184) 
			{
				if (inversion) 
				{
					LCD_12864_print_symbol(x, 160, 1, Frame_buffer); ////Таблица UTF-8. Кириллица. Буква "ё". Инверсия вкл.
				} else 
				{
					LCD_12864_print_symbol(x, 160, 0, Frame_buffer); ////Таблица UTF-8. Кириллица. Буква "ё". Инверсия выкл.
				}
				x = x + 6;
			} 
			if (symbol == 176) 
			{
				if (inversion)
			    {
					LCD_12864_print_symbol(x, 161, 1, Frame_buffer); ////Таблица UTF-8. Basic Latin. Символ "°". Инверсия вкл.
				} 
				else 
				{
					LCD_12864_print_symbol(x, 161, 0, Frame_buffer); ////Таблица UTF-8. Basic Latin. Символ "°". Инверсия выкл.
				}
				x = x + 6;
			}

		}
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
/*---------------------Функция вывода Jpg картинки------------------*/
void LCD_12864_Print_jpeg(uint8_t *Frame_buffer)
{
	/*FATFS fs;
	asm("nop");
	pf_mount(&fs); //Монтируем FAT
	pf_open("/jpg2.txt");*/
	int count = 0;
	for(int k = 0; k <= 7; k++)
	{
	 for (int i = 31 + 128*k; i < 95 + 128*k; i++) 
	 {
		Frame_buffer[i] = JPEG[count];//read_symbol_from_SD (count);
		count++;
	 }
	}
	// Завершаем работу с файлом
	//pf_mount(0x00);
}
/*---------------------Функция инверсии любого места в буфере------------------*/
/*void LCD_12864_Inversion(uint16_t x_start, uint16_t x_end) {
	/// Функция инверсии любого места в буфере
	/// \param x_start - начальная точка по х от 0 до 1024
	/// \param x_end - конечная точка по y от 0 до 1024
	for (; x_start < x_end; x_start++) {
		Frame_buffer[x_start] = ~(Frame_buffer[x_start]);
	}
}*/
/*---------------Функция очистки дисплея в графическом режиме--------------------*/
void LCD_12864_Clean(uint8_t *Frame_buffer) {
	/// Функция очистки дисплея в графическом режиме
	uint8_t x, y;
	for (y = 0; y < 64; y++) {
		if (y < 32) {
			LCD_12864_send_command(0x80 | y);
			LCD_12864_send_command(0x80);
			} else {
			LCD_12864_send_command(0x80 | (y - 32));
			LCD_12864_send_command(0x88);
		}
		for (x = 0; x < 8; x++) {
			LCD_12864_send_data(0x00);
			LCD_12864_send_data(0x00);
		}
	}
	LCD_12864_Clean_Frame_buffer(Frame_buffer);
}
/*------------------------Функция очистки буфера кадра-------------------------*/
void LCD_12864_Clean_Frame_buffer(uint8_t *Frame_buffer) {
	/// Функция очистки буфера кадра
	memset(Frame_buffer, 0x00, sizeof(Frame_buffer));
}
/*----------Различная графика--------------------*/
/*-----------------------------------------------*/
/*----------Линия--------------------*/
void LCD_12864_Draw_line(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, uint8_t *Frame_buffer) {
	int dx = (x_end >= x_start) ? x_end - x_start : x_start - x_end;
	int dy = (y_end >= y_start) ? y_end - y_start : y_start - y_end;
	int sx = (x_start < x_end) ? 1 : -1;
	int sy = (y_start < y_end) ? 1 : -1;
	int err = dx - dy;

	for (;;) {
		LCD_12864_Draw_pixel(x_start, y_start, Frame_buffer);
		if (x_start == x_end && y_start == y_end)
		break;
		int e2 = err + err;
		if (e2 > -dy) {
			err -= dy;
			x_start += sx;
		}
		if (e2 < dx) {
			err += dx;
			y_start += sy;
		}
	}
}
/*----------Пустотелый прямоугольник--------------------*/
void LCD_12864_Draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *Frame_buffer) {
	/// Вывести пустотелый прямоугольник
	/// \param x - начальная точка по оси "x"
	/// \param y - начальная точка по оси "y"
	/// \param width - ширина прямоугольника
	/// \param height - высота прямоугольника

	/*Проверка ширины и высоты*/
	if ((x + width) >= ST7920_width) {
		width = ST7920_width - x;
	}
	if ((y + height) >= ST7920_height) {
		height = ST7920_height - y;
	}

	/*Рисуем линии*/
	LCD_12864_Draw_line(x, y, x + width, y, Frame_buffer); /*Верх прямоугольника*/
	LCD_12864_Draw_line(x, y + height, x + width, y + height, Frame_buffer); /*Низ прямоугольника*/
	LCD_12864_Draw_line(x, y, x, y + height, Frame_buffer); /*Левая сторона прямоугольника*/
	LCD_12864_Draw_line(x + width, y, x + width, y + height, Frame_buffer); /*Правая сторона прямоугольника*/
}
/*-------------------------------Закрашенный прямоугольник---------------------------------*/
void LCD_12864_Draw_rectangle_filled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *Frame_buffer) {
	/// Вывести закрашенный прямоугольник
	/// \param x - начальная точка по оси "x"
	/// \param y - начальная точка по оси "y"
	/// \param width - ширина прямоугольника
	/// \param height - высота прямоугольника

	/*Проверка ширины и высоты*/
	if ((x + width) >= ST7920_width) {
		width = ST7920_width - x;
	}
	if ((y + height) >= ST7920_height) {
		height = ST7920_height - y;
	}

	/*Рисуем линии*/
	for (uint8_t i = 0; i <= height; i++) {
		LCD_12864_Draw_line(x, y + i, x + width, y + i, Frame_buffer);
	}
}
/*---------------------------------Пустотелая окружность-----------------------------------*/
void LCD_12864_Draw_circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t *Frame_buffer) {
	/// Вывести пустотелую окружность
	/// \param x - точка центра окружности по оси "x"
	/// \param y - точка центра окружности по оси "y"
	/// \param radius - радиус окружности

	int f = 1 - (int) radius;
	int ddF_x = 1;

	int ddF_y = -2 * (int) radius;
	int x_0 = 0;

	LCD_12864_Draw_pixel(x, y + radius, Frame_buffer);
	LCD_12864_Draw_pixel(x, y - radius, Frame_buffer);
	LCD_12864_Draw_pixel(x + radius, y, Frame_buffer);
	LCD_12864_Draw_pixel(x - radius, y, Frame_buffer);

	int y_0 = radius;
	while (x_0 < y_0) {
		if (f >= 0) {
			y_0--;
			ddF_y += 2;
			f += ddF_y;
		}
		x_0++;
		ddF_x += 2;
		f += ddF_x;
		LCD_12864_Draw_pixel(x + x_0, y + y_0, Frame_buffer);
		LCD_12864_Draw_pixel(x - x_0, y + y_0, Frame_buffer);
		LCD_12864_Draw_pixel(x + x_0, y - y_0, Frame_buffer);
		LCD_12864_Draw_pixel(x - x_0, y - y_0, Frame_buffer);
		LCD_12864_Draw_pixel(x + y_0, y + x_0, Frame_buffer);
		LCD_12864_Draw_pixel(x - y_0, y + x_0, Frame_buffer);
		LCD_12864_Draw_pixel(x + y_0, y - x_0, Frame_buffer);
		LCD_12864_Draw_pixel(x - y_0, y - x_0, Frame_buffer);
	}
}
/*--------------------------------Закрашенная окружность-----------------------------------*/
void LCD_12864_Draw_circle_filled(int16_t x, int16_t y, int16_t radius, uint8_t *Frame_buffer) {
	/// Вывести закрашенную окружность
	/// \param x - точка центра окружности по оси "x"
	/// \param y - точка центра окружности по оси "y"
	/// \param radius - радиус окружности

	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x_0 = 0;
	int16_t y_0 = radius;

	LCD_12864_Draw_pixel(x, y + radius, Frame_buffer);
	LCD_12864_Draw_pixel(x, y - radius, Frame_buffer);
	LCD_12864_Draw_pixel(x + radius, y, Frame_buffer);
	LCD_12864_Draw_pixel(x - radius, y, Frame_buffer);
	LCD_12864_Draw_line(x - radius, y, x + radius, y, Frame_buffer);

	while (x_0 < y_0) {
		if (f >= 0) {
			y_0--;
			ddF_y += 2;
			f += ddF_y;
		}
		x_0++;
		ddF_x += 2;
		f += ddF_x;

		LCD_12864_Draw_line(x - x_0, y + y_0, x + x_0, y + y_0, Frame_buffer);
		LCD_12864_Draw_line(x + x_0, y - y_0, x - x_0, y - y_0, Frame_buffer);
		LCD_12864_Draw_line(x + y_0, y + x_0, x - y_0, y + x_0, Frame_buffer);
		LCD_12864_Draw_line(x + y_0, y - x_0, x - y_0, y - x_0, Frame_buffer);
	}
}
/*----------------------------------Закрашенный треугольник--------------------------------*/
void LCD_12864_Draw_triangle_filled(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t *Frame_buffer) {
	/// Вывести закрашенный треугольник
	/// \param x_1 - первая точка треугольника. Координата по оси "x"
	/// \param y_1 - первая точка треугольника. Координата по оси "y"
	/// \param x_2 - вторая точка треугольника. Координата по оси "x"
	/// \param y_2 - вторая точка треугольника. Координата по оси "y"
	/// \param x_3 - третья точка треугольника. Координата по оси "x"
	/// \param y_3 - третья точка треугольника. Координата по оси "y"

	#define ABS(x)   ((x) > 0 ? (x) : -(x))
	int16_t deltax = 0;
	int16_t deltay = 0;
	int16_t x = 0;
	int16_t y = 0;
	int16_t xinc1 = 0;
	int16_t xinc2 = 0;
	int16_t yinc1 = 0;
	int16_t yinc2 = 0;
	int16_t den = 0;
	int16_t num = 0;
	int16_t numadd = 0;
	int16_t numpixels = 0;
	int16_t curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
		} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
		} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
		} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		LCD_12864_Draw_line(x, y, x3, y3, Frame_buffer);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}