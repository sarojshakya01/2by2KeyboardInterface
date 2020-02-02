/* Keypad 2.c
 * 
 * Created: 6/6/2014 4:55:52 AM
 * Author: Saroj Shakya
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define KEY_PAD_ROW PORTC
#define KEY_PAD_COLUMN PINC
#define KEY_PAD_DDR DDRC
//---LCD Port Definition---//
#define LCD_DATA_PORT PORTD
#define LCD_DATA_DDR DDRD
#define LCD_RS 2
#define LCD_EN 3
int main();
//-------------------------//
//---LCD Code starts from here---//
void LCD_cmnd(unsigned char cmnd) //function to send command to LCD Module
{
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd & 0xF0); //send upper 4 bit
	LCD_DATA_PORT &= ~(1<<LCD_RS); //0b11111011 //RS = 0
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000 //EN = 1
	_delay_us(50);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111 //EN = 0
	_delay_us(200);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd << 4); //send lower 4 bit
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000 //EN = 1
	_delay_us(50);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111 //EN = 0
}
void LCD_data(unsigned char data) //Function to send data to LCD Module
{
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
	LCD_DATA_PORT |= 1<<LCD_RS; //0b00000100 //RS = 1
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000
	_delay_us(50);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111
	_delay_us(2000);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data << 4);
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000
	_delay_us(2000);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111
}
void LCD_initialize(void) //Function to initialize LCD Module
{
	LCD_DATA_DDR = 0xFC;
	LCD_DATA_PORT &= ~(0xC1<<LCD_EN); //0b00110111;
	_delay_ms(200);
	LCD_cmnd(0x33);
	_delay_ms(20);
	LCD_cmnd(0x32);
	_delay_ms(20);
	LCD_cmnd(0x28);
	_delay_ms(20);
	LCD_cmnd(0x0E);
	_delay_ms(20);
	LCD_cmnd(0x01);
	_delay_ms(20);
}
void LCD_clear(void) //Function to clear the LCD Screen
{
	LCD_cmnd(0x01);
	_delay_ms(2);
}
void LCD_print(char * str) //Function to print the string on LCD Screen
{
	unsigned char i=0;
	while(str[i] != 0)
	{
		LCD_data(str[i]);
		i++;
		_delay_ms(5);
	}
}
void LCD_set_curser(unsigned char y, unsigned char x) //Function to move the Curser at (y,x) position
{
	if(y==1)
	LCD_cmnd(0x7F+x);
	else if(y==2)
	LCD_cmnd(0xBF+x);
}
void LCD_num(unsigned char num) //Function to display number
{
	//LCD_data(num/100 + 0x30);
	//num = num%100;
	LCD_data(num/10 + 0x30);
	LCD_data(num%10 + 0x30);
}
//---LCD Code ends here---//

void port_initialize()
{
	KEY_PAD_DDR = 0x0F;		//Key-board port, higer nibble - input, lower nibble - output
	KEY_PAD_ROW = 0xFF;
}

void input()
{
	unsigned char i,column_value,keyCode,keyPressed=0;
	column_value = 0xFF;
	for(i=0; i<4; i++)
	{
		_delay_ms(1);
		KEY_PAD_ROW= ~(0x01 << i);
		_delay_ms(1);  		  	 		  //delay for port o/p settling
		column_value = KEY_PAD_COLUMN | 0x0F;
		
		if (column_value != 0xFF)
		{
			_delay_ms(20); 		  		 //for de-bounce
			column_value = KEY_PAD_COLUMN | 0x0F;
			if(column_value == 0xFF) goto OUT;
			
			keyCode = (column_value & 0xF0) | (0x0F & ~(0x01 << i));
			
			while (column_value != 0xFF)
			column_value = KEY_PAD_COLUMN | 0x0F;
			
			_delay_ms(20);   			   //for de-bounce
			
			switch (keyCode)
			{
				case (0xEE): keyPressed = '*';
				break;
				case (0xED): keyPressed = '7';
				break;
				case (0xEB): keyPressed = '4';
				break;
				case (0xE7): keyPressed = '1';
				break;
				case (0xDE): keyPressed = '0';
				break;
				case (0xDD): keyPressed = '8';
				break;
				case (0xDB): keyPressed = '5';
				break;
				case (0xD7): keyPressed = '2';
				break;
				case (0xBE): keyPressed = '#';
				break;
				case (0xBD): keyPressed = '9';
				break;
				case (0xBB): keyPressed = '6';
				break;
				case (0xB7): keyPressed = '3';
				break;
				case (0x7E): keyPressed = 'D';
				break;
				case (0x7D): keyPressed = 'C';
				break;
				case (0x7B): keyPressed = 'B';
				break;
				case (0x77): keyPressed = 'A';
				break;
				default	   : keyPressed = 'X';
			}
			if (keyPressed=='1')
			{
				LCD_data('1');
			}
			if (keyPressed=='2')
			{
				LCD_data('2');
			}
			if (keyPressed=='3')
			{
				LCD_data('3');
			}
			if (keyPressed=='4')
			{
				LCD_data('4');
			}
			if (keyPressed=='5')
			{
				LCD_data('5');
			}
			if (keyPressed=='6')
			{
				LCD_data('6');
			}
			if (keyPressed=='7')
			{
				LCD_data('7');
			}
			if (keyPressed=='8')
			{
				LCD_data('8');
			}
			if (keyPressed=='9')
			{
				LCD_data('9');
			}
			if (keyPressed=='0')
			{
				LCD_data('0');
			}
			if (keyPressed=='A')
			{
				LCD_data('A');
			}
			if (keyPressed=='B')
			{
				LCD_data('B');
			}
			if (keyPressed=='C')
			{
				LCD_data('C');
			}
			if (keyPressed=='D')
			{
				LCD_data('C');
			}
			if (keyPressed=='#')
			{
				LCD_data('D');
			}
			if (keyPressed=='*')
			{
				LCD_data('*');
			}
		}
		OUT:;
	}
}

int main()
{
	LCD_initialize();
	port_initialize();
	LCD_cmnd(0x0C);
	LCD_print("WELCOME");
	_delay_ms(1000);
	LCD_clear();
	while(1){
		input();	
	}
}