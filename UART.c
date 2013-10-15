#include "UART.h"
#include "NVIC.h"
#include <stdio.h>
#include <stdlib.h>
									/****** Method Declarations ******/
void drawLoop(void);
void drawValue(int value);
void setupUART(void);
void _printStringAtPosition(char str[], int x, int y);
void _printString(char str[]);
void receiveISR(void);


int _calculateYPos(int value);
void _outChar(unsigned char c);
void _setCursorPosition(int x, int y);
void _clearWindow(void);
void _printXScale(void);
void _printYScale(void);
void _increaseScale(void);
void _decreaseScale(void);
void _setColour(int colour);
void _clearGraph(void);


extern int dequeue(void);
extern void increaseSampleRate(void);
extern void decreaseSampleRate(void);
extern int getSampleRate(void);

									/****** Variables *******/
float yScaleValues[] = {1, 2, 4, 8};
int yScale = 0;
int xPos = 5;
int colour = 0;

									/****** Constants ******/
const int WIDTH = 80;
const int HEIGHT = 20;
const char ESC = 27;
const int POINTFIVEVOLTS = 409;
const float MAXYSCALE = 3;
const float MINYSCALE = 0;
const int XINDENT = 4;
const int YINDENT = 5;

extern const int CYCLESPERSECOND;

									/****** "Public" Methods ******/
void drawLoop(void)
{
	_printXScale();
	_printYScale();
	while (1)
	{
		int value = dequeue();
		if (value != NULL)
		{
			drawValue(value);
		}
		free(&value);
	}
}

void drawValue(int value)
{
	int y = _calculateYPos(value);
	if (y > 0)
	{
		_printStringAtPosition(".", xPos, y);
	}
	xPos = (xPos + 1) % (WIDTH - 4) ; //start back at first row if at edge
	if (xPos == 0)
	{
		_clearGraph();
		xPos = XINDENT + 1;

	}
	free(&y);
	
	
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
	//testing higher baudrates
	UART0_IBRD_R = 0x1;	
	UART_FBRD_R = 0x28;	
	//UART0_IBRD_R = 0x8;
	//UART_FBRD_R = 0x2C;
	UART0_CTRH_R |= 0x60;
	UARTIM |= 0x10; //enable receive interrupt
	*NVIC_EN0 |= 0x20; 
	UART0_CTL_R |= 0x301;
		
}

void receiveISR(void)
{
	char received = UART0_DR_R;
	if (received == 'A')
			_increaseScale();
	else if (received == 'Z')
			_decreaseScale();
	else if (received == '>')
	{
		increaseSampleRate();
		_clearWindow();
	}
	else if (received == '<')
	{
		decreaseSampleRate();
		_clearWindow();
	}
	else if (received == 'C')
	{
		colour++;
		_setColour(colour);
	}
}



								/****** "Private" Methods ******/
void _printStringAtPosition(char str[], int x, int y)
{
	_setCursorPosition(x, y);
	_printString(str);
}


void _printString(char str[])
{
	int i = 0;
	//_outChar(13);
	while (str[i] != '\0')
	{
		_outChar(str[i]);
		++i;
	}
	free(&i);
}

int _calculateYPos(int y)
{
	return HEIGHT - y/POINTFIVEVOLTS * yScaleValues[yScale];
}

void _setCursorPosition(int x, int y)
{
	char str[6];
	sprintf(str, "%c[%i;%iH", ESC, y,x); //column;row
	_printString(str);
	free(str);
}

void _clearWindow(void)
{
	char str[5];
	_setColour(0);
	sprintf(str, "%c[2J", ESC);
	_printString(str);
	
	free(str);
	_printXScale();
	_printYScale();
	_setColour(colour);
}

void _outChar(unsigned char c)
{
	//while tx reg full wait
	while ((UART0_FR_R & 0x20) != 0) {;}
	UART0_DR_R = c;
}

void _printXScale(void)
{
	int i;
	char str[4];
	for (i = XINDENT + 1; //so it doesn't overwrite y scale label
				i < WIDTH - 4; i++)
	{
		_printStringAtPosition("|", i, HEIGHT + 1);
	}
	sprintf(str, "1/%is", getSampleRate());
	_printStringAtPosition(str, (WIDTH - 6)/2, HEIGHT + 2);
	
}

void _printYScale(void)
{
	int i;
	char str[4];
	int y;
	for (i = YINDENT; i <= HEIGHT; i++) //Print outline
	{
		_printStringAtPosition("-", XINDENT, i);
	}
	
	for (i = 0; i <= 8; i++) //print scale values
	{
		sprintf(str, "%iV", i);
		y = _calculateYPos(POINTFIVEVOLTS * i * 2);
		if (y < YINDENT) break; //gone higher than y label outline
		_printStringAtPosition(str, XINDENT - 3, y);
	}
	free(&i);
	free(str);
	free(&y);
	
}

void _increaseScale()
{
	if (!(yScale == MAXYSCALE))
	{
		yScale++;
		_clearWindow();
	}
}

void _decreaseScale()
{
	if (!(yScale == MINYSCALE))
	{
		yScale--;
		_clearWindow();
	}
		
}

void _setColour(int colour)
{
	char str[5];
	colour = colour % 4;
	if (colour == 0)
		sprintf(str, "%c[39m", ESC);
	else if (colour == 1)
		sprintf(str, "%c[31m", ESC);
	else if (colour == 2)
		sprintf(str, "%c[32m", ESC);
	else if (colour == 3)
		sprintf(str, "%c[34m", ESC);
	
	if (str[0] == ESC)
		_printString(str);
	
	
}

void _clearGraph(void)
{
	int i;
	char str[4];
	sprintf(str, "%c[K", ESC);
	for (i = YINDENT - 1; i <= HEIGHT; i++) //Print outline
	{
		_printStringAtPosition(str, XINDENT + 1, i);
	}
	
}