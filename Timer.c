#include "Timer.h"

void setupTimer(void);
int getSampleRate(void);
void _setTimerReload(int reload);

									/****** Constants ******/
const int MAXSAMPLERATE = 170;
const int MINSAMPLERATE = 10;
const int CYCLESPERSECOND = 16000000; //clock cycles persecond

									/****** Static variables *******/
static int sampleRate = 10;

									/****** "Public" Methods ******/


void setupTimer(void) 
{
	*RCGCTIMER |= 0x1; 
	*GPTMCTL &= ~0x01; //disable timer
	*GPTMCFG &= ~0x7; //set to 32bit mode
	*GPTMTAMR |= 0x02; //set timer to periodic mode
	*GPTMTAILR = CYCLESPERSECOND/sampleRate; //set reload value
	
	*GPTMCTL |= 0x21; //enable timer and adc trigger
	*RCGCTIMER |= 0x1; //startTimer
	
}

int getSampleRate(void)
{
	return sampleRate;
}

void increaseSampleRate(void)
{
	if (!(sampleRate == MAXSAMPLERATE))
	{
		sampleRate += 10;
		_setTimerReload(CYCLESPERSECOND/sampleRate);
	}
}

void decreaseSampleRate(void)
{
	if (!(sampleRate == MINSAMPLERATE))
	{
		sampleRate -= 10;
		_setTimerReload(CYCLESPERSECOND/sampleRate);
	}
}

/*
 * Stops timer, changes reload value, resets and restarts timer
*/
void _setTimerReload(int reload)
{
	*GPTMCTL &= ~0x01;
	*GPTMTAILR = reload;
	*GPTMCTL |= 0x01;
}