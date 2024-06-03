/*
* lcd.c
 *  Author: Fayoumi
 */

#include "include/LCD.h"

void LCD_init(void)
{
	// Step 1 : initialize all pins as output (D7,D6,D5,D4,RS,EN)
	INIT_LCD_PINS();
	// Send command 0x3 and wait for 4 mSec (repeat this process 3  times)
	LCD_write_command(0x3);
	DELAY_MS(4);
	LCD_write_command(0x3);
	DELAY_MS(4);
	LCD_write_command(0x3);
	DELAY_MS(4);
	// Send command 0x2 to enable 4 bit mode
	LCD_write_command(0x2);
	// Send command 0x28 to enable both lines to write on
	LCD_write_command(0x28);
	// Send command 0x08 to disable cursor display (Optional)
	LCD_write_command(0x08);
	// Send command 0x01 to clear LCD
	LCD_write_command(0x01);
	// Send command 0x06 to to make cursor increment to right
	LCD_write_command(0x06);
	LCD_write_command(0x01);
	// Send command 0x0C to turn on display
	LCD_write_command(0x0C);
	// Wait for 20 mSec
	DELAY_MS(20);	
}

void LCD_write_command(uint8_t cmd)
{
	// Sending Command
	RS(0);
	
	// Send Most Significant Nibble of command
	D7(READBIT(cmd,7));
	D6(READBIT(cmd,6));
	D5(READBIT(cmd,5));
	D4(READBIT(cmd,4));
	
	TRIGGER_LCD();
	
	// Send Least Significant Nibble of command
	D7(READBIT(cmd,3));
	D6(READBIT(cmd,2));
	D5(READBIT(cmd,1));
	D4(READBIT(cmd,0));
		
	TRIGGER_LCD();
		
}

void LCD_write_char(uint8_t data)
{
	// Sending Data Mode
	RS(1);

	// Send Most Significant Nibble of Data
	D7(READBIT(data,7));
	D6(READBIT(data,6));
	D5(READBIT(data,5));
	D4(READBIT(data,4));
		
	TRIGGER_LCD();

	// Send Least Significant Nibble of Data
	D7(READBIT(data,3));
	D6(READBIT(data,2));
	D5(READBIT(data,1));
	D4(READBIT(data,0));
		
	TRIGGER_LCD();
}

void LCD_write_string(uint8_t* txt)
{
	uint8_t i = 0;
	
	while(txt[i] != '\0')
	{
		LCD_write_char(txt[i]);
		i++;
	}
}

void LCD_write_num(uint32_t num)
{
/*	
	Example: num = 1234
	1234 % 10 --- 4
	123 %  10 ----3
	12  %  10 ----2
	1   %  10 ----1
    0
*/
	int8_t i = 0;
	uint8_t num_arr[10];
	
	if(num == 0) LCD_write_char('0');
	
	for(i = 0; num != 0 ;i++)
	{
		// + 48  to convert decimal number into ASCII
		num_arr[i] = num % 10 + 48;
		num /= 10;
	}
	
	i--;
	while (i >= 0)
	{
		LCD_write_char(num_arr[i]);
		i--;
	}
}