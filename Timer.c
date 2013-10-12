#include "Timer.h"

void setupTimer(void);
void setTimerReload(int reload);

void setupTimer(void) 
{
	*RCGCTIMER |= 0x1; 
	*GPTMCTL &= ~0x01; //disable timer
	*GPTMCFG &= ~0x7; //set to 32bit mode
	*GPTMTAMR |= 0x02; //set timer to periodic mode
	*GPTMTAILR = 160000; //set reload value
	
	*GPTMCTL |= 0x21; //enable timer and adc trigger
	*RCGCTIMER |= 0x1; //startTimer
	
}

/*
 * Stops timer, changes reload value, resets and restarts timer
*/
void setTimerReload(int reload)
{
	
	*GPTMTAILR = reload;
}