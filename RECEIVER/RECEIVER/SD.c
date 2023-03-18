/*
 * SD.c
 *
 * Created: 30.05.2022 23:46:22
 *  Author: Alex2
 */ 
#include "SD.h"

/*
void SPI_SendByte (unsigned char byte)
{
	unsigned char i;
	for (i=0;i<8;i++) //движемся по битам байта
	{
		if ((byte&0x80)==0x00)//проверяем левый бит
		PORTC&=~(1<<MOSI_SD); //если 0, то выставляем 0 на шине
		else PORTC|=(1<<MOSI_SD); //если 1, то выставляем 1
		byte<<=1; //сдвигаем влево, в сторону старшего для проверки следующего бита
		PORTC|=(1<<SCK_SD); //фронт на ножке SCK
		asm("nop"); //1 такт подождём
		PORTC&=~(1<<SCK_SD); //спад на ножке SCK
	}
}
//—————————————-
unsigned char SPI_ReceiveByte(void)
{
	unsigned char i, result=0;
	for(i=0;i<8;i++)
	{
		PORTC|=(1<<SCK_SD);//фронт на лапке SCK
		result<<=1;//сдвигаем влево байт, чтобы записать очередной бит
		if((PINC&(1<<MISO_SD))!=0x00)//запишем новый бит в младший разряд
		result=result|0x01;//запишем считанный с лапки порта MISO бит
		PORTC&=~(1<<SCK_SD);//спад на лапке SCK
		asm("nop");//1 такт подождём
	}
	return result;//вернем результат
}
//—————————————-
unsigned char SD_cmd (char dt0,char dt1,char dt2,char dt3,char dt4,char dt5)
{
	unsigned char result;
	long int cnt;
	SPI_SendByte(dt0); //индекс
	SPI_SendByte(dt1); //Аргумент
	SPI_SendByte(dt2);
	SPI_SendByte(dt3);
	SPI_SendByte(dt4);
	SPI_SendByte(dt5); //контрольная сумма
	cnt=0;
	do
	{ //Ждём ответ в формате R1 (даташит стр 109)
		result=SPI_ReceiveByte();
		cnt++;
	} while (((result&0x80)!=0x00)&&cnt<0xFFFF);
	return result;
} 
//—————————————-
unsigned char SD_Init(void)
{
	unsigned char i,temp;
	long int cnt;
	_delay_ms(1);
	for(i=0;i<10;i++) //80 импульсов (не менее 74) Даташит стр 91
	SPI_SendByte(0xFF);
	PORTB&=~(1<<SS_SD);//опускаем SS
	temp=SD_cmd(0x40,0x00,0x00,0x00,0x00,0x95); //CMD0 Даташит стр 102 и 96
	if(temp!=0x01) return 1; //Выйти если ответ не 0x01
	SPI_SendByte(0xFF);
	temp=SD_cmd(0x48,0x00,0x00,0x01,0xAA,0x87); //CMD8 передаем также, меняется только индекс
	if(temp!=0x01) return 1; //Выйти если ответ не 0x01
	SPI_SendByte(0xFF);
	cnt=0;
	 do
	 {
		 temp=SD_cmd(0x41,0x00,0x00,0x00,0x00,0x95); //CMD1 передаем также, меняется только индекс
		 SPI_SendByte(0xFF);
		 cnt++;
	 } while ((temp!=0x00)&&cnt<0xFFFF); //Ждёс ответа R1
	 if(cnt>=0xFFFF) return 2;
	 return 0;
}
//—————————————-
*/

uint8_t read_symbol_from_SD (uint16_t symbol)
{
	WORD s1;
	uint8_t result;
	char buffer[10] = {};

	pf_lseek(6*symbol); //Установим курсор чтения на 0 
	pf_read(buffer,4,&s1);
	sscanf(buffer, "%x", &result);

	return result;
} 