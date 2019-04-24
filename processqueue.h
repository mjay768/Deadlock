

// A user-defined library file for general queue operations.
#ifndef PROCESSQUEUE_H
#define PROCESSQUEUE_H

#include <stdio.h>
#include <stdlib.h>
int item;
struct Node {
	int pid;
	int rid;
	int num;
	struct Node *next;
};
 
struct Queue {
	struct Node *front;
	struct Node *last;
	unsigned int size;
};
 
void init(struct Queue *q) {
	q->front = NULL;
	q->last = NULL;
	q->size = 0;
}
 

int pop(struct Queue *q) {
	if(q->size == 0)
    {
        return -1;
    }
	item = q->front->data;
	q->size--;
    
	struct Node *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
	return item;
}
 
void push(struct Queue *q, int pid, int rid, int num) {
	q->size++;
 
	if (q->front == NULL) {
		q->front = (struct Node *) malloc(sizeof(struct Node));
		q->front->pid = pid;
		q->front->rid = rid;
		q->front->num = num;
		q->front->next = NULL;
		q->last = q->front;
	} else {
		q->last->next = (struct Node *) malloc(sizeof(struct Node));
		q->last->next->pid = pid;
		q->last->next->rid = rid;
		q->last->next->num = num;
		q->last->next->next = NULL;
		q->last = q->last->next;
	}
}
 
#endif
