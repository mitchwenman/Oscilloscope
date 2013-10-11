#include "ADC.h"

#define RCGCADC (unsigned long*) 0x400FE638
#define ADCACTSS (unsigned long*) 0x40038000
#define SAMPLER 0x01;
#define ADCEMUX (unsigned long*) 0x40038014
#define ADCSSMUX0 (unsigned long*) 0x40038040
#define ADCSSCTL0 (unsigned long*) 0x40038044
#define ADCIM (unsigned long*) 0x40038008

#define NVIC_EN0 (unsigned long*) 0xE000E100


#define RCGCGPIO (unsigned long*) 0x400FE608
#define GPIOE_AFSEL (unsigned long*) 0x40024420
#define GPIOE_DEN (unsigned long*) 0x4002451C
#define GPIOE_AMSEL (unsigned long*) 0x40024528

void setup(void);
void _setupGPIO(void);
void _enableADC(void);

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
	*NVIC_EN0 |= 16384; //enable IRQ 30
	
	_enableADC();
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

void _enableADC(void)
{
	*ADCACTSS |= 0x01;
}
	