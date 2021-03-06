#include <ncurses.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#ifndef QUEUE_
#define QUEUE_
typedef struct coord_ 
{
   	int x;
	int y;
}coord;

typedef struct Node_ {
    coord data; 
    struct Node_ * next;
}Node;

typedef struct Queue_ {
    Node* head; 									//head of snake
	Node* tail; 									//tail of snake
} Queue;


Node* Node_new( coord data, Node* next );

Queue* queue_new();

Queue* queue_push( Queue* q, coord val );

Queue* queue_pop( Queue* q );

void queue_delete( Queue* q );

coord snake_tail( Queue* q, int* error );

int queue_size( Queue* q );

int queue_search(Queue * q , coord r );

void queue_print( Queue* q );

coord get_head(Queue *q);

#endif

