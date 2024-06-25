// queue.h  

#include <stdio.h>  
#include <stdlib.h>  
#include <stdbool.h>  

#ifndef QUEUE_H  
#define QUEUE_H  
  
#define SIZE 30
#define QUEUE_SIZE 12 
  
// ???????
typedef struct {
    int data[QUEUE_SIZE];
    int front;
    int rear;
} Queue;  
  
void initQueue(Queue *q);
void enqueue(Queue *q, int x);
int dequeue(Queue *q);
void printQueue(Queue *q);
int findMode(int *a, int size, int *modeCount);
int findMedian(int *a, int size);
void findGreaterNumbers(Queue *q, int *a, int size, int num);
void findSmallerNumbers(Queue *q, int *a, int size, int num);
int average(Queue *q);


  
#endif // QUEUE_H

