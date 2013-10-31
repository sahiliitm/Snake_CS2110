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
#define DBUG(x)			mvprintw(20, 10, "%d%d%d",x,x,x);refresh();sleep(5);


//Structures to be used

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

typedef struct Node_ {
     coord data; 
    struct Node_ * next;
}Node;

typedef struct Queue_ {
    Node* head; 									//head of snake
	Node* tail; 									//tail of snake
} Queue;

typedef struct bombregion_
{
	coord array[23];
}bombregion;

//CREATING/INITIALIZING NEW STRUCTURES

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

void init_bombregion(bombregion * br,bomb b)
{
	int i,k=0;
	for(i=-1;i <= 1;i++)
	{
			br->array[k].x = (b.topleft.x +i)%WIDTH;
			br->array[k++].y=(b.topleft.y-2)%HEIGHT;
			br->array[k].x=(b.topleft.x+i)%WIDTH;
			br->array[k++].y=(b.topleft.y+2)%HEIGHT;
	}
	for(i=-2;i <= 2;i++)
	{
			br->array[k].x = (b.topleft.x +i)%WIDTH;
			br->array[k++].y=(b.topleft.y-1)%HEIGHT;
			br->array[k].x=(b.topleft.x+i)%WIDTH;
			br->array[k++].y=(b.topleft.y+1)%HEIGHT;
	}	
	for(i=-3;i <= 3;i++)
	{
			br->array[k].x = (b.topleft.x +i)%WIDTH;
			br->array[k++].y=(b.topleft.y)%HEIGHT;
	}

}

// Create a new empty queue
Queue* queue_new(){
	Queue* new_queue=(Queue*)malloc(sizeof(Queue));
	new_queue->head=NULL;
	new_queue->tail=NULL;
	
	return new_queue;
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

int coord_in_bomb(coord c, bombregion br)
{
	int i;
	for(i=0;i<23;i++)
	if(br.array[i].x==c.x && br.array[i].y==c.y)
	return 1;
	return 0;
}


void printscreen(Queue *s1, Queue *s2, coord *rew,bomb *b)
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
	if (rew!=NULL)
		mvprintw(rew->y, rew->x, "R");
	
	if((b!=NULL)&& (b->isactive ) )
	switch(b->duration%4)
	{
		case 0:
				mvprintw(b->topleft.y,b->topleft.x,"_");
				mvprintw(b->topleft.y+1,b->topleft.x+1,"|");
				break;
		case 1: mvprintw(b->topleft.y+1,b->topleft.x+1,"|");
				mvprintw(b->topleft.y+1,b->topleft.x,"_");
				break;
		case 2: mvprintw(b->topleft.y+1,b->topleft.x,"_");
				mvprintw(b->topleft.y+1,b->topleft.x-1,"|");
				break;
		case 3:mvprintw(b->topleft.y+1,b->topleft.x-1,"|");
				mvprintw(b->topleft.y,b->topleft.x,"_");
				break;
	
	}
	mvprintw(HEIGHT, WIDTH,"");
	refresh();
}


void end_game(int col ,Queue *s1 , Queue *s2)
{
		if (col==0)
			return;
		
		if (col==2)
		{
			clear();
			if(s1!=NULL && s2!=NULL)
				printscreen(s1, s2, NULL, NULL);	
			mvprintw(HEIGHT/2, WIDTH/2, "Draw");

		}
		else if (col==1)
		{
			clear();
			if(s1!=NULL && s2!=NULL)
			printscreen(s1, s2, NULL, NULL);	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 2 wins!");

		}
		else if (col==-1)
		{
			clear();
			if(s1!=NULL && s2!=NULL)
			printscreen(s1, s2, NULL, NULL);	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 1 wins!");

		}
		timeout(5000);
		getch();		
		clrtoeol();
		refresh();
		endwin();
		exit(0);
}


int modify_snake(Queue* s1,bomb *b,bombregion br)
{
	Node *n = s1->tail;
	Node *last_in_bomb = NULL;
	if (!coord_in_bomb(s1->head->data, br))
	{	
		while (n!=NULL)
		{
			if (coord_in_bomb(n->data, br))
			{
						last_in_bomb = n;	
			}
			n=n->next;
		}
		if (last_in_bomb!=NULL)
		{
			Node *last= s1->tail;
			s1->tail = last_in_bomb->next;
			while(last!=last_in_bomb->next)
			{
				Node *temp = last;
				free(temp);
				last=last->next;	
			}	
		}
	}
	else 
	{
		//DBUG(2);
		Node * last_not_in_bomb = NULL;
		Node * last_in = NULL;
		n = s1->tail;
			
		while (n->next!=NULL)
		{
			if ((coord_in_bomb(n->data, br))&&(!(coord_in_bomb(n->next->data, br))))
			{
						last_in = n;	
			}
			else if ((!(coord_in_bomb(n->data, br)))&&(coord_in_bomb(n->next->data, br)))
			{
					last_not_in_bomb = n;
			}
			n=n->next;
		}	
		if ((last_not_in_bomb==NULL)&&(last_in==NULL))
		{
			Node *last= s1->tail;
			s1->tail = NULL;
			while(last!=NULL)
			{
				Node *temp = last;
				free(temp);
				last=last->next;	
			}	
			
			return 1;
		}
		else if (last_in==NULL)
		{
		//	DBUG(4)
			Queue *temp_q = queue_new();
			temp_q->tail = last_not_in_bomb->next;
			temp_q->head = s1->head;
			s1->head = last_not_in_bomb;
			last_not_in_bomb->next = NULL;
			queue_delete(temp_q);
		}
		else
		{
		//	DBUG(5)
			Queue *temp_q = queue_new();
			temp_q->tail = s1->tail;
			temp_q->head = last_in;
			s1->tail = last_in->next;
			last_in->next = NULL;
			queue_delete(temp_q);

			Node *last= s1->tail;
			s1->tail = last_in_bomb->next;
			while(last!=last_in_bomb->next)
			{
				Node *temp = last;
				free(temp);
				last=last->next;	
			}	
		
		}
	}
	return 0;
}

int explode(Queue *s1, Queue *s2, bomb *b, int *call_no,bombregion br)
{
//modify_snake(s1,b);

	int i, j;
	if (*call_no>=0 && *call_no <=2)
	{
		mvprintw(b->topleft.y, b->topleft.x, ".");
		refresh();
	}
	else if (*call_no>=2&& *call_no <=4)
	{
		for (i=(b->topleft.x-2)%WIDTH; i<=(b->topleft.x+2)%WIDTH; i++)
		{
			mvprintw((b->topleft.y-1)%HEIGHT, i, ".");
			mvprintw((b->topleft.y+1)%HEIGHT, i, ".");
		}
		mvprintw((b->topleft.y)%HEIGHT,(b->topleft.x - 2)%WIDTH, ".");
		mvprintw((b->topleft.y)%HEIGHT,(b->topleft.x + 2)%WIDTH, ".");
		refresh();
	}	
	else if(*call_no>=4 && *call_no<=6){
		for (i=(b->topleft.x-2)%WIDTH; i<=(b->topleft.x+2)%WIDTH; i++)
		{
			mvprintw((b->topleft.y-1)%HEIGHT, i, ".");
			mvprintw((b->topleft.y+1)%HEIGHT, i, ".");
		}
		mvprintw((b->topleft.y)%HEIGHT,(b->topleft.x - 2)%WIDTH, ".");
		mvprintw((b->topleft.y)%HEIGHT,(b->topleft.x + 2)%WIDTH, ".");
		
		for(i=(b->topleft.x-1)%WIDTH; i<=(b->topleft.x+1)%WIDTH; i++){
			mvprintw((b->topleft.y-2)%HEIGHT, i, ".");
			mvprintw((b->topleft.y+2)%HEIGHT, i, ".");
		}
		refresh();
	}	
	
	else{
		*call_no=0;
		b->isactive=0;
		b->duration=0;
		int m1=modify_snake(s1, b,br);
		int m2=modify_snake(s2, b,br);
		if(m1==1 && m2==1 )
			end_game(2,s1,s2);
		
		else if(m1==0 && m2==1)
			end_game(-1,s1,s2);
		
		else if(m1==1 && m2==0)
			end_game(1,s1,s2);
		
		else if(m1==0 && m2==0)
			return 0;
	}
	(*call_no)++;
	return 0;			
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

int auto_collide (Queue *s2, Queue *s1, coord auto_head)
{
	int x, y;
//	coord head_s2 = get_head(s2);
//	if ( ( auto_head.x == head_s2.x ) && ( auto_head.y == head_s2.y ) )
//			return 1;									// Both get penalised/Both made a mistake
	Node * temp = s2->tail;
	while (temp!=NULL)
	{
		
		if ((temp->data.x==auto_head.x) && (temp->data.y==auto_head.y))
			return 1;
		temp = temp->next;	
	}
	temp = s1->tail;
	while (temp!=NULL)
	{
		
		if ((temp->data.x==auto_head.x) && (temp->data.y==auto_head.y))
			return 1;
		temp = temp->next;	
	}
	return 0;
}	
	
int auto_dist_to_reward(coord auto_head, coord reward)
{
	int distx=0, disty=0;
	distx+=(auto_head.x>reward.x)?(auto_head.x-reward.x):(reward.x-auto_head.x);
	if (distx>WIDTH/2)
		distx = WIDTH-distx;
	disty+=(auto_head.y>reward.y)?(auto_head.y-reward.y):(reward.y-auto_head.y);
	if (disty>HEIGHT/2)
		disty = HEIGHT-disty;
	return (distx+disty);
}

int auto_move(Queue *s1, Queue *s2, char *snake1dir, coord *reward, bomb *b)
{
	coord auto_head = get_head(s1);
	coord new_head = auto_head;
	int points[4]={-1, -1, -1, -1};
	switch(*snake1dir)
	{
		case 'u': points[2]=(HEIGHT+WIDTH+2)/2;
				break;
		case 'r': points[3]=(HEIGHT+WIDTH+2)/2;
				break;
		case 'd': points[0]=(HEIGHT+WIDTH+2)/2;
				break;
		case 'l': points[1]=(HEIGHT+WIDTH+2)/2;
				break;
	}
	int i;
	if (points[0]!=(HEIGHT+WIDTH+2)/2)
	{
		if (new_head.y==0)
			new_head.y = HEIGHT;
		else
			new_head.y--;
		if (auto_collide(s2,s1, new_head)==1)
			points[0]=(HEIGHT+WIDTH+2)/2;
		else
			points[0]=auto_dist_to_reward(new_head, *reward);
	}
	new_head=auto_head;					
	if (points[1]!=(HEIGHT+WIDTH+2)/2)
	{
		if (new_head.x==WIDTH)
			new_head.x = 0;
		else
			new_head.x++;
		if (auto_collide(s2,s1, new_head)==1)
			points[1]=(HEIGHT+WIDTH+2)/2;
		else
			points[1]=auto_dist_to_reward(new_head, *reward);
	}					
	new_head=auto_head;
	if (points[2]!=(HEIGHT+WIDTH+2)/2)
	{
		if (new_head.y==HEIGHT)
			new_head.y = 0;
		else
			new_head.y++;
		if (auto_collide(s2,s1, new_head)==1)
			points[2]=(HEIGHT+WIDTH+2)/2;
		else
			points[2]=auto_dist_to_reward(new_head, *reward);
	}					
	new_head=auto_head;
	if (points[3]!=(HEIGHT+WIDTH+2)/2)
	{
		if (new_head.x==0)
			new_head.x = WIDTH;
		else
			new_head.x--;
		if (auto_collide(s2, s1, new_head)==1)
			points[3]=(HEIGHT+WIDTH+2)/2;
		else
			points[3]=auto_dist_to_reward(new_head, *reward);
	}	
	int min = points[0];
	int pos=0;				
	for (i=1; i<4; i++)
	{
		if (min>points[i])
		{
			min=points[i];
			pos=i;
		}
	}
	new_head=auto_head;
	switch(pos)
	{
		case 0:	if (new_head.y==0)
					new_head.y = HEIGHT;
				else
					new_head.y--;
				*snake1dir='u';
				break;
		case 1: if (new_head.x==WIDTH)
					new_head.x = 0;
				else
					new_head.x++;
				*snake1dir='r';
				break;
		case 2: if (new_head.y==HEIGHT)
					new_head.y = 0;
				else
					new_head.y++;
				*snake1dir='d';
				break;
		case 3: if (new_head.x==0)
					new_head.x = WIDTH;
				else
					new_head.x--;
				*snake1dir='l';
				break;
	}
	s1 = queue_push(s1, new_head);
	if ((reward->x!=new_head.x)||(reward->y!=new_head.y))
		s1 = queue_pop(s1);
	else 
		*reward = mk_reward(s1, s2);
//	usleep(40000);
	return 0;		
	
}


int play_one(Queue *s1, Queue *s2, coord *snake2, char *snake2dir, coord *reward)
{
		int ch;
		int set_dir2=0;
		ch=getch();						
		switch (ch)
		{
				case (int) 'q': return 1;
				
				case 259: if ((*snake2dir=='r')||(*snake2dir=='l'))
							{
								if (snake2->y==0)
									snake2->y = HEIGHT;
								else
									snake2->y--;
								*snake2dir = 'u';
								set_dir2=1;
							}
							break;
				case 258 : if ((*snake2dir=='r')||(*snake2dir=='l'))
							{
								if (snake2->y==HEIGHT)
									snake2->y = 0;
								else
									snake2->y++;
								set_dir2=1;
								*snake2dir = 'd';
							}
							break;
				case 260 : if ((*snake2dir=='u')||(*snake2dir=='d'))
							{
								if (snake2->x==0)
									snake2->x = WIDTH;
								else
									snake2->x--;
								set_dir2=1;
								*snake2dir = 'l';
							}
							break;		
							
				case 261: if ((*snake2dir=='u')||(*snake2dir=='d'))
							{
								if (snake2->x==WIDTH)
									snake2->x = 0;
								else
									snake2->x++;
								set_dir2=1;
								*snake2dir = 'r';
							}
							break;
			}
		if (set_dir2==0)
		{
			switch(*snake2dir)
			{
				case 'l': if (snake2->x==0)
							snake2->x = WIDTH;
						else
							snake2->x--;
						break;
				case 'r': if (snake2->x==WIDTH)
							snake2->x = 0;
						else
							snake2->x++;
						break;
				case 'u': if (snake2->y==0)
							snake2->y = HEIGHT;
						else
							snake2->y--;
						break;
				case 'd': if (snake2->y==HEIGHT)
							snake2->y = 0;
						else
							snake2->y++;
						break;
			}
		}
		
		s2 = queue_push(s2, *snake2);
		if ((reward->x!=snake2->x)||(reward->y!=snake2->y))
			s2 = queue_pop(s2);
		else 
			*reward = mk_reward(s1, s2);

	return 0;
}

int play(Queue *s1, Queue *s2, coord* snake1, coord *snake2, char *snake1dir, char *snake2dir, coord *reward )
{
		int ch;
		int set_dir1=0;
		int set_dir2=0;
		ch=getch();						
		switch (ch)
		{
				case (int)'q': return 1;
				
				case (int)'w': if ((*snake1dir=='r')||(*snake1dir=='l'))
							{
								if (snake1->y==0)
									snake1->y = HEIGHT;
								else
									snake1->y--;
								*snake1dir = 'u';
								set_dir1=1;
							}
							break;
				
				case (int)'s':  if ((*snake1dir=='r')||(*snake1dir=='l'))
							{
								if (snake1->y==HEIGHT)
									snake1->y = 0;
								else
									snake1->y++;
								set_dir1=1;
								*snake1dir = 'd';
							}
							break;
								
				case (int)'a': if ((*snake1dir=='u')||(*snake1dir=='d'))
							{
								if (snake1->x==0)
									snake1->x = WIDTH;
								else
									snake1->x--;
								set_dir1=1;
								*snake1dir = 'l';
							}
							break;
				
				case (int)'d': if ((*snake1dir=='u')||(*snake1dir=='d'))
							{
								if (snake1->x==WIDTH)
									snake1->x = 0;
								else
									snake1->x++;
								set_dir1=1;
								*snake1dir = 'r';
							}
							break;
				case 259: if ((*snake2dir=='r')||(*snake2dir=='l'))
							{
								if (snake2->y==0)
									snake2->y = HEIGHT;
								else
									snake2->y--;
								*snake2dir = 'u';
								set_dir2=1;
							}
							break;
				case 258 : if ((*snake2dir=='r')||(*snake2dir=='l'))
							{
								if (snake2->y==HEIGHT)
									snake2->y = 0;
								else
									snake2->y++;
								set_dir2=1;
								*snake2dir = 'd';
							}
							break;
				case 260 : if ((*snake2dir=='u')||(*snake2dir=='d'))
							{
								if (snake2->x==0)
									snake2->x = WIDTH;
								else
									snake2->x--;
								set_dir2=1;
								*snake2dir = 'l';
							}
							break;		
							
				case 261: if ((*snake2dir=='u')||(*snake2dir=='d'))
							{
								if (snake2->x==WIDTH)
									snake2->x = 0;
								else
									snake2->x++;
								set_dir2=1;
								*snake2dir = 'r';
							}
							break;
			}

		if (set_dir1==0)
		{
			switch(*snake1dir)
			{
				case 'l': if (snake1->x==0)
							snake1->x = WIDTH;
						else
							snake1->x--;
						break;
				case 'r': if (snake1->x==WIDTH)
							snake1->x = 0;
						else
							snake1->x++;
						break;
				case 'u': if (snake1->y==0)
							snake1->y = HEIGHT;
						else
							snake1->y--;
						break;
				case 'd': if (snake1->y==HEIGHT)
							snake1->y = 0;
						else
							snake1->y++;
						break;
			}
		}
		if (set_dir2==0)
		{
			switch(*snake2dir)
			{
				case 'l': if (snake2->x==0)
							snake2->x = WIDTH;
						else
							snake2->x--;
						break;
				case 'r': if (snake2->x==WIDTH)
							snake2->x = 0;
						else
							snake2->x++;
						break;
				case 'u': if (snake2->y==0)
							snake2->y = HEIGHT;
						else
							snake2->y--;
						break;
				case 'd': if (snake2->y==HEIGHT)
							snake2->y = 0;
						else
							snake2->y++;
						break;
			}
		}
		
		s1 = queue_push(s1, *snake1);
		s2 = queue_push(s2, *snake2);
		if ((reward->x!=snake1->x)||(reward->y!=snake1->y))
			s1 = queue_pop(s1);
		else 
			*reward = mk_reward(s1, s2);
		
		if ((reward->x!=snake2->x)||(reward->y!=snake2->y))
			s2 = queue_pop(s2);
		else 
			*reward = mk_reward(s1, s2);

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
//	int snake1y = 0 , snake2y = 0;	// The number of rows the snake head is below the top left
//	int snake1x = 0 , snake2x = WIDTH;			
	coord snake1 = make_coord(0, 0);
	coord snake2 = make_coord(WIDTH, 0);
	Queue *s1 = queue_new();
	s1 = queue_push(s1, snake1);
	Queue *s2 = queue_new();
	s2 = queue_push(s2, snake2);
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
	printscreen(s1, s2, &reward,&b);
	bombregion br;	
	init_bombregion(&br, b);
	/*int col = collide(s1, s2);
	printw("%d", col);
	getch();*/
	int nextexplode=( rand() % 20 ) * 10 +runno+10;
	int call_no=0;	
	double t2=1000,t1=0;
	do 
	{
//		set_dir1=0;
//		set_dir2=0;
		gettimeofday(&tim, NULL);
		runno++;
		if(b.isactive)
		{
			if(b.duration == MAXDURATION )
			{
				explode(s1,s2, &b, &call_no, br);
				
				//	b.isactive=0;
			//	b.duration=0;

				nextexplode = ( rand() % 20 )  +runno + 20 ;
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
		
			init_bombregion(&br,b);
		}
			
			        	
		t1 = tim.tv_sec*1000 + ( tim.tv_usec / 1000.0 );  
		timeout(100); 		
		ch = play_one(s1, s2, &snake2, &snake2dir, &reward); 		
		if (ch==1)
			break;
		auto_move(s1, s2, &snake1dir, &reward, &b);
		int col = collide(s1, s2, snake1dir, snake2dir);
	//	int col=0;
		end_game(col,s1,s2);	
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
			
		
		printscreen(s1, s2, &reward,&b);	
		refresh();
	}	
	while (ch!='q');
	clrtoeol();
	refresh();
	endwin();
} 
