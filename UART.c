#include "UART.h"
#include <stdio.h>

									/****** Method Declarations ******/
void drawValue(int value);
void setupUART(void);
void printString(char str[]);
int _calculateYPos(int value);
void _outChar(unsigned char c);
void _setCursorPosition(int x, int y);
void _clearWindow(void);
									/****** Static variables *******/
int yScale = 1;
int xPos = 0;

									/****** Constants ******/
const int WIDTH = 50;
const int HEIGHT = 20;
const char ESC = 27;
const int POINTFIVEVOLTS = 409;

									/****** "Public" Methods ******/
void drawValue(int value)
{
	_setCursorPosition(xPos, _calculateYPos(value));
	printString(".");
	xPos = (xPos + 1) % WIDTH; //start back at first row if at edge
	if (xPos == 0)
	{
		_clearWindow();
	}
	
}


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
		
}

void printString(char str[])
{
	int i = 0;
	//_outChar(13);
	while (str[i] != '\0')
	{
		_outChar(str[i]);
		++i;
	}
}

								/****** "Private" Methods ******/
int _calculateYPos(int y)
{
	return HEIGHT - y * yScale/POINTFIVEVOLTS;
}

void _setCursorPosition(int x, int y)
{
	char str[6];
	sprintf(str, "%c[%i;%if", ESC, y,x); //because the terminal is ass backward
	printString(str);
}

void _clearWindow(void)
{
	char str[5];
	sprintf(str, "%c[2J", ESC);
	printString(str);
}

void _outChar(unsigned char c)
{
	//while tx reg full wait
	while ((UART0_FR_R & 0x20) != 0) {;}
	UART0_DR_R = c;
}

