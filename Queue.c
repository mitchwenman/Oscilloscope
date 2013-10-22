#include <stdlib.h>

					/****** Method Prototypes ******/
void enqueue(int elem);
int dequeue(void);
int isFull(void);
int isEmpty(void);

					/****** Constants ******/
const int SIZE = 64; //Size of the array 
										//Power of 2 so an overflow won't break it


					/****** Static variables ******/
					
int elements[SIZE]; //The array of data

//The head and tail pointers
unsigned int head = 0; //unsigned so overflows to 0
unsigned int tail = 0;

					/****** Public Methods ******/
//Adds the element to the end of the queue
void enqueue(int elem)
{
	if (!isFull())
	{
		elements[tail % SIZE] = elem;
		tail++;
	}
}

//Returns the element at the head of the queue
int dequeue(void)
{
	if (isEmpty())
		return NULL;
	else
		return elements[head++ % SIZE]; //head will post increment after return
}

//Returns 1 if the queue is full
//Returns 0 if it isn't
int isFull(void)
{
	if (tail == head + SIZE) 
		return 1;
	else 
		return 0;
	
}

//Returns 1 if the queue is empty
//Returns 0 if is isn't
int isEmpty(void)
{
	if (head == tail) return 1;
	else return 0;
}


