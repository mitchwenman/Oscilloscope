#include <stdlib.h>

					/****** Method Prototypes ******/
void enqueue(int elem);
int dequeue(void);
int isFull(void);
int isEmpty(void);

					/****** Constants ******/
const int SIZE = 64;


					/****** Static variables ******/
					
int elements[SIZE];
unsigned int head = 0;
unsigned int tail = 0;

					/****** Public Methods ******/
void enqueue(int elem)
{
	if (!isFull())
	{
		elements[tail % SIZE] = elem;
		tail++;
	}
}

int dequeue(void)
{
	if (isEmpty())
		return NULL;
	else
		return elements[head++ % SIZE]; //head will post increment after return
}

int isFull(void)
{
	if (tail == head + SIZE) 
		return 1;
	else 
		return 0;
	
}

int isEmpty(void)
{
	if (head == tail) return 1;
	else return 0;
}


					/****** Private Methods ******/

