#define  F_CPU		8000000
#include <util/delay.h>

#define DDRA		(*(volatile unsigned char *)0x3A)
#define PORTA		(*(volatile unsigned char *)0x3B)
#define PINA		(*(volatile unsigned char *)0x39)

#define DDRB		(*(volatile unsigned char *)0x37)
#define PORTB		(*(volatile unsigned char *)0x38)
#define PINB		(*(volatile unsigned char *)0x36)

#define DDRC		(*(volatile unsigned char *)0x34)
#define PORTC		(*(volatile unsigned char *)0x35)
#define PINC		(*(volatile unsigned char *)0x33)

#define DDRD		(*(volatile unsigned char *)0x31)
#define PORTD		(*(volatile unsigned char *)0x32)
#define PIND		(*(volatile unsigned char *)0x30)

#define SET_BIT(reg, bit)			(reg |= (1 << bit))
#define READ_BIT(reg, bit)			((reg >> bit) & 1)
#define CLR_BIT(reg, bit)			(reg &= ~(1 << bit))
#define TOGGLE_BIT(reg, bit)		(reg ^= (1 << bit))


void SEGMENT_BCD(char n){
	
	PORTC = (n % 10) | ((n / 10) << 4);
	
}

int main(void)
{
	char time = 20, oldTime = time;
	char i = time, stop_flag = 0, adjust_flag = 0, reset_flag = 0, x = time;
	DDRA = 0;
	PORTA = 0x0f;
	DDRC = 0xff;
	DDRD = 1;
	while (1)
	{
		
		if (!READ_BIT(PINA, 3))
		{
			while(!READ_BIT(PINA, 3));
			adjust_flag ^= 1;
		}
		
		if (adjust_flag == 0)
		{
			// Normal Mode

			if (!READ_BIT(PINA, 0))
			{
				while(!READ_BIT(PINA, 0));
				stop_flag ^= 1;
			}
			
			if (!READ_BIT(PINA, 2))
			{
				while(!READ_BIT(PINA, 2));
				reset_flag ^= 1;
			}
			
			if (reset_flag == 1)
			{
				PORTD = 0;
				i = oldTime;
				reset_flag = 0;
				stop_flag = 0;
				adjust_flag = 0;
			}
					
			if (stop_flag == 0)
			{
				SEGMENT_BCD(i);
				i--;
				if (i == 255)
				{
					 i = 0;
					SET_BIT(PORTD, 0);
					stop_flag = 1;
				}
				_delay_ms(300);
			}
			else{
				SEGMENT_BCD(i);
			}
		}
		
		else{
			// Adjusting Mode

			SEGMENT_BCD(x);
			if (!READ_BIT(PINA, 1))
			{
				while(!READ_BIT(PINA, 1));
				x++;
				if (x > 99)
				{
					x = 0;
				}
			}
			
			if (!READ_BIT(PINA, 2))
			{
				while(!READ_BIT(PINA, 2));
				x--;
				if (x == 255)
				{
					x = 99;
				}
			}
			
			if (!READ_BIT(PINA, 0))
			{
				while(!READ_BIT(PINA, 0));
				oldTime = time;
				time = x;
				i = x;
				adjust_flag = 0;
			}
		}
		
	}
}