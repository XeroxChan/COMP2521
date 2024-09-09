
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Queue.h"

#define LARGE_NUMBER 100000

int main(void) {
	Queue q = QueueNew();

	// TODO
	// Write a benchmark test to demonstrate the difference between
	// ArrayQueue and CircularArrayQueue
    
	for (int i = 1; i <= LARGE_NUMBER; i++) {
		QueueEnqueue(q, i);
		
	}


	for (int j = 1; j <= LARGE_NUMBER; j++) {
		
		QueueDequeue(q);
	}
	
	QueueFree(q);
}

