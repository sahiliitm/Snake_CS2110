#include<stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#define MAXDURATION 10
#define WIDTH 79
#define HEIGHT 23

typedef struct coord_ 
{
   	int x;
	int y;
}coord;

typedef struct bomb_
{
	int isactive;
	int duration;
	coord topleft;
}bomb;

typedef struct Node_ Node;
struct Node_ {
     coord data; 
    Node* next;
};

typedef struct Queue_ {
    Node* head; //head of snake
	Node* tail; //tail of snake
} Queue;


coord make_coord(int x,int y)
{
	coord c;
	c.x=x;
	c.y=y;
	return c;
}
// Create a new node with data and next element (can be NULL)
Node* Node_new( coord data, Node* next ){
	Node* new_node=(Node*)malloc(sizeof(Node));
	new_node->data=data;
	new_node->next=next;
	
	return new_node;
}

// Create a new empty queue
Queue* queue_new(){
	Queue* new_queue=(Queue*)malloc(sizeof(Queue));
	new_queue->head=NULL;
	new_queue->tail=NULL;
	
	return new_queue;
}	

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
	free(q);
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

coord mk_reward(Queue *s1, Queue *s2)
{
//	Snake *reward;
	int x = rand()%WIDTH;
	int y = rand()%HEIGHT;
	coord data;
	data.x = x;
	data.y = y;
	return data;
}
explode(Queue* s1,Queue* s2)
{}
void printscreen(Queue *s1, Queue *s2, coord rew,bomb b)
{
	clear();
	Node *q = s1->tail;
	while(q!=NULL)
	{		
		mvprintw(q->data.y, q->data.x, "0");
		q = q->next;	
	}		
	q = s2->tail;
	while(q!=NULL)
	{
		mvprintw(q->data.y, q->data.x, "#");
		q = q->next;	
	}
	mvprintw(rew.y, rew.x, "R");
	if( b.isactive  )
	switch(b.duration%4)
	{
		case 0:
				mvprintw(b.topleft.y,b.topleft.x,"_");
				mvprintw(b.topleft.y+1,b.topleft.x+1,"|");
				break;
		case 1: mvprintw(b.topleft.y+1,b.topleft.x+1,"|");
				mvprintw(b.topleft.y+1,b.topleft.x,"_");
				break;
		case 2: mvprintw(b.topleft.y+1,b.topleft.x,"_");
				mvprintw(b.topleft.y+1,b.topleft.x-1,"|");
				break;
		case 3:mvprintw(b.topleft.y+1,b.topleft.x-1,"|");
				mvprintw(b.topleft.y,b.topleft.x,"_");
				break;
	
	}
}

int collide (Queue *s1, Queue *s2, char dir1, char dir2)
{
	int x, y;
	coord head_s2 = get_head(s2);
	coord head_s1 = get_head(s1);

		if ( ( head_s1.x == head_s2.x ) && ( head_s1.y == head_s2.y ) )
			return 2;									// Both get penalised/Both made a mistake
		else if ((dir1=='l')&&(dir2=='r'))//snake 1 moves left, snake 2 moves right
		{
			if ((head_s1.y==head_s2.y)&&(head_s1.x==head_s2.x-1))
				return 2;
		}
		else if ((dir2=='l')&&(dir1=='r'))//snake 1 moves right, snake 2 moves left					
		{
			if ((head_s1.y==head_s2.y)&&(head_s1.x==head_s2.x+1))
				return 2;
		}
		else if	((dir1=='u')&&(dir2=='d'))//snake 1 moves up, snake 2 moves down	
		{
			if ((head_s1.x==head_s2.x)&&(head_s1.y==head_s2.y+1))
				return 2;
		}
		else if ((dir1=='d')&&(dir2=='u'))//snake 1 moves down, snake 2 moves up
		{
			if ((head_s1.x==head_s2.x)&&(head_s1.y==head_s2.y-1))
				return 2;
		}
	if ((queue_size(s1)==1)&&(queue_size(s2)==1))
		return 0;	
	Node *temp = s1->tail;
	while (temp->next!=NULL)
	{ 
		
		if ( (temp->data.x == head_s1.x) && (temp->data.y == head_s1.y) )
			return 1;
		else if ( (temp->data.y == head_s2.y) && (temp->data.x == head_s2.x))
			return -1;
		temp = temp->next;	
	}
	temp = s2->tail;

	while (temp->next!=NULL)
	{
		
		if ((temp->data.x==head_s1.x) && (temp->data.y==head_s1.y))
			return 1;
		else if ((temp->data.y==head_s2.y)&&(temp->data.x==head_s2.x))
			return -1;
		temp = temp->next;	
	}
	return 0;
}	


void main()
{
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	//raw();	
	refresh();
	long long int runno=0;	
	struct timeval tim;  
	timeout(100);
	int snake1y = 0 , snake2y = 0;	// The number of rows the snake head is below the top left
	int snake1x = 0 , snake2x = WIDTH;			
	Queue *s1 = queue_new();
	s1 = queue_push(s1, make_coord(snake1x, snake1y));
	Queue *s2 = queue_new();
	s2 = queue_push(s2, make_coord(snake2x, snake2y));
	keypad(stdscr, TRUE);
	refresh();
	int ch;
	char snake1dir,snake2dir;
	snake1dir='r';
	snake2dir='l';
	int set_dir1=0 , set_dir2=0;
	srand(time(NULL));
	coord reward = mk_reward(s1, s2); 
	bomb b={1,0,{10,10}};
	int bombx,bomby;	
	printscreen(s1, s2, reward,b);
	
	/*int col = collide(s1, s2);
	printw("%d", col);
	getch();*/
	int nextexplode=( rand() % 20 ) * 10 +runno+10;
		
	double t2=1000,t1=0;
	do 
	{
		set_dir1=0;
		set_dir2=0;
		gettimeofday(&tim, NULL);
		runno++;
		if(b.isactive)
		{
			if(b.duration == MAXDURATION )
			{
				explode(s1,s2);
				nextexplode = ( rand() % 20 ) * 10 +runno;
			}			
			else 
			b.duration ++;
		}		
		else if ( runno == nextexplode )
		{
			b.isactive=  1 ;
			b.duration= 0 ;
			b.topleft.x = (rand()%WIDTH) ;
			b.topleft.y= (rand()%HEIGHT)  ;
		}
			
			        	
		t1 = tim.tv_sec*1000 + ( tim.tv_usec / 1000.0 );  
		timeout(100); 		
				
		ch=getch();						
		switch (ch)
		{
				
						
				case (int)'w': if ((snake1dir=='r')||(snake1dir=='l'))
							{
								if (snake1y==0)
									snake1y = HEIGHT;
								else
									snake1y--;
								snake1dir = 'u';
								set_dir1=1;
							}
							break;
				
				case (int)'s':  if ((snake1dir=='r')||(snake1dir=='l'))
							{
								if (snake1y==HEIGHT)
									snake1y = 0;
								else
									snake1y++;
								set_dir1=1;
								snake1dir = 'd';
							}
							break;
								
				case (int)'a': if ((snake1dir=='u')||(snake1dir=='d'))
							{
								if (snake1x==0)
									snake1x = WIDTH;
								else
									snake1x--;
								set_dir1=1;
								snake1dir = 'l';
							}
							break;
				
				case (int)'d': if ((snake1dir=='u')||(snake1dir=='d'))
							{
								if (snake1x==WIDTH)
									snake1x = 0;
								else
									snake1x++;
								set_dir1=1;
								snake1dir = 'r';
							}
							break;
				case 259: if ((snake2dir=='r')||(snake2dir=='l'))
							{
								if (snake2y==0)
									snake2y = HEIGHT;
								else
									snake2y--;
								snake2dir = 'u';
								set_dir2=1;
							}
							break;
				case 258 : if ((snake2dir=='r')||(snake2dir=='l'))
							{
								if (snake2y==HEIGHT)
									snake2y = 0;
								else
									snake2y++;
								set_dir2=1;
								snake2dir = 'd';
							}
							break;
				case 260 : if ((snake2dir=='u')||(snake2dir=='d'))
							{
								if (snake2x==0)
									snake2x = WIDTH;
								else
									snake2x--;
								set_dir2=1;
								snake2dir = 'l';
							}
							break;		
							
				case 261: if ((snake2dir=='u')||(snake2dir=='d'))
							{
								if (snake2x==WIDTH)
									snake2x = 0;
								else
									snake2x++;
								set_dir2=1;
								snake2dir = 'r';
							}
							break;
			}

		if (set_dir1==0)
		{
			switch(snake1dir)
			{
				case 'l': if (snake1x==0)
							snake1x = WIDTH;
						else
							snake1x--;
						break;
				case 'r': if (snake1x==WIDTH)
							snake1x = 0;
						else
							snake1x++;
						break;
				case 'u': if (snake1y==0)
							snake1y = HEIGHT;
						else
							snake1y--;
						break;
				case 'd': if (snake1y==HEIGHT)
							snake1y = 0;
						else
							snake1y++;
						break;
			}
		}
		if (set_dir2==0)
		{
			switch(snake2dir)
			{
				case 'l': if (snake2x==0)
							snake2x = WIDTH;
						else
							snake2x--;
						break;
				case 'r': if (snake2x==WIDTH)
							snake2x = 0;
						else
							snake2x++;
						break;
				case 'u': if (snake2y==0)
							snake2y = HEIGHT;
						else
							snake2y--;
						break;
				case 'd': if (snake2y==HEIGHT)
							snake2y = 0;
						else
							snake2y++;
						break;
			}
		}
		
		s1 = queue_push(s1, make_coord(snake1x, snake1y));
		s2 = queue_push(s2, make_coord(snake2x, snake2y));
		if ((reward.x!=snake1x)||(reward.y!=snake1y))
			s1 = queue_pop(s1);
		else 
			reward = mk_reward(s1, s2);
		
		if ((reward.x!=snake2x)||(reward.y!=snake2y))
			s2 = queue_pop(s2);
		else 
			reward = mk_reward(s1, s2);
		int col = collide(s1, s2, snake1dir, snake2dir);
	//	int col=0;
		if (col==2)
		{
			clear();
			printscreen(s1, s2, reward,b);	
			mvprintw(HEIGHT/2, WIDTH/2, "Draw");

			timeout(5000);
			getch();
			refresh();
			break;
		}
		else if (col==1)
		{
			clear();
			printscreen(s1, s2, reward,b);	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 2 wins!");

			timeout(5000);
			getch();
			refresh();
			break;
		}
		else if (col==-1)
		{
			clear();
			printscreen(s1, s2, reward,b);	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 1 wins!");

			timeout(5000);
			getch();
			refresh();
			break;
		}
		
		gettimeofday(&tim, NULL);  
    		double t2 = tim.tv_sec*1000.0 + ( tim.tv_usec/1000.0  );		
		
		while(   t2-t1 < 100.1 )
		{
			timeout(0);
			getch();
			getch();
			getch();
			gettimeofday(&tim, NULL);  
    			t2 = tim.tv_sec*1000.0 + ( tim.tv_usec/1000.0  );		
					
		}		
			
		
		printscreen(s1, s2, reward,b);	
		refresh();
	}	
	while (ch!='q');
	clrtoeol();
	refresh();
	endwin();
} 
   			
	
	          
