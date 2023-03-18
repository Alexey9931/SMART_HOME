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
	for (i=0;i<8;i++) //�������� �� ����� �����
	{
		if ((byte&0x80)==0x00)//��������� ����� ���
		PORTC&=~(1<<MOSI_SD); //���� 0, �� ���������� 0 �� ����
		else PORTC|=(1<<MOSI_SD); //���� 1, �� ���������� 1
		byte<<=1; //�������� �����, � ������� �������� ��� �������� ���������� ����
		PORTC|=(1<<SCK_SD); //����� �� ����� SCK
		asm("nop"); //1 ���� �������
		PORTC&=~(1<<SCK_SD); //���� �� ����� SCK
	}
}
//�������������-
unsigned char SPI_ReceiveByte(void)
{
	unsigned char i, result=0;
	for(i=0;i<8;i++)
	{
		PORTC|=(1<<SCK_SD);//����� �� ����� SCK
		result<<=1;//�������� ����� ����, ����� �������� ��������� ���
		if((PINC&(1<<MISO_SD))!=0x00)//������� ����� ��� � ������� ������
		result=result|0x01;//������� ��������� � ����� ����� MISO ���
		PORTC&=~(1<<SCK_SD);//���� �� ����� SCK
		asm("nop");//1 ���� �������
	}
	return result;//������ ���������
}
//�������������-
unsigned char SD_cmd (char dt0,char dt1,char dt2,char dt3,char dt4,char dt5)
{
	unsigned char result;
	long int cnt;
	SPI_SendByte(dt0); //������
	SPI_SendByte(dt1); //��������
	SPI_SendByte(dt2);
	SPI_SendByte(dt3);
	SPI_SendByte(dt4);
	SPI_SendByte(dt5); //����������� �����
	cnt=0;
	do
	{ //��� ����� � ������� R1 (������� ��� 109)
		result=SPI_ReceiveByte();
		cnt++;
	} while (((result&0x80)!=0x00)&&cnt<0xFFFF);
	return result;
} 
//�������������-
unsigned char SD_Init(void)
{
	unsigned char i,temp;
	long int cnt;
	_delay_ms(1);
	for(i=0;i<10;i++) //80 ��������� (�� ����� 74) ������� ��� 91
	SPI_SendByte(0xFF);
	PORTB&=~(1<<SS_SD);//�������� SS
	temp=SD_cmd(0x40,0x00,0x00,0x00,0x00,0x95); //CMD0 ������� ��� 102 � 96
	if(temp!=0x01) return 1; //����� ���� ����� �� 0x01
	SPI_SendByte(0xFF);
	temp=SD_cmd(0x48,0x00,0x00,0x01,0xAA,0x87); //CMD8 �������� �����, �������� ������ ������
	if(temp!=0x01) return 1; //����� ���� ����� �� 0x01
	SPI_SendByte(0xFF);
	cnt=0;
	 do
	 {
		 temp=SD_cmd(0x41,0x00,0x00,0x00,0x00,0x95); //CMD1 �������� �����, �������� ������ ������
		 SPI_SendByte(0xFF);
		 cnt++;
	 } while ((temp!=0x00)&&cnt<0xFFFF); //��� ������ R1
	 if(cnt>=0xFFFF) return 2;
	 return 0;
}
//�������������-
*/

uint8_t read_symbol_from_SD (uint16_t symbol)
{
	WORD s1;
	uint8_t result;
	char buffer[10] = {};

	pf_lseek(6*symbol); //��������� ������ ������ �� 0 
	pf_read(buffer,4,&s1);
	sscanf(buffer, "%x", &result);

	return result;
} 