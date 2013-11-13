#include "Snake.h"
#include<stdlib.h>
#include<stdio.h>
#include"Queue.h"
#include <ncurses.h>
#include <sys/time.h>
#define BOMBCHAR "*"

/* Making a structure of coordinates as given, sort of like a constructor*/

coord make_coord(int x,int y)
{
	coord c;
	c.x = x;
	c.y = y;
	return c;
}

/* While computing a bomb region with coordinates of the form 2 above row 0 taking modulo gives negative numbers and we try to access 
invalid board positions hence this function basically works like a x modulo dimention */

int correct_value(int x, int dimention )
{
	if( x < 0 )
	return x + dimention;
	else if ( x >= dimention)
	return x - dimention;
	else
	return x;
}	

/* Decide the limits of the bomb region based on the coordinates of the bomb center*/

void init_bombregion( bombregion * br , bomb b)
{
	int i,k=0;
	br->array[k].x = correct_value( b.topleft.x  , WIDTH );
	br->array[k++].y = correct_value( b.topleft.y - 4 , HEIGHT );
	br->array[k].x = correct_value( b.topleft.x  , WIDTH );
	br->array[k++].y = correct_value( b.topleft.y + 4 , HEIGHT );
	
	
	for(i=-1;i <= 1;i++)
	{
			br->array[k].x = correct_value( b.topleft.x + i , WIDTH );
			br->array[k++].y = correct_value( b.topleft.y  -3 ,HEIGHT );
			br->array[k].x = correct_value( b.topleft.x+i,WIDTH);
			br->array[k++].y = correct_value( b.topleft.y+3,HEIGHT);
	}
	
	for(i=-3;i <= 3;i++)
	{
			br->array[k].x = correct_value( b.topleft.x +i,WIDTH);
			br->array[k++].y=correct_value( b.topleft.y-2,HEIGHT);
			br->array[k].x=correct_value( b.topleft.x+i,WIDTH);
			br->array[k++].y=correct_value( b.topleft.y+2,HEIGHT);
	}	
	
	
	for(i=-4;i <= 4;i++)
	{
			br->array[k].x = correct_value( b.topleft.x +i,WIDTH);
			br->array[k++].y=correct_value( b.topleft.y-1,HEIGHT);
			br->array[k].x=correct_value( b.topleft.x+i,WIDTH);
			br->array[k++].y=correct_value( b.topleft.y+1,HEIGHT);
	}	
	
	for(i=-5;i <= 5;i++)
	{
			br->array[k].x = correct_value( b.topleft.x +i,WIDTH);
			br->array[k++].y = correct_value( b.topleft.y,HEIGHT);
	}

}



coord mk_reward(Queue *s1, Queue *s2)
{
//	Snake *reward;
	int x = rand()%WIDTH;
	int y = rand()%HEIGHT;
	coord data;
	data.x = x;
	data.y = y;
//	data.x = 10;
//	data.y = 10;
	
	return data;
}

int coord_in_bomb(coord c, bombregion br)
{
	int i;
	for(i=0;i<SIZEBOMBREGION;i++)
	if( br.array[i].x == c.x && br.array[i].y == c.y )
	return 1;
	return 0;
}

Points* mk_points(){
	Points* p=malloc(sizeof(Points));
	p->s1_points=1;
	p->s2_points=1;
	
	return p;
}

void get_points(Queue* s1,Queue* s2,Points* p){
	p->s1_points=queue_size(s1);
	p->s2_points=queue_size(s2);

}

/*Print the current status of the screen */

void printscreen(Queue * s1, Queue * s2, coord * reward , bomb * b , int * call_no, bombregion * br, Points* p,double ptime,double stime,int mtime)
{
	clear();
	int i,j;
	attron(COLOR_PAIR(10));
	for(i=0;i<=WIDTH;i++)
	for(j=0;j<=HEIGHT+2;j++)
	mvprintw(j, i, " ");
	
	if (reward!=NULL)
		{
			
			if (   (         ((int)(ptime-stime) %60000) / 500    )    % 2 == 0 ) 
			{
				attron(COLOR_PAIR(13));
				mvprintw(reward->y,correct_value( reward->x-2,WIDTH), "--");
				mvprintw(reward->y, correct_value(reward->x+1,WIDTH), "--");
				mvprintw(  correct_value( reward->y-1,HEIGHT),correct_value( reward->x+1,WIDTH), "\\");
				mvprintw( correct_value( reward->y-1,HEIGHT),correct_value( reward->x-1,WIDTH), "/");
				mvprintw( correct_value( reward->y+1,HEIGHT),correct_value( reward->x-1,WIDTH), "\\");
				mvprintw( correct_value(reward->y+1,HEIGHT),correct_value( reward->x+1,WIDTH), "/");
				
				attron(COLOR_PAIR(9));
				mvprintw( correct_value( reward->y-1,HEIGHT), reward->x, "v");					
				mvprintw(  correct_value(reward->y+1,HEIGHT), reward->x, "^");			
				attron(COLOR_PAIR(5));
				mvprintw(reward->y, reward->x, "O");
			}	
			else
			{
				attron(COLOR_PAIR(13));
				mvprintw(reward->y,correct_value( reward->x-1,WIDTH), "-");
				mvprintw(reward->y, correct_value(reward->x+1,WIDTH), "-");
				mvprintw(  correct_value( reward->y+1,HEIGHT),correct_value( reward->x+1,WIDTH), "\\");
				mvprintw( correct_value( reward->y+1,HEIGHT),correct_value( reward->x-1,WIDTH), "/");
				mvprintw( correct_value( reward->y-1,HEIGHT),correct_value( reward->x-1,WIDTH), "\\");
				mvprintw( correct_value(reward->y-1,HEIGHT),correct_value( reward->x+1,WIDTH), "/");
			
				attron(COLOR_PAIR(9));
				mvprintw( correct_value( reward->y+1,HEIGHT), reward->x, "v");				
				mvprintw(  correct_value(reward->y-1,HEIGHT), reward->x, "^");			
				attron(COLOR_PAIR(5));
				mvprintw(reward->y, reward->x, "o");
			} 
		
		}
	
	attron(COLOR_PAIR(1));
	if( s1->head != NULL )
	{
		Node * q = s1->tail;
		while( q != s1->head )
		{		
			mvprintw(q->data.y, q->data.x, " ");
			q = q->next;	
		}	
		attron(COLOR_PAIR(2));
		mvprintw(q->data.y, q->data.x, " ");		
	}
	if( s2->head != NULL )
	{
		attron(COLOR_PAIR(3));
		Node * q = s2->tail;	
		while( q != s2->head )
		{
			mvprintw(q->data.y, q->data.x, " ");
			q = q->next;	
		}
		attron(COLOR_PAIR(4));
		mvprintw(q->data.y, q->data.x, " ");	
	}

	
	attron(COLOR_PAIR(6));
	if( (b!=NULL) && (b->isactive ) )
	
	{
		switch( b->duration%4 )
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
	}
	if ( b!=NULL && b->isexploding && call_no != NULL )
			explode(s1,s2, b, call_no, *br, p);
	attron(COLOR_PAIR(1));
	if(p!=NULL)
	{
		mvprintw(HEIGHT+1,WIDTH-20,"Player 1: %d",(p->s1_points));
		mvprintw(HEIGHT+2,WIDTH-20,"Player 2: %d",(p->s2_points));	
	}
	int min = mtime  - (ptime-stime)/60000;
	int sec = 60 - (    ((int) (ptime-stime)) %60000)/1000;
	if(mtime!=0)
	{
		mvprintw(HEIGHT+1,WIDTH/2-7,"Time Remaining");
		mvprintw(HEIGHT+2,WIDTH/2 -3,"%d : %d",min,sec);
	}
	mvprintw(HEIGHT, WIDTH,"");
	refresh();
}

/*To determine whether the game has ended and if it has print the appropropriate message */

void end_game( int col , Queue * s1 , Queue * s2, Points* p)
{
		if ( col == 0)
			return;
		
		clear();
		printscreen(s1, s2, NULL, NULL,NULL, NULL,p,0,0,0);
		attron(COLOR_PAIR(1));		
		if ( col == 3)
		{
			if( p->s1_points == p->s2_points  )
			mvprintw(HEIGHT/2, WIDTH/2, "Draw");
			else if ( p->s1_points > p->s2_points  )	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 1 wins!");
			else 	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 2 wins!");
		}		
		else if (col == 2 )
			mvprintw(HEIGHT/2, WIDTH/2, "Draw");
		else if ( col==1 )	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 2 wins!");
		else if ( col==-1 )	
			mvprintw(HEIGHT/2, WIDTH/2, "Snake 1 wins!");
		
		timeout(5000);
		getch();		
		clrtoeol();
		refresh();
		endwin();
		exit(0);
}

/* To modify the current snake based on it's relationship with the current bomb position Deals with
   	memory and hence can give seg faults if not written properly :P */

int modify_snake(Queue* s1,bomb *b,bombregion br)
{
	Node * n = s1->tail;
	Node * last_in_bomb = NULL;

	/* The case where the head of the snake is out of the bomb, in this case ONLY the
	   head segment survives and the rest of the snake dies off */
	
	if ( ! coord_in_bomb(s1->head->data, br) )
	{	
		while (n!=NULL)
		{
			if ( coord_in_bomb(n->data, br) )
				last_in_bomb = n;	
			n=n->next;
		}
		
		if ( last_in_bomb != NULL )
		{
			Node *temp= s1->tail,*temp2;
			s1->tail = last_in_bomb->next;
			while( temp != s1->tail)
			{
				temp2 = temp;
				temp = temp->next;
				free(temp2);	
			}	
		}
	}

	/* In the following cases the head of the snake is INSIDE the bomb */

	else 
	{
		//DBUG(2);
		Node * last_not_in_bomb = NULL;
		Node * last_in_nextout = NULL;
		Node * last_out_next_in = NULL;
		Node * last_in_next_out = NULL;		
		n = s1->tail;
			
		while ( n->next != NULL)
		{
			if ( coord_in_bomb( n->data, br) && !coord_in_bomb(n->next->data, br) )
				last_in_next_out = n;
			else if ( !coord_in_bomb(n->data, br) && coord_in_bomb(n->next->data, br) )
				last_out_next_in = n;
			n = n->next;
		}	

		/* This case is when the snake is entirely inside the bomb */
		
		if ( last_out_next_in == NULL && last_in_next_out==NULL )
		{
			Node *temp= s1->tail,*temp2;
			s1->tail = NULL;
			s1->head = NULL;
			while(temp!=NULL)
			{
				temp2 = temp;
				temp = temp->next;				
				free(temp2);	
			}	
			return 1;
		}
		
		/* This case is when the snake only goes into the bomb once and 
		    never comes out */

		else if ( last_in_next_out == NULL)
		{
									//	DBUG(4)
			Node *temp = last_out_next_in->next , *temp2;
			s1->head = last_out_next_in;
			while(temp!=NULL)
			{
				temp2 = temp;
				temp = temp->next;				
				free(temp2);
					
			}		
		}

		/* This is the case where the snake goes into the bomb comes out and goes in again */
		
		else
		{
									//	DBUG(5)
			Node *temp = s1->tail , *temp2;
			s1->head = last_out_next_in;
			s1->tail = last_in_next_out->next;
			while(temp!=s1->tail)
			{
				temp2 = temp;
				temp = temp->next;
				free(temp2);	
			}		
		
			temp = s1->head->next;
			while(temp!=NULL)
			{
				temp2 = temp;
				temp=temp->next;
				free(temp2);	
			}	
		
		}
	}
	return 0;
}
//reqrite this urgently
int explode(Queue *s1, Queue *s2, bomb *b, int *call_no,bombregion br, Points* p)
{
//modify_snake(s1,b);

	int i, j;
	if (*call_no>=0 && *call_no <=0)
	{
		attron(COLOR_PAIR(11));
		mvprintw(b->topleft.y, b->topleft.x, BOMBCHAR);
		
	}
	else if (*call_no >=1 && *call_no <= 1 )
	{
		attron(COLOR_PAIR(12));
		for ( i = -1 ; i <= 1 ; i++)
		{
			mvprintw( correct_value( b->topleft.y - 1 , HEIGHT ) , 
				  correct_value( b->topleft.x + i , WIDTH ) , 
				    BOMBCHAR );
			mvprintw( correct_value( b->topleft.y + 1 , HEIGHT ) , 
				  correct_value( b->topleft.x + i , WIDTH ), 
				  BOMBCHAR );
		}
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x - 2,WIDTH), BOMBCHAR);
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x - 1,WIDTH), BOMBCHAR);
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x + 1,WIDTH), BOMBCHAR);
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x + 2,WIDTH), BOMBCHAR);			
		
	}	
	else if( *call_no >= 2 && *call_no<=2 )
	{
		
		attron(COLOR_PAIR(13));
		mvprintw(correct_value(b->topleft.y - 2,HEIGHT),correct_value(b->topleft.x,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 2,HEIGHT),correct_value(b->topleft.x,WIDTH) , BOMBCHAR);
		
		mvprintw(correct_value(b->topleft.y - 1,HEIGHT),correct_value(b->topleft.x - 2,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y - 1,HEIGHT),correct_value(b->topleft.x + 2,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 1,HEIGHT),correct_value(b->topleft.x - 2,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 1,HEIGHT),correct_value(b->topleft.x + 2,WIDTH) , BOMBCHAR);
		
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x - 3,WIDTH), BOMBCHAR);
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x + 3,WIDTH), BOMBCHAR);
		
	}	
	else if( *call_no >= 3 && *call_no<=3 )
	{
		int i=0;
		attron(COLOR_PAIR(14));
		mvprintw(correct_value(b->topleft.y - 3,HEIGHT),correct_value(b->topleft.x,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 3,HEIGHT),correct_value(b->topleft.x,WIDTH) , BOMBCHAR);
		
	
		for( i = -2 ; i <= 2 ; i += 4 )
		{
			mvprintw(correct_value(b->topleft.y + i,HEIGHT),correct_value(b->topleft.x - 2,WIDTH) , BOMBCHAR);
			mvprintw(correct_value(b->topleft.y + i,HEIGHT),correct_value(b->topleft.x - 1,WIDTH) , BOMBCHAR);
			mvprintw(correct_value(b->topleft.y + i,HEIGHT),correct_value(b->topleft.x + 1,WIDTH) , BOMBCHAR);
			mvprintw(correct_value(b->topleft.y + i,HEIGHT),correct_value(b->topleft.x + 2,WIDTH) , BOMBCHAR);
		}
		mvprintw(correct_value(b->topleft.y - 1,HEIGHT),correct_value(b->topleft.x - 3,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y - 1,HEIGHT),correct_value(b->topleft.x + 3,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 1,HEIGHT),correct_value(b->topleft.x - 3,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 1,HEIGHT),correct_value(b->topleft.x + 3,WIDTH) , BOMBCHAR);
		
	mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x - 4,WIDTH), BOMBCHAR);
	mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x + 4,WIDTH), BOMBCHAR);
	mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x - 5,WIDTH), BOMBCHAR);
	mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x + 5,WIDTH), BOMBCHAR);
	}	
	
	else if ( *call_no >= 4 && *call_no <= 4 )
	{
		attron(COLOR_PAIR(11));
		mvprintw(correct_value(b->topleft.y-4,HEIGHT),correct_value(b->topleft.x ,WIDTH), BOMBCHAR);
		mvprintw(correct_value(b->topleft.y+4,HEIGHT),correct_value(b->topleft.x ,WIDTH), BOMBCHAR);
	
		mvprintw(correct_value(b->topleft.y - 3,HEIGHT),correct_value(b->topleft.x - 1,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y - 3,HEIGHT),correct_value(b->topleft.x + 1,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 3,HEIGHT),correct_value(b->topleft.x - 1,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 3,HEIGHT),correct_value(b->topleft.x + 1,WIDTH) , BOMBCHAR);
	
		mvprintw(correct_value(b->topleft.y - 2,HEIGHT),correct_value(b->topleft.x - 3,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y - 2,HEIGHT),correct_value(b->topleft.x + 3,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 2,HEIGHT),correct_value(b->topleft.x - 3,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 2,HEIGHT),correct_value(b->topleft.x + 3,WIDTH) , BOMBCHAR);
		
		mvprintw(correct_value(b->topleft.y - 1,HEIGHT),correct_value(b->topleft.x - 4,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y - 1,HEIGHT),correct_value(b->topleft.x + 4,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 1,HEIGHT),correct_value(b->topleft.x - 4,WIDTH) , BOMBCHAR);
		mvprintw(correct_value(b->topleft.y + 1,HEIGHT),correct_value(b->topleft.x + 4,WIDTH) , BOMBCHAR);
		
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x-6 ,WIDTH), BOMBCHAR);
		mvprintw(correct_value(b->topleft.y,HEIGHT),correct_value(b->topleft.x +6,WIDTH), BOMBCHAR);
		
	}
	
	else{
		*call_no=0;
		b->isexploding=0;
		b->duration=0;
		int m1=modify_snake(s1, b,br);
		int m2=modify_snake(s2, b,br);
		get_points(s1,s2,p);
		if(m1==1 && m2==1 )
			end_game(2,s1,s2,p);
		
		else if(m1==0 && m2==1)
			end_game(-1,s1,s2,p);
		
		else if(m1==1 && m2==0)
			end_game(1,s1,s2,p);
		
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




