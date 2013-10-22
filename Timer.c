#include "Timer.h"

void setupTimer(void);
int getSampleRate(void);
void _setTimerReload(int reload);

									/****** Constants ******/
const int MAXSAMPLERATE = 400; //Max samples per second
const int MINSAMPLERATE = 10; //Min samples per second
const int CYCLESPERSECOND = 16000000; //clock cycles persecond

									/****** Static variables *******/
static int sampleRate = 10; //The current sample rate

									/****** "Public" Methods ******/

//Configures the timer - Period mode, 10Hz
void setupTimer(void) 
{
	*RCGCTIMER |= 0x1; 
	*GPTMCTL &= ~0x01; //disable timer
	*GPTMCFG &= ~0x7; //set to 32bit mode
	*GPTMTAMR |= 0x02; //set timer to periodic mode
	*GPTMTAILR = CYCLESPERSECOND/sampleRate; //set reload value
	
	*GPTMCTL |= 0x21; //enable timer and adc trigger
	
	
}

//Returns the current sample rate
int getSampleRate(void)
{
	return sampleRate;
}

//Increments the sample rate by 10
void increaseSampleRate(void)
{
	if (!(sampleRate == MAXSAMPLERATE))
	{
		sampleRate += 10;
		_setTimerReload(CYCLESPERSECOND/sampleRate);
	}
}

//Decrements the sample rate by 10
void decreaseSampleRate(void)
{
	if (!(sampleRate == MINSAMPLERATE))
	{
		sampleRate -= 10;
		_setTimerReload(CYCLESPERSECOND/sampleRate);
	}
}

/*
 * Changes the reload value of the timer
*/
void _setTimerReload(int reload)
{
	*GPTMTAILR = reload;
	
}

