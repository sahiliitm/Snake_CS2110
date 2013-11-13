#include "Queue.h"
#include <ncurses.h>
#include<stdlib.h>
#include<stdio.h>
#include <sys/time.h>


/* Purpose  :   Create a new node 
   Inputs   :    data        -       Data of this node
            :    *next       -       Next node
   Output   :    *new_node   -       Node created
 */    
 
Node* Node_new( coord data, Node* next ){
	Node* new_node=(Node*)malloc(sizeof(Node));
	new_node->data=data;
	new_node->next=next;
	
	return new_node;
}


/* Purpose  :   Create a new queue 
   Output   :    *new_queue   -       Queue created
 */    
 

Queue* queue_new(){
	Queue* new_queue=(Queue*)malloc(sizeof(Queue));
	new_queue->head=NULL;
	new_queue->tail=NULL;
	
	return new_queue;
}	


/* Purpose  :   Push a node into the queue and make it head  
   Inputs   :    *q       -       Queue into which node is to be pushed
            :    val      -       Node to be pushed
   Output   :    *q       -       Updated Queue
 */    
 
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


/* Purpose  :   Remove the element at the tail of the snake - also frees memory
   Inputs   :    *q       -       Queue into which node is to be pushed
   Output   :    *q       -       Updated Queue
 */ 

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

/* Purpose  :    Deletes the queue, frees memory.
   Inputs   :    *q       -       Queue 
 */ 

void queue_delete( Queue* q ){
	while((q->tail)!=NULL){
		q=queue_pop(q);
	}
//	free(q);
}


/* Purpose  :   Returns the element currently at the tail of the snake or the top of the queue.
   Inputs   :    *q       -       Queue 
            :    *error   -       For reporting error
   Output   :    answer   -       Tail of snake
 */ 

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

/* Purpose  :   Returns the number of elements in the queue
   Inputs   :    *q       -       Queue 
   Output   :    size     -       Size of snake
 */ 

int queue_size( Queue* q ){
	int size=0;
	Node* next_node=q->tail;
	while(next_node!=NULL){
		size++;
		next_node=next_node->next;
	}
	
	return size;
}

/* Purpose  :   Prints the elements currently in the queue
   Inputs   :    *q       -       Queue 
 */ 

void queue_print( Queue* q ){
	Node* next_node=q->tail;
	while(next_node!=NULL){
		printw("(%d,%d) ",next_node->data.y,next_node->data.x);
		next_node=next_node->next;
	}
	printw("\n");
	refresh();
}


/* Purpose  :   Outputs the head of the snake
   Inputs   :    *q       -       Queue 
   output   :    ans      -       Head of snake
 */ 

coord get_head(Queue *q)
{
	coord ans;	
	if(q->head!=NULL)	
	return (q->head)->data;
	else return ans;
}

/* Purpose  :   Searches for a particular coord in the Queue
   Inputs   :    *q       -       Queue 
            :     r       -       coord to be searched
   output   :     1       -       If it is found
                  0       -       Otherwise
 */ 

int queue_search(Queue * q , coord r )
{
	Node* temp = q->tail;
	while( temp != NULL)
	{
	    if( temp->data.x == r.x && temp->data.y == r.y )
	    return 1;
        temp=temp->next;
	}
	return 0;
}
