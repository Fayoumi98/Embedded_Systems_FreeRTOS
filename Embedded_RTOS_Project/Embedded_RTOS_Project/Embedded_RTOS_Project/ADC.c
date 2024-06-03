/*
 * ADC.c
 * Author : Fayoumi
 */ 

#include "include/ADC.h"


void ADC_init(void)
{
	// AVCC with external capacitor at AREF pin
	ADMUX |= 1 << REFS0;
	// ADEN: ADC Enable  ADEN = 1
	// ADPS2:0: ADC Pre-scaler Select Bits (128)		ADPS0 =	1		ADPS1 =	1		ADPS2 = 1
	
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}



uint16_t ADC_read(uint8_t channel)
{
	// channel select 
	// MUX4:0: Analog Channel and Gain Selection Bits
	ADMUX |= channel | (1 << REFS0);
	
	// Start conversion (ADSC: ADC Start Conversion)
	SETBIT(ADCSRA,ADSC);
	
	// wait until ADC Start Conversion  is turned into 0
	while(READBIT(ADCSRA,ADSC) == 1){}
	/*
	or :  wait until ADIF ADC Interrupt Flag  is turned into 1
	while(READBIT(ADCSRA,ADIF) == 0){}
	*/
	return ADC;
}