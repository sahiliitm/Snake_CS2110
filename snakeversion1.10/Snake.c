#include "Snake.h"
#include<stdlib.h>
#include<stdio.h>
#include"Queue.h"
#include <ncurses.h>
#include <sys/time.h>
#define BOMBCHAR "*"

/* Purpose  :   Initializing and constructing a structure of coordinates(x,y) as given through inputs
   Inputs   :  x   -   x coordinate    
            :  y   -   y coordinate 
   Output   :  c   -   coordinate structure
*/

coord make_coord(int x,int y)
{
	coord c;
	c.x = x;
	c.y = y;
	return c;
}

/* Purpose  :   While computing a bomb region with coordinates of the form :  2 above row 0 taking modulo ( a%b) gives negative numbers 
                and we try to access  invalid board positions hence this function basically works like a :  x modulo dimention 
   Inputs   :  x               -    input to be reduced modulo dimention   
            :  dimention       -    modular base to work with
   Output   :  x + dimention   -    in case x < 0
               x - dimention   -    when x > dimention
               x               -    x otherwise
*/



int correct_value(int x, int dimention )
{
	if( x < 0 )
	return x + dimention;
	else if ( x > dimention)
	return x - dimention;
	else
	return x;
}	

/* Purpose  :   Decide the limits of the bomb region based on the coordinates of the bomb center
   Inputs   :  * br      -    Structure containing the coordinates of all points in bomb region   
            :    b       -    Structure containing info about the bomb
  */


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
 
/* Purpose  :   Checks if coord x1 is near coord x2 , two coords are near iff the manhattan distance b/w their centers is
                <= 2
   Inputs   :   x1      -    Coordinate pair 1
            :   x2      -    Coordinate pair 2
   Output   :    1      -    if x1 and x2 are near each other
                 0      -    otherwise
 */

int coord_near(coord x1, coord x2)
{
	int distx=0, disty=0;
	distx+=(x1.x>x2.x)?(x1.x-x2.x):(x2.x-x1.x);
	if (distx>WIDTH/2)
		distx = WIDTH-distx;
	disty+=(x1.y>x2.y)?(x1.y-x2.y):(x2.y-x1.y);
	if (disty>HEIGHT/2)
		disty = HEIGHT-disty;
	if (  distx+disty <=2 )
	return 1;
	return 0; 
}


/* Purpose  :   Checks if the potential reward at position i is near the already existant rewards
   Inputs   :   r       -    Reward structure
            :   i       -    Position of new potential reward
   Output   :    1      -    if reward ar position i is near any reward in positions 0 to i-1
                 0      -    otherwise
 */

int is_coord_near_reward ( reward * r , int i )
{
    int j;
    for(j = i-1 ; j >= 0 ; j-- )
    if( coord_near( r->rewcoord[j], r->rewcoord[i] ) )
    return 1;
    return 0;
}

/* Purpose  :   Initialize the rewards at the starting of the game depending on the snakes are 
   Inputs   :   s1       -    Snake 1
            :   s2       -    Snake 2
            :   *r       -    Reward structure
 */
 
void init_reward ( Queue * s1 , Queue * s2 , reward * r)
{
    int i = 0 ;
    coord temp;
    for( i = 0 ; i < r->rewnum ; i++ )
    {
        do
        {
            temp.x = rand()%WIDTH;
            temp.y = rand()%HEIGHT;
            r->rewcoord[i].x = temp.x ;
            r->rewcoord[i].y = temp.y ;
        }
           while (  queue_search ( s1,temp ) || queue_search ( s2,temp ) || is_coord_near_reward ( r , i ) ) ;
    }      
}


/* Purpose  :   This fucntion is called when a reward gets eaten and generates a new reward in it's liu 
   Inputs   :   s1             -    Snake 1
            :   s2             -    Snake 2
            :   *r             -    Reward structure
            :   reawardeaten   -    Position of eaten reward in array of rewards
 */
 

reward * mk_reward(Queue *s1, Queue *s2, reward * r, int rewardeaten)
{
	int i;
	coord temp;
	coord tem = r->rewcoord[rewardeaten];
    r->rewcoord[rewardeaten] = r->rewcoord[r->rewnum-1];
    r->rewcoord[r->rewnum-1] = tem;
	do
        {
            temp.x = rand()%WIDTH;
            temp.y = rand()%HEIGHT;
            r->rewcoord[r->rewnum-1].x = temp.x ;
            r->rewcoord[r->rewnum-1].y = temp.y ;
        }
           while (  queue_search ( s1,temp ) || queue_search ( s2,temp ) || is_coord_near_reward ( r , r->rewnum - 1 ) ) ;
    return r;       
}


/* Purpose  :   Checks whether a given coordinate is inside the bomb region
   Inputs   :   c           -    Coordinate to be checked
            :   br          -    Bomb region
 */
 
int coord_in_bomb(coord c, bombregion br)
{
	int i;
	for( i = 0 ; i < SIZEBOMBREGION ; i++ )
	if( br.array[i].x == c.x && br.array[i].y == c.y )
	return 1;
	return 0;
}


/* Purpose  :   Initializes the points at the starting of the game
   Output   :   *p          -   structure of points
 */

Points* mk_points(){
	Points* p=malloc(sizeof(Points));
	p->s1_points=1;
	p->s2_points=1;
	
	return p;
}


/* Purpose  :   Get the current points of both players
   Inputs   :    s1  -          snake 1
            :    s2  -          snake 2
            :    *p  -          Points structure pointer
 */

void get_points(Queue* s1,Queue* s2,Points* p){
	p->s1_points=queue_size(s1);
	p->s2_points=queue_size(s2);

}


/* Purpose  :   Dump the current posotions of bomb rewards and snakes onto the screen
   Inputs   :    s1         -          snake 1
            :    s2         -          snake 2
            :    *p         -          Points structure pointer
            :    *r         -          Rewards structure pointer
            :    *b         -          Pointer to bomb
            :    * call_no  -          Call number to printscreen used to decide which stage of bomb are we on..
            :    *br        -          Bomb region
            :    ptime      -          Current time
            :    stime      -          Starting time of game
            :    mtime      -          maximum time of the game( in minutes )
 */         

void printscreen(Queue * s1, Queue * s2, reward  *r , bomb * b , int * call_no, bombregion * br, Points* p,double ptime,double stime,int mtime)
{
	clear();
	int i,j;
	attron(COLOR_PAIR(10));
	for(i=0;i<=WIDTH;i++)
	for(j=0;j<=HEIGHT+2;j++)
	mvprintw(j, i, " ");
	int k=0;
	if (r!=NULL)
	{
	for( k = 0 ; k < r->rewnum ; k++)
	{
	
	        if (   r->rewcoord[k].x >= 0 && r->rewcoord[k].y >= 0  )
		        {
			    
    			    if (   (         ((int)(ptime-stime) %60000) / 500    )    % 2 == 0 ) 
	    		    {
	    			    attron(COLOR_PAIR(13));
	    			    mvprintw(r->rewcoord[k].y,correct_value( r->rewcoord[k].x-2,WIDTH), "--");
	    			    mvprintw(r->rewcoord[k].y, correct_value(r->rewcoord[k].x+1,WIDTH), "--");
	    			    mvprintw(  correct_value( r->rewcoord[k].y-1,HEIGHT),correct_value( r->rewcoord[k].x+1,WIDTH), "\\");
	    			    mvprintw( correct_value( r->rewcoord[k].y-1,HEIGHT),correct_value( r->rewcoord[k].x-1,WIDTH), "/");
	    			    mvprintw( correct_value( r->rewcoord[k].y+1,HEIGHT),correct_value( r->rewcoord[k].x-1,WIDTH), "\\");
	    			    mvprintw( correct_value(r->rewcoord[k].y+1,HEIGHT),correct_value( r->rewcoord[k].x+1,WIDTH), "/");
	    			    attron(COLOR_PAIR(9));
	    			    mvprintw( correct_value( r->rewcoord[k].y-1,HEIGHT), r->rewcoord[k].x, "v");					
        				mvprintw(  correct_value(r->rewcoord[k].y+1,HEIGHT), r->rewcoord[k].x, "^");			
	        			attron(COLOR_PAIR(5));
	        			mvprintw(r->rewcoord[k].y, r->rewcoord[k].x, "O");
	        		}	
	        		else
	        		{
	        			attron(COLOR_PAIR(13));
	        			mvprintw(r->rewcoord[k].y,correct_value( r->rewcoord[k].x-1,WIDTH), "-");
	        			mvprintw(r->rewcoord[k].y, correct_value(r->rewcoord[k].x+1,WIDTH), "-");
	        			mvprintw(  correct_value( r->rewcoord[k].y+1,HEIGHT),correct_value( r->rewcoord[k].x+1,WIDTH), "\\");
	        			mvprintw( correct_value( r->rewcoord[k].y+1,HEIGHT),correct_value( r->rewcoord[k].x-1,WIDTH), "/");
	        			mvprintw( correct_value( r->rewcoord[k].y-1,HEIGHT),correct_value( r->rewcoord[k].x-1,WIDTH), "\\");
	        			mvprintw( correct_value(r->rewcoord[k].y-1,HEIGHT),correct_value( r->rewcoord[k].x+1,WIDTH), "/");
	           			attron(COLOR_PAIR(9));
	        			mvprintw( correct_value( r->rewcoord[k].y+1,HEIGHT), r->rewcoord[k].x, "v");				
	        			mvprintw(  correct_value(r->rewcoord[k].y-1,HEIGHT), r->rewcoord[k].x, "^");			
	        			attron(COLOR_PAIR(5));
	        			mvprintw(r->rewcoord[k].y, r->rewcoord[k].x, "o");
	        		} 
	    	
	    	}
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


/* Purpose  :   Ending game printscreen 
   Inputs   :    s1         -          snake 1
            :    s2         -          snake 2
            :    *p         -          Points
            :    col        -          whether collision happende and if yes who collided into whom
 */   
 
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
		
		sleep(1);
        timeout(4000);
        getch();		
		clrtoeol();
		refresh();
		endwin();
		exit(0);
}

/* Purpose  :   To modify the current snake based on it's relationship with the current bomb position 
   Inputs   :    s1         -          snake 1
            :    s2         -          snake 2
            :    *br        -          Bomb region
   Output   :    1          -          if snake is being nodified
                 0          -          otherwise 
 */         

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


/* Purpose  :   Print the exploding bomb
   Inputs   :    s1             -          snake 1
            :    s2             -          snake 2
            :    *p             -          Points
            :    *call_no       -          Keep a track of what tage of bomb printing are we on
            :    *b             -          Bomb 
            :     br            -          Bomb region
 */   
 

int explode(Queue *s1, Queue *s2, bomb *b, int *call_no,bombregion br, Points* p)
{

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


/* Purpose  :   Checks whether snake 1 collided with snake 2 or vice versa or neither or whether both collided with each other 
   Inputs   :    s1         -          snake 1
            :    s2         -          snake 2
            :    dir1       -          direction of snake 1
            :    dir2       -          direction of snake 2
 */   

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
