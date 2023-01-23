/*
 * USART1_Interrupt.c
 *
 * Created: 3/30/2022 4:55:41 PM
 *  Author: DELL
 */ 
#include "USART0_Interrupt.h"
void USART0_Init(unsigned long BAUDRATE)				/* USART initialize function */
{	
	/*
	UCSRB = 0x00; //disable while setting baud rate
	UCSRA = 0x00;
	UCSRC = 0x06;
	UBRRL = BAUD_PRESCALE;							
	UBRRH = (BAUD_PRESCALE >> 8);
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
	*/
	
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	UBRRH=0x00;
	UBRRL=0x08;
	sei();
	
}

char USART0_RxChar()									/* Data receiving function */
{
	while (!(UCSRA & (1 << RXC)));					/* Wait until new data receive */
	return(UDR);									/* Get and return received data */
}

void USART0_TxChar(char data)						/* Data transmitting function */
{									/* Write data to be transmitting in UDR */
	while (!(UCSRA & (1<<UDRE)));					/* Wait until data transmit and buffer get empty */
	UDR = data;	
}

void USART0_SendString(char *str)					/* Send string of USART data function */
{
	int m=0;
	while (str[m]!=0)
	{
		USART0_TxChar(str[m]);						/* Send each char of string till the NULL */
		m++;
	}
}

void USART0_SendString_With_Num_Bytes(char str[], int length)					/* Send string of USART data function */
{
	int m=0;
	for (m = 0; m < length; m++)
	{
		USART0_TxChar(str[m]);						/* Send each char of string till the NULL */
	}
}