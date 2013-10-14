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

extern int dequeue(void);
extern void increaseSampleRate(void);
extern void decreaseSampleRate(void);
extern int getSampleRate(void);

									/****** Static variables *******/
float yScaleValues[] = {1, 2, 4, 8};
int yScale = 2;
int xPos = 4;

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
	xPos = (xPos + 1) % WIDTH; //start back at first row if at edge
	if (xPos == 0)
	{
		_clearWindow();
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
	UART0_IBRD_R = 0x8;
	UART_FBRD_R = 0x2C;
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
		increaseSampleRate();
	else if (received == '<')
		decreaseSampleRate();
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
	sprintf(str, "%c[2J", ESC);
	_printString(str);
	free(str);
	_printXScale();
	_printYScale();
}

void _outChar(unsigned char c)
{
	//while tx reg full wait
	while ((UART0_FR_R & 0x20) != 0) {;}
	UART0_DR_R = c;
}

void _printXScale(void)
{
	
}

void _printYScale(void)
{
	int i;
	char str[4];
	int y;
	for (i = YINDENT; i <= HEIGHT; i++)
	{
		_printStringAtPosition("-", XINDENT, i);
	}
	
	for (i = 0; i <= 8; i++)
	{
		sprintf(str, "%iV", i);
		y = _calculateYPos(POINTFIVEVOLTS * i * 2);
		if (y < YINDENT) break; //gone higher than y label outline
		_printStringAtPosition(str, XINDENT - 3, y);
	}
	
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
