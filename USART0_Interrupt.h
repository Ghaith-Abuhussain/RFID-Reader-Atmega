/*
 * USART1_Interrupt.h
 *
 * Created: 3/30/2022 4:55:20 PM
 *  Author: DELL
 */ 


#ifndef USART0_INTERRUPT_H_
#define USART0_INTERRUPT_H_
#define F_CPU 16000000UL						/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>							/* Include AVR std. library file */
#include <avr/interrupt.h>
#include "USART0_Interrupt.h"

#define BAUD_PRESCALE (F_CPU / 16 / BAUDRATE-1) //(((F_CPU / (BAUDRATE * 16UL))) - 1)	/* Define prescale value */

void USART0_Init(unsigned long);				/* USART initialize function */
char USART0_RxChar();						/* Data receiving function */
void USART0_TxChar(char);					/* Data transmitting function */
void USART0_SendString(char*);				/* Send string of USART data function */
void USART0_SendString_With_Num_Bytes(char str[], int length);

#endif /* USART1_INTERRUPT_H_ */