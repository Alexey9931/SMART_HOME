/*
 * NRF24.c
 *
 * Created: 11.02.2022 20:29:40
 *  Author: Alex2
 */ 
#include "NRF24.h"
//-------------------------------------------------------------
#define TX_ADR_WIDTH 3//������ ������
#define TX_PLOAD_WIDTH 5// �������� ������(���-�� ���� � ������)
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xb3,0xb4,0x01};//�����
uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};//������ ��� �������
extern char temp_street[5];
extern char hum_street[5];
extern char temp_home[5];
extern char hum_home[5];
extern char WIND_speed[5];
extern char wind_direction[6];
extern char Vbat[5];
extern char Rain[6];
extern char Press_home[6];
extern char temp_street_to_DB[6];
extern char hum_street_to_DB[5];
extern char WIND_speed_to_DB[5];
extern char wind_direction_to_DB[6];
extern char Vbat_to_DB[5];
extern char Rain_to_DB[6];
extern struct Time_Parameters {
	char hours[4];
	char minutes[4];
	char seconds[4];
	char weakday[10];
	char mounthday[4];
	char Mounth[4];
	char Year[4];
} T_Param;
char HALL_counter[5] = {0};
char adc_value1[6];
char adc_value2[6];
uint8_t ErrCnt_Fl = 0;//��� �������� ������
uint8_t flag_irq = 1;
uint8_t rx_flag = 0;
extern char receive_time[20];
uint8_t receive_counter;
extern uint8_t timer1_flag, timer2_flag;

uint8_t street_temp_sign;//���� ����� ����
uint8_t street_temp_integer;//����� ����� ����� ����
uint8_t street_temp_fraction;//������� ����� ����� ����
uint8_t wind_speed_integer;//����� ����� �� �����
uint8_t wind_speed_fraction;//������� ����� �� �����

//-------------------------------------------------------------
void NRF24_ini(void)
{
	LOW_CE;
	_delay_ms(5);
	nRF_write_register(CONFIG, 0x0a); // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
	_delay_ms(5);
	nRF_write_register(EN_AA, 0x02); // Enable Pipe1
	nRF_write_register(EN_RXADDR, 0x02); // Enable Pipe1
	nRF_write_register(SETUP_AW, 0x01); // Setup address width=3 bytes
	nRF_write_register(SETUP_RETR, 0x5F); // // 1500us, 15 retrans
	NRF24_ToggleFeatures();
	nRF_write_register(FEATURE, 0);
	nRF_write_register(DYNPD, 0);
	nRF_write_register(STATUS, 0x70);//Reset flags for IRQ
	nRF_write_register(RF_CH, 76); // ������� 2476 MHz
	nRF_write_register(RF_SETUP, 0x06); //TX_PWR:0dBm, Datarate:1Mbps
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS, TX_ADR_WIDTH);
	nRF_write_register(RX_PW_P1, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 1
	//���� ������ � ����� ��������
	NRF24L01_RX_Mode();
}
//-------------------------------------------------------------
void NRF24L01_Receive(void)
{
	int temp, hum;
	uint8_t n = 0;
	
	uint8_t byte1 = 0;
	uint8_t byte2 = 0;
	if(rx_flag==1)
	{
		switch (RX_BUF[0])
		{
			
			//--------------------------------------
			//��������� �����������
			case 1:	receive_counter++;
					memset(temp_street, 0, sizeof(char) * strlen(temp_street));//������� �������
					memset(temp_street_to_DB, 0, sizeof(char) * strlen(temp_street_to_DB));//������� �������
					byte1 = RX_BUF[1];//������� ��� �����������
					byte2 = RX_BUF[2];//������� ��� �����������
					temp = ((byte2<<8)|byte1);
					if ((temp & 0x8000) == 0x8000) temp = ~(temp & 0x7FFF);
					sprintf(temp_street,"%d.%d",temp/10 ,abs(temp)%10);
					sprintf(temp_street_to_DB,"%d.%d",temp/10 ,abs(temp)%10);
					break;
			//--------------------------------------
			//��������� ���������
			case 5:	receive_counter++;
					memset(hum_street, 0, sizeof(char) * strlen(hum_street));//������� �������
					memset(hum_street_to_DB, 0, sizeof(char) * strlen(hum_street_to_DB));//������� �������
					byte1 = RX_BUF[1];//������� ��� �����������
					byte2 = RX_BUF[2];//������� ��� �����������
					hum = ((byte2<<8)|byte1) / 10;
					sprintf(hum_street,"%d",hum);
					sprintf(hum_street_to_DB,"%d",hum);
					break;
			//--------------------------------------
			//��������� �������� �����
			case 2:	receive_counter++;
					memset(HALL_counter, 0, sizeof(char) * strlen(HALL_counter));//������� �������
					memset(WIND_speed, 0, sizeof(char) * strlen(WIND_speed));//������� �������
					memset(WIND_speed_to_DB, 0, sizeof(char) * strlen(WIND_speed_to_DB));//������� �������
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						HALL_counter[n] = RX_BUF[n+1];
					}
					if (wind_speed (HALL_counter) != 0)
					{
						sprintf(WIND_speed,"%d.%d", wind_speed (HALL_counter)/100, wind_speed (HALL_counter)%100);
						sprintf(WIND_speed_to_DB,"%d.%d", wind_speed (HALL_counter)/100, wind_speed (HALL_counter)%100);
					}
					else
					{
						sprintf(WIND_speed,"0.00");
						sprintf(WIND_speed_to_DB,"0.00");
					}
					break;
			//--------------------------------------
			//��������� ����������� �����
			case 3:	receive_counter++;
					memset(wind_direction, 0, sizeof(char) * strlen(wind_direction));//������� �������
					memset(wind_direction_to_DB, 0, sizeof(char) * strlen(wind_direction_to_DB));//������� �������
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						wind_direction[n] = RX_BUF[n+1];
						wind_direction_to_DB[n] = RX_BUF[n+1];
					}
					break;
			//--------------------------------------
			//��������� ������ ������������
			case 4:	receive_counter++;
					memset(adc_value1, 0, sizeof(char) * strlen(adc_value1));//������� �������
					memset(Vbat, 0, sizeof(char) * strlen(Vbat));//������� �������
					memset(Vbat_to_DB, 0, sizeof(char) * strlen(Vbat_to_DB));//������� �������
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						adc_value1[n] = RX_BUF[n+1];
					}
					sprintf(Vbat,"%d.%d", V_BAT(adc_value1)/100, V_BAT(adc_value1)%100);
					sprintf(Vbat_to_DB,"%d.%d", V_BAT(adc_value1)/100, V_BAT(adc_value1)%100);
					break;
			//--------------------------------------
			//��������� ���-�� �������
			case 6:	receive_counter++;
					memset(adc_value2, 0, sizeof(char) * strlen(adc_value2));//������� �������
					memset(Rain, 0, sizeof(char) * strlen(Rain));//������� �������
					memset(Rain_to_DB, 0, sizeof(char) * strlen(Rain_to_DB));//������� �������
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						adc_value2[n] = RX_BUF[n+1];
					}
					sprintf(Rain,"%d", RAIN_AMOUNT(adc_value2));
					sprintf(Rain_to_DB,"%d", RAIN_AMOUNT(adc_value2));
					break;
		}
		//--------------------------------------
		//memset(RX_BUF, 0, sizeof(int) * strlen(RX_BUF));//������� �������
		rx_flag = 0;
	}
}
//-------------------------------------------------------------
ISR(INT0_vect)
{
	uint8_t status=0x01;
	status = nRF_read_register(STATUS);
	if(status & 0x40)
	{
		NRF24_Read_Buf(R_RX_PAYLOAD,RX_BUF,TX_PLOAD_WIDTH);
		nRF_write_register(STATUS, 0x40);
	}
	 if(receive_counter == 6)
	 {
		 receive_counter = 0;
	 }
	 rx_flag = 1;
}
//-------------------------------------------------------------
void NRF24_Transmit(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
	LOW_CE;
	LOW_CSN;
	spi_send_recv(addr);//�������� ����� � ����
	_delay_us(1);
	while (bytes--)//�������� ������ � �����
	{
		spi_send_recv(*(pBuf++));
	}
	HIGH_CSN;
	HIGH_CE;
}
//-------------------------------------------------------------
uint8_t NRF24L01_Send(uint8_t *pBuf)
{
	uint8_t status=0x00, regval=0x00; //���������� ��������� ������� ��������, � ����� ���������� ���������� �������
	NRF24L01_TX_Mode(pBuf);//������� � ����� �����������
	regval = nRF_read_register(CONFIG);
	//���� ������ ���� � ������ �����, �� �������� ���, ������� ��� PWR_UP � �������� PRIM_RX
	regval |= (1<<PWR_UP);
	regval &= ~(1<<PRIM_RX);
	nRF_write_register(CONFIG,regval);
	_delay_us(150); //�������� ������� 130 ���
	//�������� ������ � ������
	NRF24_Transmit(W_TX_PAYLOAD, pBuf, TX_PLOAD_WIDTH);
	HIGH_CE;
	_delay_us(15); //minimum 10us high pulse (Page 21)
	LOW_CE;
	while(flag_irq == 1) {}
	status = nRF_read_register(STATUS);
	if(status&TX_DS) //tx_ds == 0x20
	{
		nRF_write_register(STATUS, 0x20);
	}
	else if(status&MAX_RT)
	{
		nRF_write_register(STATUS, 0x10);
		NRF24_FlushTX();
	}
	regval = nRF_read_register(OBSERVE_TX);
	//������ � ����� ��������
	NRF24L01_RX_Mode();
    flag_irq = 1;
	return regval;
}
//-------------------------------------------------------------
void NRF24L01_TX_Mode(uint8_t *pBuf)  //������� �������� � ����� �����������
{
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	LOW_CE;
	// Flush buffers
	NRF24_FlushRX();
	NRF24_FlushTX();
}
//-------------------------------------------------------------
void NRF24_FlushRX(void)
{
	LOW_CSN;
	spi_send_recv(FLUSH_RX);
	_delay_us(1);
	HIGH_CSN;
}
//-------------------------------------------------------------
void NRF24_FlushTX(void)
{
	LOW_CSN;
	spi_send_recv(FLUSH_TX);
	_delay_us(1);
	HIGH_CSN;
}
//-------------------------------------------------------------
void NRF24L01_RX_Mode(void) // ��������� ������ ������
{
	uint8_t regval = 0x00;
	regval = nRF_read_register(CONFIG);
	//�������� ������ � �������� ��� � ����� ��������, ������� ���� PWR_UP � PRIM_RX
	regval |= (1<<PWR_UP)|(1<<PRIM_RX);
	nRF_write_register(CONFIG,regval);
	HIGH_CE;
	_delay_us(150); //�������� ������� 130 ���
	// Flush buffers
	NRF24_FlushRX();
	NRF24_FlushTX();
}
//-------------------------------------------------------------
void NRF24_ToggleFeatures(void)
{
	LOW_CSN;
	spi_send_recv(ACTIVATE);
	_delay_us(1);
	spi_send_recv(0x73);
	HIGH_CSN;
}
//-------------------------------------------------------------
void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
	LOW_CSN;
    spi_send_recv(addr);//�������� ������ � �����
	while (bytes--) 
	{
	  *(pBuf++) = spi_send_recv(0xFF);
	}
	HIGH_CSN;
}
//-------------------------------------------------------------
void NRF24_Write_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
	addr |= W_REGISTER;//������� ��� ������ � �����
	LOW_CSN;
	spi_send_recv(addr);//�������� ����� � ����
	_delay_us(1);
	while (bytes--)
    {
	   spi_send_recv(*(pBuf++));
	}
	HIGH_CSN;
}
//-------------------------------------------------------------
void nRF_command(uint8_t command)
{
	LOW_CSN;
	spi_send_recv(command);
	HIGH_CSN;
}
//-------------------------------------------------------------
void nRF_write_register(uint8_t reg, uint8_t data_out)
{
	reg |= W_REGISTER;
	LOW_CSN;
	spi_send_recv(reg);
	spi_send_recv(data_out);
	HIGH_CSN;
}
//-------------------------------------------------------------
uint8_t nRF_read_register(uint8_t reg)
{
	uint8_t data_in;
	LOW_CSN;
	data_in = spi_send_recv(reg);
	if(reg != STATUS)
	{
		data_in = spi_send_recv(NOP);
	}
	HIGH_CSN;
	return data_in;
}
//-------------------------------------------------------------