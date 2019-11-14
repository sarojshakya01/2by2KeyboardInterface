/* Keypad 2.c
 * 
 * Created: 6/6/2014 4:55:52 AM
 * Author: Saroj Shakya
 */ 

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define KEY_PORT PORTC
#define KEY_DDR DDRC
#define KEY_PIN PINC
//---LCD Port Definition---//
#define LCD_DATA_PORT PORTA
#define LCD_DATA_DDR DDRA
#define LCD_RS 2
#define LCD_EN 3
unsigned char col,row;
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

int main()
{
	KEY_DDR=0x0F;
	KEY_PORT=0xFF;
	LCD_initialize();
	LCD_cmnd(0x0C);
	LCD_print("WELCOME");
	_delay_ms(1000);
	while(1)
	{
		do
		{
			KEY_PORT&=0xF0;
			col=(KEY_PIN&0xF0);
		}while(col!=0xF0);
		do
		{
			do
			{
				_delay_ms(1000);
				col=(KEY_PIN&0xF0);
			} while (col==0xF0);
			
			_delay_ms(1000);
			col=(KEY_PIN&0xF0);
		} while (col==0xF0);
		
		while(1)
		{
			KEY_PORT=0xFE;
			col=(KEY_PIN&0xF0);
			if (col!=0xF0)
			{
				row=0;
				break;
			}
			KEY_PORT=0xFD;
			col=(KEY_PIN&0xF0);
			if (col!=0xF0)
			{
				row=1;
				break;
			}
			KEY_PORT=0xFB;
			col=(KEY_PIN&0xF0);
			if (col!=0xF0)
			{
				row=2;
				break;
			}
			KEY_PORT=0xF7;
			col=(KEY_PIN&0xF0);
			row=3;
			break;
		}
		if(col==0x0E)
		{
			LCD_print("A");
		}
		else if(col==0x0D)
		{
			LCD_print("B");
		}
		else if(col==0x0B)
		{
			LCD_print("C");
		}
		else
		{
			LCD_print("D");
		}	
	}
}