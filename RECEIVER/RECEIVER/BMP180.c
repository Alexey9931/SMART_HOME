#include "BMP180.h"
#include "math.h"
// ���������� ������
//int32_t temperature = 0;
//int32_t    pressure = 0;
//int32_t    altitude = 0;
// ���������� ����������
int16_t ac1;
int16_t ac2;
int16_t ac3;
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1;
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;
// ������������� ����������
//uint32_t D1;
//uint32_t D2;
//uint32_t D3;
// ������� ��� ������ � bmp180
#define OSS 3
#define BMP180_R 0xEF
#define BMP180_W 0xEE

// ������ �������� 16 ���
uint16_t Read(uint8_t address) 
{
	uint16_t msb=0;
	uint16_t lsb=0;
	uint16_t data;

	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(BMP180_W);        // �������� ������ ����������, ����� ������
	I2C_SendByte(address);	        // �������� ������ ������
	I2C_StopCondition();                // ��������� i2c

	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(BMP180_R);        // �������� ������ ����������, ����� ������
	msb = I2C_ReadByte();
	lsb = I2C_ReadLastByte();
	I2C_StopCondition();                // ��������� i2c

	data = (uint16_t)((msb << 8) + lsb);
	return data;
}
// �������� ������ ��� ����������
void BMP180_Calibration (void) 
{
	ac1 = Read(0xAA);
	ac2 = Read(0xAC);
	ac3 = Read(0xAE);
	ac4 = Read(0xB0);
	ac5 = Read(0xB2);
	ac6 = Read(0xB4);
	b1  = Read(0xB6);
	b2  = Read(0xB8);
	mb  = Read(0xBA);
	mc  = Read(0xBC);
	md  = Read(0xBE);
	/*ac1 = 7773;
	ac2 = -1139;
	ac3 = -14510;
	ac4 = 33929;
	ac5 = 25188;
	ac6 = 17744;
	b1 = 6515;
	b2 = 43;
	mb = -32768;
	mc = -11786;
	md = 2705;*/
}

// ��������� �������� �����������
int32_t BMP180_get_temper(void)
{
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(BMP180_W);        // �������� ������ ����������, ����� ������
	I2C_SendByte(0xF4);	        // �������� ������ ������
	I2C_SendByte(0x2E);	        // �������� ���������� (oss) ������ ������ ����������� (����� ��� �� ��� ��� ��������)
	I2C_StopCondition();                // ��������� i2c

	_delay_ms(5);                   // ����� �� ����� (�� 5 �� 26 �� � ����������� �� ���������� (oss))

	return Read(0xF6);     // ��������� �����������
}
// ��������� �������� ������������ ��������
int32_t BMP180_get_pressure()
{
	// ������������� ����������
	uint32_t D1;
	uint32_t D2;
	uint32_t D3;
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(BMP180_W);        // �������� ������ ����������, ����� ������
	I2C_SendByte(0xF4);	        // �������� ������ ������
	I2C_SendByte(0x34+(OSS<<6));	// �������� ���������� (oss) ������ ������ �����������
	I2C_StopCondition();                // ��������� i2c

	_delay_ms(26);                  // ����� �� ����� (�� 5 �� 26 �� � ����������� �� ���������� (oss))
	
	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(BMP180_W);        // �������� ������ ����������, ����� ������
	I2C_SendByte(0xF6);	        // �������� ������ ������
	I2C_StopCondition();                // ��������� i2c

	I2C_StartCondition();               // ������ i2c
	I2C_SendByte(BMP180_R);        // �������� ������ ����������, ����� ������
	D1=I2C_ReadByte();             // MSB
	D2=I2C_ReadByte();             // LSB
	D3=I2C_ReadLastByte();             // XLSB
	I2C_StopCondition();                // ��������� i2c
	

	return (int32_t)(((D1 << 16) + (D2 << 8) + D3) >> (8-OSS));  // ��������� �������� (� �� ?)
}
// �������� �������� ����������� � ������������ �������� � ������ ������������� �������������
int32_t BMP180_calculation () 
{
	int32_t pressure = 0;
	//int8_t i;
	int32_t ut=0;
	int32_t up=0;
	int32_t x1, x2, b5, b6, x3, b3, p;
	uint32_t b4, b7;

	ut += BMP180_get_temper();

	up = BMP180_get_pressure();

	x1 = ((int32_t)ut - (int32_t)ac6) * (int32_t)ac5 >> 15;
	x2 = ((int32_t)mc << 11) / (x1 + md);
	b5 = x1 + x2;
	//*temperature = (b5 + 8) >> 4;
	
	b6 = b5 - 4000;
	x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
	x2 = (ac2 * b6) >> 11;
	x3 = x1 + x2;
	b3 = (((((int32_t) ac1) * 4 + x3)<<OSS) + 2)>> 2;
	x1 = (ac3 * b6) >> 13;
	x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
	b7 = ((uint32_t) (up - b3) * (50000 >> OSS));
	//p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
	
	if (b7 < 0x80000000)
	{
		p = (b7 << 1) / b4;
	}
	else
	{
		p = (b7 / b4) << 1;
	}

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	pressure = p + ((x1 + x2 + 3791) >> 4);
	return pressure;
}
// ������� ������� ������ ��� ������� ���� (���������) (������� ����� ����� ����� ������ ��-�� �������������� ������� !!!)
/*void bmp180CalcAltitude(int32_t pressure)
{
	float temp;

	temp = (float) pressure/101325;
	temp = 1-pow(temp, 0.19029);
	//altitude = round(44330*temp*10);
	altitude = 44330*temp*100;
	//get altitude in dm
}*/