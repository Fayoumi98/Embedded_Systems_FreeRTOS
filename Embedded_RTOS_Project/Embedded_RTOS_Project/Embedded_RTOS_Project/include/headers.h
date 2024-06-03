// garding header to prevent multiple definitions
#ifndef HEADERS_H_
#define HEADERS_H_


// this file include all register addresses macros like DDRA
#include <avr/io.h>
// set the frequency of micro-controller
// UL = when writing a large number
#define F_CPU 16000000UL
// this header include all delay function like _delay_ms
#include <util/delay.h> 
// this header is necessary when you deal with interrupt
#include <avr/interrupt.h> 

// Helping macros
#define SETBIT(REG,BIT) (REG |= 1<<BIT)
#define CLRBIT(REG,BIT) (REG &= ~(1<<BIT))
#define TOGBIT(REG,BIT) (REG ^= 1<<BIT)
#define READBIT(REG,BIT)  ((REG >> BIT) & 1)


#endif /* HEADERS_H_ */