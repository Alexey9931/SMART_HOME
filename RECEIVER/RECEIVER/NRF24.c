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
uint8_t TX_ADDRESS0[TX_ADR_WIDTH] = {0xb5,0xb5,0xa1};//адрес в режиме передатчика (т.е. это адрес приемного устройства)
uint8_t TX_ADDRESS1[TX_ADR_WIDTH] = {0xb7,0xb5,0xa1};//адрес в режиме приемника (т.е. это адрес газового котла)
uint8_t TX_ADDRESS2[TX_ADR_WIDTH] = {0xb3,0xb4,0x01};//адрес метеостанции 
//uint8_t TX_ADDRESS0[TX_ADR_WIDTH] = {0xb3,0xb4,0x01};//адрес метеостанции 
//uint8_t TX_ADDRESS1[TX_ADR_WIDTH] = {0xb7,0xb5,0xa1};//адрес газового котла	(одновременно и адрес в режиме приемника)
uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};//буффер для пакетов
extern char wind_direction[6];
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
uint8_t ErrCnt_Fl = 0;//для подсчета ошибок
uint8_t flag_irq = 1;
uint8_t rx_flag = 0;
extern char receive_time[20];
uint8_t receive_counter;
extern uint8_t timer1_flag, timer2_flag;
uint8_t pipe;//номер канала

uint8_t street_temp_sign;//знак уличн темп
uint8_t street_temp_integer;//целая часть уличн темп
uint8_t street_temp_fraction;//дробная часть уличн темп
int street_hum_integer;//целая часть уличн влажн
uint8_t wind_speed_integer;//целая часть ск ветра
uint8_t wind_speed_fraction;//дробная часть ск ветра
uint8_t rain; //целая часть осадков
uint8_t V_Bat_integer;//целая часть заряда акб
uint8_t V_Bat_fraction;//дробная часть заряда акб

extern uint8_t gas_boiler_enable_flag ;
extern uint8_t gas_boiler_setpoint_temp_integer;
extern uint8_t gas_boiler_setpoint_temp_fraction;
extern uint8_t setpoint_change_flag;

//-------------------------------------------------------------
void NRF24_ini(void)
{
	LOW_CE;
	_delay_ms(5);
	nRF_write_register(CONFIG, 0x0a); // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
	_delay_ms(5);
	nRF_write_register(EN_AA, 0x03); // Enable Pipe1 and Pipe0
	nRF_write_register(EN_RXADDR, 0x03); // Enable Pipe1 and Pipe0
	nRF_write_register(SETUP_AW, 0x01); // Setup address width=3 bytes
	nRF_write_register(SETUP_RETR, 0x5F); // // 1500us, 15 retrans
	NRF24_ToggleFeatures();
	nRF_write_register(FEATURE, 0);
	nRF_write_register(DYNPD, 0);
	nRF_write_register(STATUS, 0x70);//Reset flags for IRQ
	nRF_write_register(RF_CH, 76); // частота 2476 MHz
	nRF_write_register(RF_SETUP, 0x06); //TX_PWR:0dBm, Datarate:1Mbps
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS1, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS2, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS1, TX_ADR_WIDTH);
	nRF_write_register(RX_PW_P0, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 0
	nRF_write_register(RX_PW_P1, TX_PLOAD_WIDTH); //Number of bytes in RX payload in data pipe 1
	//пока уходим в режим приёмника
	NRF24L01_RX_Mode();
}
//-------------------------------------------------------------
void NRF24L01_Receive(void)
{
	int temp, hum;
	uint8_t n = 0;
	
	uint8_t byte1 = 0;
	uint8_t byte2 = 0;
	if((rx_flag==1)&&(pipe == 0))
	{
		switch (RX_BUF[0])
		{	
			//--------------------------------------
			//получение температуры
			case 1:	receive_counter++;
					memset(temp_street_to_DB, '\0', sizeof(char) * strlen(temp_street_to_DB));//очистка массива
					street_temp_sign = RX_BUF[1];
					street_temp_integer = RX_BUF[2];
					street_temp_fraction = RX_BUF[3];
					if (RX_BUF[1] != 0x00)
					{
						//отриц темп
						sprintf(temp_street_to_DB,"-%d.%d",RX_BUF[2],RX_BUF[3]);
					}
					else
					{
						sprintf(temp_street_to_DB,"%d.%d",RX_BUF[2],RX_BUF[3]);
					}
					break;
			//--------------------------------------
			//получение влажности
			case 5:	receive_counter++;
					memset(hum_street_to_DB, '\0', sizeof(char) * strlen(hum_street_to_DB));//очистка массива
					if (RX_BUF[1] > 99) 
					{
						RX_BUF[1] = 99;
					}
					street_hum_integer = RX_BUF[1];
					sprintf(hum_street_to_DB,"%d",RX_BUF[1]);
					break;
			//--------------------------------------
			//получение скорости ветра
			case 2:	receive_counter++;
					memset(HALL_counter, '\0', sizeof(char) * strlen(HALL_counter));//очистка массива
					memset(WIND_speed_to_DB, '\0', sizeof(char) * strlen(WIND_speed_to_DB));//очистка массива
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						HALL_counter[n] = RX_BUF[n+1];
					}
					if (wind_speed (HALL_counter) != 0)
					{
						wind_speed_integer = wind_speed (HALL_counter)/100;
						wind_speed_fraction = wind_speed (HALL_counter)%100;
						sprintf(WIND_speed_to_DB,"%d.%d", wind_speed (HALL_counter)/100, wind_speed (HALL_counter)%100);
					}
					else
					{
						wind_speed_integer = 0;
						wind_speed_fraction = 0;
						sprintf(WIND_speed_to_DB,"0.00");
					}
					break;
			//--------------------------------------
			//получение направления ветра
			case 3:	receive_counter++;
					memset(wind_direction, '\0', sizeof(char) * strlen(wind_direction));//очистка массива
					memset(wind_direction_to_DB, '\0', sizeof(char) * strlen(wind_direction_to_DB));//очистка массива
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						wind_direction[n] = RX_BUF[n+1];
						wind_direction_to_DB[n] = RX_BUF[n+1];
					}
					break;
			//--------------------------------------
			//получение заряда аккумулятора
			case 4:	receive_counter++;
					memset(adc_value1, '\0', sizeof(char) * strlen(adc_value1));//очистка массива
					memset(Vbat_to_DB, '\0', sizeof(char) * strlen(Vbat_to_DB));//очистка массива
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						adc_value1[n] = RX_BUF[n+1];
					}
					V_Bat_integer = V_BAT(adc_value1)/100;
					V_Bat_fraction = V_BAT(adc_value1)%100;
					sprintf(Vbat_to_DB,"%d.%d", V_BAT(adc_value1)/100, V_BAT(adc_value1)%100);
					break;
			//--------------------------------------
			//получение кол-ва осадков
			case 6:	receive_counter++;
					memset(adc_value2, '\0', sizeof(char) * strlen(adc_value2));//очистка массива
					memset(Rain_to_DB, '\0', sizeof(char) * strlen(Rain_to_DB));//очистка массива
					for ( n = 0; n < (strlen(RX_BUF)-1); n++)
					{
						adc_value2[n] = RX_BUF[n+1];
					}
					rain = RAIN_AMOUNT(adc_value2);
					sprintf(Rain_to_DB,"%d", RAIN_AMOUNT(adc_value2));
					break;
		}
		//--------------------------------------
		//memset(RX_BUF, 0, sizeof(int) * strlen(RX_BUF));//очистка массива
		rx_flag = 0;
	}
	if((rx_flag==1)&&(pipe == 1))
	{	
		//если получили сигнал от котла (флаг>100) что надо поменять уставку то меняем ее
		if (RX_BUF[0] >= 100)
		{
			gas_boiler_setpoint_temp_integer = RX_BUF[1];
			gas_boiler_setpoint_temp_fraction = RX_BUF[2];
			//gas_boiler_enable_flag -= 100;
		}
		//если сами хотим ее поменять то меняем
		else if ((gas_boiler_setpoint_temp_integer != RX_BUF[1]) || (gas_boiler_setpoint_temp_fraction != RX_BUF[2])) 
		{
			setpoint_change_flag = 1;
		}
		
		//в авто режиме просто читаем флаг состояния
		if ((gas_boiler_enable_flag / 10) == 0) 
		{
			if (RX_BUF[0] >= 100)
			{
				RX_BUF[0] -= 100;
			}
			if ((RX_BUF[0] / 10) == 0)
			{
				gas_boiler_enable_flag = RX_BUF[0];
			}
		}
		//в ручном не читаем флаг
		
		rx_flag = 0;
	}
}
//-------------------------------------------------------------
ISR(INT2_vect)
{
	uint8_t status=0x01;
	status = nRF_read_register(STATUS);
	if(status & 0x40)
	{
		pipe = (status>>1)&0x07;
		NRF24_Read_Buf(R_RX_PAYLOAD,RX_BUF,TX_PLOAD_WIDTH);
		nRF_write_register(STATUS, 0x40);
		rx_flag = 1;
	}
	if((receive_counter == 6)&&(pipe == 0))
	{
		receive_counter = 0;
	}
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
void NRF24L01_TX_Mode(uint8_t *pBuf)  //функция перехода в режим передатчика
{
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS0, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS0, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH);
	//NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH);
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
	NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS2, TX_ADR_WIDTH);
	//NRF24_Write_Buf(RX_ADDR_P0, TX_ADDRESS0, TX_ADR_WIDTH);
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