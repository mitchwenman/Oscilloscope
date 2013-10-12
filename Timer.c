#define RCGCTIMER (unsigned long*) 0x400FE604
#define GPTMCTL (unsigned long*) 0x4003000C
#define GPTMCFG (unsigned long*) 0x40030000
#define GPTMTBMR (unsigned long*) 0x40030008
#define GPTMTBILR (unsigned long*) 0x4003002C
#define GPTMIMR (unsigned long*) 0x40030018


void setupTimer();
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