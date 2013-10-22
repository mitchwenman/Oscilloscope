#include "ADC.h"
#include "GPIO.h"
#include "NVIC.h"
#include <stdio.h>
#include <stdlib.h>

									/****** Method Declarations ******/

void setup(void);
void increaseSampleRate(void);
void decreaseSampleRate(void);
void _setupGPIO(void);
void _enableADC(void);

extern void setTimerReload(int reload);
extern void enqueue(int elem);
extern void printString(char str[]);

//Configures the ADC - PE3, Sampler 0, ADC0
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
	
	*NVIC_EN0 |= 16384; //enable IRQ 14
	
	_enableADC();
}


//Retrieves the converted value from the ADC FIFO
//and puts it in the buffer.
void adcISR(void)
{
	int result = *ADCSSFIFO0;
	*ADCISC |= 0x0F; //clear interrupt
	enqueue(result);
	
	
}

//Sets up the GPIO port for voltage input
void _setupGPIO(void)
{
	int i;
	*RCGCGPIO |= 0x10; //activate clock for Port E
	
	for (i = 0; i < 2; i++) {;} //allow clock to settle
	*GPIOE_AFSEL |= 0x8; //set afsel for pin 3
	*GPIOE_DEN &= ~0x8; //disable digital functions for pin 3
	*GPIOE_AMSEL |= 0x8; //set analogue isolation for pin 3
}



//enables the adc
void _enableADC(void)
{
	*ADCACTSS |= 0x01;
}
	
