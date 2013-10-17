#include <stdio.h>
#include <stdlib.h>

FILE *fp;



void writeValue(int value);
void initLog(void);
void close(void);

void writeValue(int value)
{
	fprintf(fp, "%i\n", value);
}


void initLog(void)
{
	fp = fopen("Log.txt", "w");
}

void close(void)
{
	fclose(fp);
}