/*
 * NRF24-Test.c
 *
 * Created: 11.02.2022 20:28:22
 * Author : Alex2
 */ 

#include "main.h"

uint8_t buf1[10] = {0};//буффер для отправки
uint8_t  reg = 0x00;
uint8_t retr_cnt, dt;
uint16_t retr_cnt_full;
//uint8_t data[5];
unsigned int hall_counter = 0;
unsigned int speed = 0;
char speed_str[10] = {0};
char temperature_humidity_str[10] = {0};
char wind_direction_str[10] = {0};
char Vbat[10] = {0};
char RainAmount[10] = {0};		
int adc_value1 = 0, adc_value2 = 0;	
int INTER_COUNT;

unsigned int TIM2_COUNT = 0;
void timer2_ini(void)//период 0.008с
{
	TCCR2 |= (1<<WGM21); // устанавливаем режим СТС (сброс по совпадению)
	OCR2 = 0b11111010; //записываем в регистр число для сравнения 250
	TCCR2 |= ((1<<CS21)|(1<<CS22));//установим делитель 256.
    TIMSK |= (1<<OCIE2); //устанавливаем бит разрешения прерывания 2ого счетчика по совпадению с OCR2	
}
//——————————————–
ISR(TIMER2_COMP_vect)
{
  if (TIM2_COUNT == 39750) TIM2_COUNT = 0;
  else TIM2_COUNT++;
}
ISR(INT1_vect)
{
	hall_counter++;
}
void timer_ini(void)//период 2сек
{
	TCCR1B |= (1<<WGM12); // устанавливаем режим СТС (сброс по совпадению)
	TIMSK |= (1<<OCIE1A); //устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)
	OCR1AH = 0b00111101; //записываем в регистр число для сравнения
	OCR1AL = 0b00001001;
	TCCR1B |= (1<<CS12)|(1<<CS10);//установим делитель 1024.
}
ISR (TIMER1_COMPA_vect)
{
   speed = hall_counter;
   hall_counter = 0;
}
SPI_init(void) //инициализация SPI
{
	DDRB |= ((1<<SS)|(1<<MOSI)|(1<<CE)|(1<<SCK)); //ножки SPI на выход
	PORTB &= ~((1<<SS)|(1<<MOSI)|(1<<SCK)); //низкий уровень
	DDRD |= (1<<IRQ);//IRQ на выход, в передатчике он не используется
	PORTD &= ~(1<<IRQ);
	HIGH_CSN;
	SPCR = ((1<<SPE)|(1<<MSTR));//включим шину, объявим ведущим
}
//-------------------------------------------------------------
port_init(void)
{
	//датчик Холла для анемометра
	DDRD &= ~(1<<PORTD3);//hall-sensor - на вход
	PORTD &= ~(1<<PORTD3);
	//управление транзистором
	DDRD |= (1<<TRANZISTOR);
	PORTD &= ~(1<<TRANZISTOR);
	//герконы для флюгера
	DDRB &= ~((1<<GERKON1)|(1<<GERKON5)|(1<<GERKON6));//gerkon - на вход
	PORTB &= ~((1<<GERKON1)|(1<<GERKON5)|(1<<GERKON6));
	DDRD &= ~((1<<GERKON2)|(1<<GERKON3)|(1<<GERKON4));
	PORTD &= ~((1<<GERKON2)|(1<<GERKON3)|(1<<GERKON4));
	DDRC &= ~((1<<GERKON7)|(1<<GERKON8));
	PORTC &= ~((1<<GERKON7)|(1<<GERKON8));
	//светодиод 
	DDRD |= (1<<LED);
	PORTD &= ~(1<<LED);
}
//-------------------------------------------------------------
uint8_t spi_send_recv(uint8_t data) // Передаёт и принимает 1 байт по SPI, возвращает полученное значение
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
char* WIND_DIRECT()
{
	if((PINB&(1<<GERKON1)) == 0)
	{
		sprintf (wind_direction_str,"S");
	}
    if ((PIND&(1<<GERKON2)) == 0)
	{
		sprintf (wind_direction_str,"S-W");
	}
    if ((PIND&(1<<GERKON3)) == 0)
	{
		sprintf (wind_direction_str,"W");
	}
    if ((PIND&(1<<GERKON4)) == 0)
	{
		sprintf (wind_direction_str,"N-W");
	}
    if ((PINB&(1<<GERKON5)) == 0)
	{
		sprintf (wind_direction_str,"N");
	}
    if ((PINB&(1<<GERKON6)) == 0)
	{
		sprintf (wind_direction_str,"N-E");
	}
    if ((PINC&(1<<GERKON7)) == 0)
	{
		sprintf (wind_direction_str,"E");
	}
    if ((PINC&(1<<GERKON8)) == 0)
	{
		sprintf (wind_direction_str,"S-E");
	}
	return wind_direction_str;
}

int main(void)
{
	int k = 0;
	uint8_t data[5] = {0};
	port_init();
	PORTD |= (1<<LED);
    SPI_init();
	ADC_Init();//Инициализация АЦП
	//USART_Init(16);
    NRF24_ini();
	// настраиваем параметры прерывания
	//----------------------
	MCUCR = (1<<ISC11) ;
	GICR = (1<<INT1);
	//----------------------
	_delay_ms(1000);
	PORTD &= ~(1<<LED);
	//dht22_init();	
	WDTCR &= ~(1<<WDE);//откл WDT
	ACSR |= (1<<ACD);//откл компаратор
    //INTER_COUNT == 0;
	timer_ini();
	timer2_ini();
	sei();
	
    while (1) 
    {
		if (TIM2_COUNT == 0)
		{
			DDRD &= ~(1<<PORTD3);//датчик холла на вход
			PORTD &= ~(1<<TRANZISTOR);//включаем датчик холла, дождя и герконы к питания
			ADCSRA |= (1<<ADEN);//вкл АЦП
			_delay_ms(4000);
			NRF24_ini();//инициализируем каждый раз, т.к. отключаем питание перед сном
			WIND_DIRECT();//измеряем направление ветра
			sprintf (speed_str,"%u",speed);
			ADC_convert ();
			sprintf (Vbat,"%u",adc_value1);
			_delay_ms(2000);
			ADC_convert ();
			sprintf (RainAmount,"%u",adc_value2);
			//-------------------------------------------
			//отправка температуры
			buf1[0] = 1;
			int tt = 0;
			tt = dt_check();
			uint8_t temp_sign = tt>>11;//вычисление знака температуры
			uint8_t temp_integer;//целая часть темп
			uint8_t temp_fraction;//дробная часть темп
			if (temp_sign == 0x00)
			{
				temp_fraction = tt & 0xF;
				temp_fraction = (temp_fraction<<1) + (temp_fraction<<3);// ”множаем на 10
				temp_fraction = (temp_fraction>>4);//делим на 16 или умножаем на 0.0625
				temp_integer = (tt&0x07FF)>>4;
			}
			else
			{
				temp_fraction = ((~tt) & 0xF);
				temp_fraction = (temp_fraction<<1) + (temp_fraction<<3);// ”множаем на 10
				temp_fraction = (temp_fraction>>4);//делим на 16 или умножаем на 0.0625
				temp_integer = ((~(tt))&0x07FF)>>4;
			}
			buf1[1] = temp_sign;
			buf1[2] = temp_integer;
			buf1[3] = temp_fraction;
			/*if (dht22_GetData(data))
			{
			  buf1[1] = data[1];//младший бит температуры
			  buf1[2] = data[2];//старший бит температуры
			}*/
			dt = NRF24L01_Send(buf1);
			memset(buf1, 0, sizeof(int) * strlen(buf1));//очистка массива
			_delay_ms(1000);
			//-------------------------------------------
			//отправка влажности
			 buf1[0] = 5;
			 long int hum = 0;
			 hum = HTU21D_get_humidity();
			 hum = (hum*125)/65536 - 6;
			 buf1[1] = (uint8_t) hum;
			 //buf1[1] = data[3];//младший бит влажности
			 //buf1[2] = data[4];//старший бит влажности
			 dt = NRF24L01_Send(buf1);
			 memset(buf1, 0, sizeof(int) * strlen(buf1));//очистка массива
			 _delay_ms(1000);
			//-------------------------------------------
			//отправка скорости ветра
			buf1[0] = 2;
			for (k = 1; k < strlen(speed_str)+1; k++)
			{
				buf1[k] = speed_str[k-1];
			}
			dt = NRF24L01_Send(buf1);
			memset(buf1, 0, sizeof(int) * strlen(buf1));//очистка массива
			_delay_ms(1000);
			//-------------------------------------------
			//отправка направления ветра
			buf1[0] = 3;
			for (k = 1; k < strlen(wind_direction_str)+1; k++)
			{
				buf1[k] = wind_direction_str[k-1];
			}
			dt = NRF24L01_Send(buf1);
			memset(buf1, 0, sizeof(int) * strlen(buf1));//очистка массива
			_delay_ms(1000);
			//-------------------------------------------
			//отправка заряда аккумулятора
			buf1[0] = 4;
			for (k = 1; k < strlen(Vbat)+1; k++)
			{
				buf1[k] = Vbat[k-1];
			}
			dt = NRF24L01_Send(buf1);
			memset(buf1, 0, sizeof(int) * strlen(buf1));//очистка массива
			_delay_ms(1000);
			//-------------------------------------------
			//отправка кол-ва осадков
			buf1[0] = 6;
			for (k = 1; k < strlen(RainAmount)+1; k++)
			{
				buf1[k] = RainAmount[k-1];
			}
			dt = NRF24L01_Send(buf1);
			memset(buf1, 0, sizeof(int) * strlen(buf1));//очистка массива
			//-------------------------------------------
			//перевод NRF24 в энергорежим
			//reg = nRF_read_register(CONFIG);
			//reg &= ~(1<<PWR_UP);
			//nRF_write_register(CONFIG,reg);
			//-------------------------------------------
			//перевод МК в сон
			PORTD |= (1<<TRANZISTOR);//отключаем датчик холла, дождя и герконы от питания
			ADCSRA &= ~(1<<ADEN);//выкл АЦП
			DDRD |= (1<<PORTD3);//датчик холла на выход
			//set_sleep_mode(SLEEP_MODE_PWR_SAVE);
			//sleep_enable();
			//sleep_cpu();
			//sleep_disable();
			//-------------------------------------------
			//выход из сна каждые ~5мин
			//INTER_COUNT++;
			//if (INTER_COUNT == 40) INTER_COUNT = 0;
			//-------------------------------------------
			//DDRD &= ~(1<<PORTD3);//датчик холла на вход
			//PORTD &= ~(1<<TRANZISTOR);//включаем датчик холла, дождя и герконы к питания
			//ADCSRA |= (1<<ADEN);//вкл АЦП
			//вывод NRF24 из энергорежима
			//reg |= (1<<PWR_UP);
			//nRF_write_register(CONFIG,reg);
			//-------------------------------------------
		}
    }
}

