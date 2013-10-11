#include "UART.h"

void setupUART(void);
unsigned char getChar(void);
void printString(char str[]);
void _outChar(unsigned char c);

void setupUART(void)
{
	int i = 0;
	SYSCTL_RCGUART_R |= 0x1;
	for (i =0; i < 3; i++) {;}
	SYSCTL_RCGC2 |= 0x1;
	for (i =0; i < 3; i++) {;}
	UART0_CTL_R &= ~0x1;
	
	GPIO_PORTA_AFSEL |= 0x03;
	GPIO_PORTA_DEN_R |= 0x03;
	GPIO_PORTA_PCTL_R |= 0x11;
	UART0_IBRD_R = 0x8;
	UART_FBRD_R = 0x2C;
	UART0_CTRH_R |= 0x60;
	UART0_CTL_R |= 0x301;
		
	//test UART
	_outChar('t');
}

void printString(char str[])
{
	int i = 0;
	while (str[i] != '\0')
	{
		_outChar(str[i]);
		++i;
	}
}

unsigned char getChar(void)
{
	int i;
	//While DR not full do nothing
	while ((UART0_FR_R & 0x10) != 0) {;}
	for (i =0; i < 3; i++) {;}
	return UART0_DR_R; //return char in DR
}

void _outChar(unsigned char c)
{
	//while tx reg full wait
	while ((UART0_FR_R & 0x20) != 0) {;}
	UART0_DR_R = c;
}

