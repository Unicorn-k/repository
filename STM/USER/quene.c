#include <stdio.h>  
#include <stdlib.h>  
#include <stdbool.h>  
#include <quene.h>
  


  
  
// ?????
void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

// ????
void enqueue(Queue *q, int x) {
    if ((q->rear + 1) % QUEUE_SIZE == q->front) {
        //printf("????\n");
        return;
    }
    q->data[q->rear] = x;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
}
// ????
int dequeue(Queue *q) {
	  int x=0;
    if (q->front == q->rear) {
        //printf("????\n");
        return -1;
    }
    x = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    return x;
}

// ????
void printQueue(Queue *q) {
    int i = q->front;
    while (i != q->rear) {
        //printf("%d ", q->data[i]);
        i = (i + 1) % QUEUE_SIZE;
    }
    //printf("\n");
}
int average(Queue *q) {
    int sum = 0;
    int count = 0;
    int i = q->front;
    while (i != q->rear) {
        sum += q->data[i];
        count++;
        i = (i + 1) % QUEUE_SIZE;
    }
    return count > 0 ? sum / count : 0;
}

// ????????
int findMode(int *a, int size, int *modeCount) {
    int count = 0, mode = 0,i=0,j=0;
    for (i = 0; i < size; i++) {
        int tempCount = 0;
        for (j = 0; j < size; j++) {
            if (a[j] == a[i]) tempCount++;
        }
        if (tempCount > count) {
            count = tempCount;
            mode = a[i];
        }
    }
    *modeCount = count;
    return mode;
}

int findMedian(int *a, int size) {
    int i=0,j=0,t=0,median = 0;
    int* temp = (int*)malloc(size * sizeof(int));
    for (i = 0; i < size; i++) {
        temp[i] = a[i];
    }
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (temp[j] > temp[j + 1]) {
                t = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = t;
            }
        }
    }
    if (size % 2 == 0)
        median = (temp[size / 2 - 1] + temp[size / 2]) / 2;
    else
        median = temp[size / 2];

    free(temp);
    return median;
}

// ??????????????5??
void findGreaterNumbers(Queue *q, int *a, int size, int num) {
    int temp[5] = {0};
    int diff[5] = {0};
		int i=0,j=0,k=0,d=0;
    for (i = 0; i < size; i++) {
        if (a[i] > num) {
            d = a[i] - num;
            for (j = 0; j < 5; j++) {
                if (diff[j] == 0 || diff[j] > d) {
                    for (k = 4; k > j; k--) {
                        temp[k] = temp[k - 1];
                        diff[k] = diff[k - 1];
                    }
                    temp[j] = a[i];
                    diff[j] = d;
                    break;
                }
            }
        }
    }
    for (i = 0; i < 5; i++) {
        enqueue(q, temp[i]);
    }
}

// ??????????????5??
void findSmallerNumbers(Queue *q, int *a, int size, int num) {
    int temp[5] = {0};
    int diff[5] = {0};
		int i=0,j=0,d=0,k=0;
    for (i = 0; i < size; i++) {
        if (a[i] < num) {
            d = num - a[i];
            for (j = 0; j < 5; j++) {
                if (diff[j] == 0 || diff[j] > d) {
                    for (k = 4; k > j; k--) {
                        temp[k] = temp[k - 1];
                        diff[k] = diff[k - 1];
                    }
                    temp[j] = a[i];
                    diff[j] = d;
                    break;
                }
            }
        }
    }
    for (i = 0; i < 5; i++) {
        enqueue(q, temp[i]);
    }
}

