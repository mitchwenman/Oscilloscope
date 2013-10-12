#include "Timer.h"

void setupTimer(void);
void setTimerReload(int reload);

void setupTimer(void) 
{
	*RCGCTIMER |= 0x1; //disable timer
	*GPTMCTL &= ~0x0100;
	*GPTMCFG |= 0x4;
	*GPTMTBMR |= 0x02; //set timer to periodic mode
	*GPTMCTL |= 0x1000; //set timer b to trigger adc
	*GPTMTBILR = 16000000;
	
	*GPTMCTL |= 0x2100; //enable timer and adc trigger
	*RCGCTIMER |= 0x01; //startTimer
	
}

/*
 * Stops timer, changes reload value, resets and restarts timer
*/
void setTimerReload(int reload)
{
	
	*GPTMTBILR = reload;
}