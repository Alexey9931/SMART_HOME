/*
 * Controller.c
 *
 * Created: 30.07.2023 14:25:04
 *  Author: Alex2
 */ 
#include "Controller.h"

extern uint8_t temp_setpoint_integer;
extern uint8_t temp_setpoint_fraction;
extern uint8_t home_temp_rx_integer;
extern uint8_t home_temp_rx_fraction;

extern uint8_t gas_boiler_enable_flag;
extern uint8_t work_mode;

#define DELTA_TEMP 20 //�������� ���������� ����� ����� ��� �� �������� (T*10)

void gas_boiler_controller(void)
{
	uint16_t home_temp = home_temp_rx_integer*10+home_temp_rx_fraction;
	uint16_t setpoint = temp_setpoint_integer*10+temp_setpoint_fraction;
	//���� ���������� �������� ������ � ���� ������
	if (work_mode == 0)
	{
		//���� ����������� ������� ������ ����������� �� 2 �������, �� �������� �����
		if ((home_temp + DELTA_TEMP) <= setpoint)
		{
			gas_boiler_enable_flag = 1;
			PORTB &= ~(1<<MOSFET);
			PORTD |= (1<<LED_BOILER_STATUS);
			EEPROM_write(5, gas_boiler_enable_flag);
		}
		//���� ����������� ������ �������, �� ���������
		else if (home_temp > setpoint)
		{
			gas_boiler_enable_flag = 0;
			PORTB |= (1<<MOSFET);
			PORTD &= ~(1<<LED_BOILER_STATUS);
			EEPROM_write(5, gas_boiler_enable_flag);
		}
	}
}
