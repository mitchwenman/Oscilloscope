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
	setTimerReload(16000000);
	*GPTMIMR |= 0x0100; //turn on timer interrupts
	
	
	
	*GPTMCTL |= 0x100;
	*RCGCTIMER |= 0x01; //enable timer
	
}

void setTimerReload(int reload)
{
	*GPTMTBILR = reload;
}