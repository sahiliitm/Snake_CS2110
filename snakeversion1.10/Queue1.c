#include "Queue.h"
#include <ncurses.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/time.h>

// Create a new node with data and next element (can be NULL)
Node* Node_new( coord data, Node* next ){
	Node* new_node=(Node*)malloc(sizeof(Node));
	new_node->data=data;
	new_node->next=next;
	
	return new_node;
}

Queue* queue_new(){
	Queue* new_queue=(Queue*)malloc(sizeof(Queue));
	new_queue->head=NULL;
	new_queue->tail=NULL;
	
	return new_queue;
}	


//Queue UTility functions

//Inserts val to the head of the queue
Queue* queue_push( Queue* q, coord val ){
	Node* new_node=Node_new(val,NULL);
	
	if(q->head!=NULL)
		(q->head)->next=new_node;
	
	q->head=new_node;
	
	if(q->tail==NULL){
		q->tail=q->head;	
	}
	return q;
}

// Remove the element at the tail of the snake - also frees memory
Queue* queue_pop( Queue* q ){
	Node* ptr;
	if((q->tail)!=NULL){
		if((q->tail)->next!=NULL){
			ptr=q->tail;
			q->tail=(q->tail)->next;
			free(ptr);
			return q;
		}
		else{
			ptr=q->tail;
			free(ptr);
			q->head=NULL;
			q->tail=NULL;
			return q;
		}
	}
}

// Deletes the queue, frees memory.
void queue_delete( Queue* q ){
	while((q->tail)!=NULL){
		q=queue_pop(q);
	}
//	free(q);
}

// Returns the element currently at the tail of the snake or the top of the queue.
coord snake_tail( Queue* q, int* error ){
	coord answer;	
	if(q->tail!=NULL){
		answer=(q->tail)->data;
		if(error!=NULL)
			*error=0;
		return answer;
	}
	else{
		if(error!=NULL)
			*error=1;
		return answer;
	}	
}

// Returns the number of elements in the queue
int queue_size( Queue* q ){
	int size=0;
	Node* next_node=q->tail;
	while(next_node!=NULL){
		size++;
		next_node=next_node->next;
	}
	
	return size;
}

// Prints the elements currently in the queue
void queue_print( Queue* q ){
	Node* next_node=q->tail;
	while(next_node!=NULL){
		printw("(%d,%d) ",next_node->data.y,next_node->data.x);
		next_node=next_node->next;
	}
	printw("\n");
	refresh();
}

coord get_head(Queue *q)
{
	coord ans;	
	if(q->head!=NULL)	
	return (q->head)->data;
	else return ans;
}




