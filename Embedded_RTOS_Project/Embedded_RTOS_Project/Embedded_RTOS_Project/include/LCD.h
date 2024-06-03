/*
 * LCD.h
 *  Author: Fayoumi
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "headers.h"

/*
	D7: PORTA 7
	D6: PORTA 6
	D5: PORTA 5
	D4: PORTA 4
	RS: PORTA 3
	EN: PORTA 2
*/

// Set last 6 bits of Port A as output in a symbolic constant (Macro)
// Set initial value zeros except last 2 bits
#define INIT_LCD_PINS() DDRA |= 0b11111100; \
						PORTA &= 0b00000011;
						
#define D7(val) if(val == 0)  CLRBIT(PORTA,7); else  SETBIT(PORTA,7);
#define D6(val) if(val == 0)  CLRBIT(PORTA,6); else  SETBIT(PORTA,6);
#define D5(val) if(val == 0)  CLRBIT(PORTA,5); else  SETBIT(PORTA,5);
#define D4(val) if(val == 0)  CLRBIT(PORTA,4); else  SETBIT(PORTA,4);
#define RS(val) if(val == 0)  CLRBIT(PORTA,3); else  SETBIT(PORTA,3);
#define EN(val) if(val == 0)  CLRBIT(PORTA,2); else  SETBIT(PORTA,2);

#define TRIGGER_LCD() EN(1); _delay_ms(1); EN(0); _delay_ms(1);  

#define DELAY_MS(x) _delay_ms(x);


/*****************************************************************************
* Function Name: LCD_init
* Purpose      : initialize LCD to work in 4-bit mode -
* Parameters   : void
* Return value : void
*****************************************************************************/
void LCD_init(void);

/*****************************************************************************
* Function Name: LCD_write_command
* Purpose      : Send a specific command (cmd) to lcd (valid only with 4-bit mode)
* Parameters   : cmd  (command to be done by lcd)
* Return value : void
*****************************************************************************/
void LCD_write_command(uint8_t cmd);

/*****************************************************************************
* Function Name: LCD_write_char
* Purpose      : Write a specific ascii char (data) to lcd (valid only with 4-bit mode)
* Parameters   : data  (data to be written on lcd)
* Return value : void
*****************************************************************************/
void LCD_write_char(uint8_t data);

/*****************************************************************************
* Function Name: LCD_write_string
* Purpose      : Write a string (*txt) to lcd (valid only with 4-bit mode)
*                (assume the end char of string is NULL (Ascii:'\0' - decimal:0 ))
* Parameters   : *txt  (array of char to be written on lcd)
* Return value : void
*****************************************************************************/
void LCD_write_string(uint8_t* txt);

/*****************************************************************************
* Function Name: LCD_write_num
* Purpose      : Write a specific number (num) on lcd (valid only with 4-bit mode)
* Parameters   : num  (num to be written on lcd)
* Return value : void
*****************************************************************************/
void LCD_write_num(uint32_t num);


#endif /* LCD_H_ */