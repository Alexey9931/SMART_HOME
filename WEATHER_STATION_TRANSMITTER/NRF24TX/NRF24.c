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
//-------------------------------------------------------------
void NRF24_ini(void)
{
	LOW_CE;
	_delay_ms(5);
	nRF_write_register(CONFIG, 0x0a); // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
	_delay_ms(5);
	nRF_write_register(EN_AA, 0x01); // Enable Pipe0
	nRF_write_register(EN_RXADDR, 0x01); // Enable Pipe0
	nRF_write_register(SETUP_AW, 0x01); // Setup address width=3 bytes
	nRF_write_register(SETUP_RETR, 0x5F); // // 1500us, 15 retrans
	NRF24_ToggleFeatures();
	nRF_write_register(FEATURE, 0);
	nRF_write_register(DYNPD, 0);
	nRF_write_register(STATUS, 0x70);//Reset flags for IRQ
	nRF_write_register(RF_CH, 76); // ������� 2476 MHz
	nRF_write_register(RF_SETUP, 0x06); //TX_PWR:0dBm, Datarate:1Mbps
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
	nRF_write_register(RX_PW_P0, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 0
	//���� ������ � ����� ��������
	NRF24L01_RX_Mode();
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
	while( (PIND&(1<<IRQ)) != 0);
	status = nRF_read_register(STATUS);
	if(status&TX_DS) //tx_ds == 0x20
	{
		PORTD |= (1<<LED);
		_delay_ms(100);
		PORTD &= ~(1<<LED);
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
	nRF_write_register(STATUS, 0x70);//Reset flags for IRQ
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