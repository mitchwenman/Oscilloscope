#define RCGCTIMER (unsigned long*) 0x400FE604
#define GPTMCTL (unsigned long*) 0x4003000C
#define GPTMCFG (unsigned long*) 0x40030000
#define GPTMTBMR (unsigned long*) 0x40030008
#define GPTMTBILR (unsigned long*) 0x4003002C

void setupTimer();
void setTimerReload(int reload);

void setupTimer(void) 
{
	*RCGCTIMER |= 0x01; //disable timer
	*GPTMCTL &= ~0x01;
	*GPTMCFG |= 0x4;
	*GPTMTBMR |= 0x02; //set timer to periodic mode
	*GPTMCTL |= 0x1000; //set timer b to trigger adc
	setTimerReload(100000);
	*RCGCTIMER |= 0x01; //enable timer
	
}

void setTimerReload(int reload)
{
	*GPTMTBILR = reload;
}