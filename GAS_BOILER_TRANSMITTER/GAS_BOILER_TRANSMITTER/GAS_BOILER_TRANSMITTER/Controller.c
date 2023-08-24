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

void gas_boiler_controller(void)
{
	//���� ���������� �������� ������ � ���� ������
	if (work_mode == 0)
	{
		//���� ����������� ������ �������, �� �������� �����
		if ((home_temp_rx_integer*10+home_temp_rx_fraction) < (temp_setpoint_integer*10+temp_setpoint_fraction))
		{
			gas_boiler_enable_flag = 1;
			PORTB &= ~(1<<MOSFET);
			PORTD |= (1<<LED_BOILER_STATUS);
		}
		//���� ������ �� ���������
		else if((home_temp_rx_integer*10+home_temp_rx_fraction) >= (temp_setpoint_integer*10+temp_setpoint_fraction))
		{
			gas_boiler_enable_flag = 0;
			PORTB |= (1<<MOSFET);
			PORTD &= ~(1<<LED_BOILER_STATUS);
		}
	}
}
