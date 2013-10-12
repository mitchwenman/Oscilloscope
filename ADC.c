#include "ADC.h"
#include "GPIO.h"
#include "NVIC.h"
#include <stdio.h>
#include <stdlib.h>

void setup(void);
void increaseSampleRate(void);
void decreaseSampleRate(void);
void _setupGPIO(void);
void _enableADC(void);
float _convertToVolts(int adcReading);


extern void printString(char str[]);
extern void drawValue(int value);

void setupADC(void)
{
	*RCGCADC |= 0x01; //Activate clock to ADC0
	*RCGC0 |= 0x10000; //active clock on legacy register (doesn't fix it)
	_setupGPIO();
	
	*ADCACTSS &= ~SAMPLER; //Disable Sampler number
	*ADCEMUX |= 0x05; //Sets sampler 0 trigger to timer triggered
	*ADCSSCTL0 |= 0x6;//Sets first sample in sequence to be end of sequence
										//and first sample to generate interrupt
	*ADCIM |= 0x1; //sampler 0 triggers interrupt
	*ADCACTSS |= SAMPLER; //enable the sampler
	*NVIC_EN0 |= 16384; //enable IRQ 14
	
	_enableADC();
}

void increaseSampleRate(void)
{
	
}

void decreaseSampleRate(void)
{
	
}


void _setupGPIO(void)
{
	int i;
	*RCGCGPIO |= 0x10; //activate clock for Port E
	
	for (i = 0; i < 2; i++) {;} //allow clock to settle
	*GPIOE_AFSEL |= 0x8; //set afsel for pin 3
	*GPIOE_DEN &= ~0x8; //disable digital functions for pin 3
	*GPIOE_AMSEL |= 0x8; //set analogue isolation for pin 3
}

void adcISR(void)
{
	char str[5];
	int result = *ADCSSFIFO0;
	*ADCISC |= 0x0F; //clear interrupt
	sprintf(str, "%i", result);
	drawValue(result);
}

float _convertToVolts(int adcReading)
{
	return ((float) adcReading)/4096 * 5;
}

void _enableADC(void)
{
	*ADCACTSS |= 0x01;
}
	