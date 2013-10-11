
extern void setupADC(void);
extern void setupUART(void);
extern void setupTimer(void);
extern void printString(char str[]);


int main(void)
{
	setupTimer();
	setupADC();
	setupUART();
	
	while (1) {;}

}

void adcISR(void)
{
	
	printString("test");
}