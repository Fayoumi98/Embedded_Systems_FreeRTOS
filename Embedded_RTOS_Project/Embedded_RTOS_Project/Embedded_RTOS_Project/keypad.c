/*
 * keypad.c
 *  Author: Fayoumi
 */ 

#include "include/keypad.h"


const int8_t keypad_matrix[] = {
	'7','8','9','/',
	'4','5','6','*',
	'1','2','3','-',
	'A','0','=','+'
};


void keypad_init(void)
{
	INIT_KEYPAD();
}



int8_t keypad_read(void)
{
	uint8_t i ;
	for(i = 0 ; i < 4 ; i++ )
	{
		switch(i)
		{
			case 0 : KP0(0); KP1(1); KP2(1); KP3(1); break;
			case 1 : KP0(1); KP1(0); KP2(1); KP3(1); break;
			case 2 : KP0(1); KP1(1); KP2(0); KP3(1); break;
			case 3 : KP0(1); KP1(1); KP2(1); KP3(0); break;
		}
		
		if(KP4() == 0) return keypad_matrix[0 + 4*i];
		if(KP5() == 0) return keypad_matrix[1 + 4*i];
		if(KP6() == 0) return keypad_matrix[2 + 4*i];
		if(KP7() == 0) return keypad_matrix[3 + 4*i];
		
		_delay_ms(1);
	}
	
	// if not pressed return -1 
	return -1;	
}

