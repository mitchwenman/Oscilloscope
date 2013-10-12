extern void setupADC(void);
extern void setupUART(void);
extern void setupTimer(void);



int main(void)
{
	setupUART();
	setupTimer();
	setupADC();
	
	while (1) {;}
					
}



