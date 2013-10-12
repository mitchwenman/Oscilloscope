extern void setupADC(void);
extern void setupUART(void);
extern void setupTimer(void);



int main(void)
{
	setupTimer();
	setupADC();
	setupUART();
	
	while (1) {;}
					
}



