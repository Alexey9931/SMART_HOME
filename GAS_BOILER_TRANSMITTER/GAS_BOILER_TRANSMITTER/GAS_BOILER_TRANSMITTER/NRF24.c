/*
 * NRF24.c
 *
 * Created: 11.02.2022 20:29:40
 *  Author: Alex2
 */ 
#include "NRF24.h"
//-------------------------------------------------------------
#define TX_ADR_WIDTH 3//ширина адреса
#define TX_PLOAD_WIDTH 5// величина пакета(кол-во байт в пакете)
uint8_t TX_ADDRESS0[TX_ADR_WIDTH] = {0xb7,0xb5,0xa1};//адрес в режиме передатчика
uint8_t TX_ADDRESS1[TX_ADR_WIDTH] = {0xb5,0xb5,0xa1};//адрес в режиме приемника
//uint8_t TX_ADDRESS2[TX_ADR_WIDTH] = {0xb3,0xb5,0xa1};//адрес приемного устройства
uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};//буффер для пакетов
uint8_t pipe; //номер канала
uint8_t rx_flag = 0, tx_flag = 0;
uint8_t temp_setpoint_integer = 20;
uint8_t temp_setpoint_fraction = 0;
uint8_t home_temp_rx_integer = 0;
uint8_t home_temp_rx_fraction = 0;

extern int32_t millis;
extern int32_t millis_hometemp_update;

extern uint8_t gas_boiler_enable_flag;
//-------------------------------------------------------------
void NRF24_ini(void)
{
	LOW_CE;
	_delay_ms(5);
	nRF_write_register(CONFIG, 0x0a); // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
	_delay_ms(5);
	nRF_write_register(EN_AA, 0x03); // Enable Pipe1&3
	nRF_write_register(EN_RXADDR, 0x03); // Enable Pipe1&3
	nRF_write_register(SETUP_AW, 0x01); // Setup address width=3 bytes
	nRF_write_register(SETUP_RETR, 0x6F); // // 1750us, 15 retrans
	NRF24_ToggleFeatures();
	nRF_write_register(FEATURE, 0);
	nRF_write_register(DYNPD, 0);
	nRF_write_register(STATUS, 0x70);//Reset flags for IRQ
	nRF_write_register(RF_CH, 76); // частота 2476 MHz
	nRF_write_register(RF_SETUP, 0x06); //TX_PWR:0dBm, Datarate:1Mbps
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS1, TX_ADR_WIDTH);
	nRF_write_register(RX_PW_P1, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 1
	//пока уходим в режим приёмника
	NRF24L01_RX_Mode();
}
//-------------------------------------------------------------
void NRF24_Transmit(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
	LOW_CE;
	LOW_CSN;
	spi_send_recv(addr);//отправим адрес в шину
	_delay_us(1);
	while (bytes--)//отправим данные в буфер
	{
		spi_send_recv(*(pBuf++));
	}
	HIGH_CSN;
	HIGH_CE;
}
//-------------------------------------------------------------
uint8_t NRF24L01_Send(uint8_t *pBuf)
{
	uint8_t status=0x00, regval=0x00; //количество повторных попыток передачи, а также количество потерянных пакетов
	NRF24L01_TX_Mode(pBuf);//перевод в режим передатчика
	regval = nRF_read_register(CONFIG);
	//если модуль ушел в спящий режим, то разбудим его, включив бит PWR_UP и выключив PRIM_RX
	regval |= (1<<PWR_UP);
	regval &= ~(1<<PRIM_RX);
	nRF_write_register(CONFIG,regval);
	_delay_us(150); //Задержка минимум 130 мкс
	//Отправим данные в воздух
	NRF24_Transmit(W_TX_PAYLOAD, pBuf, TX_PLOAD_WIDTH);
	HIGH_CE;
	_delay_us(15); //minimum 10us high pulse (Page 21)
	LOW_CE;
	while( (PIND&(1<<IRQ)) != 0);
	status = nRF_read_register(STATUS);
	if(status&TX_DS) //tx_ds == 0x20
	{
		PORTD |= (1<<LED_TX);
		_delay_ms(100);
		PORTD &= ~(1<<LED_TX);
		nRF_write_register(STATUS, 0x20);
	}
	else if(status&MAX_RT)
	{
		nRF_write_register(STATUS, 0x10);
		NRF24_FlushTX();
	}
	regval = nRF_read_register(OBSERVE_TX);
	//Уходим в режим приёмника
	NRF24L01_RX_Mode();
	nRF_write_register(STATUS, 0x70);//Reset flags for IRQ
	return regval;
}
//-------------------------------------------------------------
ISR(INT0_vect)
{
	uint8_t status=0x01;
	status = nRF_read_register(STATUS);
	if(status & 0x40)
	{
		pipe = (status>>1)&0x07;
		NRF24_Read_Buf(R_RX_PAYLOAD,RX_BUF,TX_PLOAD_WIDTH);
		nRF_write_register(STATUS, 0x40);
		rx_flag = 1;
		PORTD |= (1<<LED_RX);
		_delay_ms(100);
		PORTD &= ~(1<<LED_RX);
		millis_hometemp_update = millis;
		
		gas_boiler_enable_flag = RX_BUF[0];
		//если получили сигнал от станции (флаг>100) что надо поменять уставку то меняем ее
		if (gas_boiler_enable_flag >= 100)
		{
			temp_setpoint_integer = RX_BUF[1];
			temp_setpoint_fraction = RX_BUF[2];
			gas_boiler_enable_flag -= 100;
		}
		
		//в авто режиме пока ничего не делаем
		if ((gas_boiler_enable_flag / 10) == 0)
		{
			
		}
		//в ручном делаем что сказали
		else 
		{
			if ((gas_boiler_enable_flag % 10) == 0)
			{
				PORTB &= ~(1<<MOSFET);
				PORTD &= ~(1<<LED_BOILER_STATUS);
			}
			else
			{
				PORTB |= (1<<MOSFET);
				PORTD |= (1<<LED_BOILER_STATUS);
			}
		}
		home_temp_rx_integer = RX_BUF[3];
		home_temp_rx_fraction = RX_BUF[4];
	}
}
//-------------------------------------------------------------
void NRF24L01_TX_Mode(uint8_t *pBuf)  //функция перехода в режим передатчика
{
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS0, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS0, TX_ADR_WIDTH);
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
void NRF24L01_RX_Mode(void) // включение режима приема
{
	uint8_t regval = 0x00;
	regval = nRF_read_register(CONFIG);
	//разбудим модуль и переведём его в режим приёмника, включив биты PWR_UP и PRIM_RX
	regval |= (1<<PWR_UP)|(1<<PRIM_RX);
	nRF_write_register(CONFIG,regval);
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS1, TX_ADR_WIDTH);
	HIGH_CE;
	_delay_us(150); //Задержка минимум 130 мкс
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
    spi_send_recv(addr);//отправим данные в буфер
	while (bytes--) 
	{
	  *(pBuf++) = spi_send_recv(0xFF);
	}
	HIGH_CSN;
}
//-------------------------------------------------------------
void NRF24_Write_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
	addr |= W_REGISTER;//включим бит записи в адрес
	LOW_CSN;
	spi_send_recv(addr);//отправим адрес в шину
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