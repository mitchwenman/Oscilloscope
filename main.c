extern void setupADC(void);
extern void setupUART(void);
extern void setupTimer(void);
extern void drawLoop(void);


int main(void)
{
	setupUART();
	setupTimer();
	setupADC();
	
	drawLoop();
					
}



