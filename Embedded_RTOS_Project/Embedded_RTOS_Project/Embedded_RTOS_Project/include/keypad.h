/*
 * keypad.h
 * Author : Fayoumi
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

/*

-------------------------
|     |     |     |     | ---------- KP0
-------------------------
|     |     |     |     | ---------- KP1
-------------------------
|     |     |     |     | ---------- KP2
-------------------------
|     |     |     |     | ---------- KP3
-------------------------
|     |     |     | PULL UP
|     |     |     |
KP4   KP5   KP6   KP7

KP (0 TO 3) INPUT   ==> PC0,1,2,3  OUTPUT
KP (4 TO 7) OUTPUT  ==> PC4,5,6,7  INPUT PULL UPD

*/

#include "headers.h"


// PC0 ,PC1, PC2, PC3 output
// PC4 , PC5, PC6, PC7 input pull-up
#define INIT_KEYPAD() DDRC = 0x0f; PORTC = 0xff;


// Set Keypad Inputs
#define KP0(val) if(val == 0)  CLRBIT(PORTC,0); else  SETBIT(PORTC,0);
#define KP1(val) if(val == 0)  CLRBIT(PORTC,1); else  SETBIT(PORTC,1);
#define KP2(val) if(val == 0)  CLRBIT(PORTC,2); else  SETBIT(PORTC,2);
#define KP3(val) if(val == 0)  CLRBIT(PORTC,3); else  SETBIT(PORTC,3);


// Read Keypad Outputs
#define KP4()  READBIT(PINC,4)
#define KP5()  READBIT(PINC,5)
#define KP6()  READBIT(PINC,6)
#define KP7()  READBIT(PINC,7)


/*****************************************************************************
* Function Name: keypad_init
* Purpose      : initialize keypad pins
* Parameters   : void
* Return value : void
*****************************************************************************/
void keypad_init(void);


/*****************************************************************************
* Function Name: keypad_init
* Purpose      : check if one of keypad button is pressed
* Parameters   : void
* Return value : return the button value in case a button is pressed
*                return -1 in case no button pressed
*****************************************************************************/
int8_t keypad_read(void);


#endif /* KEYPAD_H_ */